/*
 * BLE data link (Nordic UART Service) to the dongle and BioGUI.
 * Ported from BioGAP Firmware/src_NRF/ble/bluetooth.c and ble/ble_appl.c
 * (pulp-bio/BioGAP main @ 3cfb671): advertising, connection tuning,
 * send queue and receive queue. GAP9 forwarding, board states, battery
 * status and the throughput test are left out.
 */

#include "transport.h"

#include <string.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <bluetooth/services/nus.h>

LOG_MODULE_REGISTER(transport_ble, LOG_LEVEL_INF);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/* Largest message in either direction: 247-byte MTU minus 3 bytes ATT header. */
#define BLE_PKT_MAX 244

/* 7.5 ms connection interval (in 1.25 ms units), as in BioGAP. */
#define INTERVAL_MIN 6
#define INTERVAL_MAX 6

#define SEND_QUEUE_LEN 40
#define RECV_QUEUE_LEN 8

typedef struct {
	uint16_t size;
	uint8_t data[BLE_PKT_MAX];
} ble_packet_t;

K_MSGQ_DEFINE(send_msgq, sizeof(ble_packet_t), SEND_QUEUE_LEN, 4);
K_MSGQ_DEFINE(recv_msgq, sizeof(ble_packet_t), RECV_QUEUE_LEN, 4);

static struct bt_conn *current_conn;
static struct bt_gatt_exchange_params exchange_params;
static struct k_work advertise_work;
static atomic_t dropped;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_NUS_VAL),
};

static void advertise(struct k_work *work)
{
	int err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));

	if (err) {
		LOG_ERR("Advertising failed to start (err %d)", err);
		return;
	}
	LOG_INF("Advertising as \"%s\"", DEVICE_NAME);
}

static void exchange_func(struct bt_conn *conn, uint8_t att_err,
			  struct bt_gatt_exchange_params *params)
{
	if (!att_err) {
		LOG_INF("MTU exchange done, max payload %u bytes", bt_gatt_get_mtu(conn) - 3);
	} else {
		LOG_WRN("MTU exchange failed (err %u)", att_err);
	}
}

static void tune_connection(struct bt_conn *conn)
{
	static const struct bt_le_conn_param conn_param =
		BT_LE_CONN_PARAM_INIT(INTERVAL_MIN, INTERVAL_MAX, 0, 400);
	static const struct bt_conn_le_phy_param phy = {
		.options = BT_CONN_LE_PHY_OPT_NONE,
		.pref_rx_phy = BT_GAP_LE_PHY_2M,
		.pref_tx_phy = BT_GAP_LE_PHY_2M,
	};
	static const struct bt_conn_le_data_len_param data_len = {
		.tx_max_len = BT_GAP_DATA_LEN_MAX,
		.tx_max_time = BT_GAP_DATA_TIME_MAX,
	};
	int err;

	err = bt_conn_le_param_update(conn, &conn_param);
	if (err) {
		LOG_WRN("Conn param update failed (err %d)", err);
	}

	err = bt_conn_le_phy_update(conn, &phy);
	if (err) {
		LOG_WRN("PHY update failed (err %d)", err);
	}

	err = bt_conn_le_data_len_update(conn, &data_len);
	if (err) {
		LOG_WRN("Data length update failed (err %d)", err);
	}

	exchange_params.func = exchange_func;
	err = bt_gatt_exchange_mtu(conn, &exchange_params);
	if (err) {
		LOG_WRN("MTU exchange failed to start (err %d)", err);
	}
}

static void connected(struct bt_conn *conn, uint8_t err)
{
	char addr[BT_ADDR_LE_STR_LEN];

	if (err) {
		LOG_ERR("Connection failed (err %u)", err);
		return;
	}

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
	LOG_INF("Connected %s", addr);

	current_conn = bt_conn_ref(conn);
	tune_connection(conn);
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason 0x%02x)", reason);

	if (current_conn) {
		bt_conn_unref(current_conn);
		current_conn = NULL;
	}

	/* Drop what was waiting for the old connection. */
	k_msgq_purge(&send_msgq);
}

/* The connection object is free again, so advertising can restart. */
static void recycled(void)
{
	k_work_submit(&advertise_work);
}

static void le_phy_updated(struct bt_conn *conn, struct bt_conn_le_phy_info *param)
{
	LOG_INF("PHY updated: TX %u, RX %u (2 = 2M)", param->tx_phy, param->rx_phy);
}

static void le_data_len_updated(struct bt_conn *conn, struct bt_conn_le_data_len_info *info)
{
	LOG_INF("Data length updated: TX %u, RX %u bytes", info->tx_max_len, info->rx_max_len);
}

static void le_param_updated(struct bt_conn *conn, uint16_t interval, uint16_t latency,
			     uint16_t timeout)
{
	LOG_INF("Conn params updated: interval %u x 1.25 ms", interval);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
	.recycled = recycled,
	.le_param_updated = le_param_updated,
	.le_phy_updated = le_phy_updated,
	.le_data_len_updated = le_data_len_updated,
};

/* NUS RX: the dongle may split one PC message into several writes. */
static void nus_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len)
{
	ble_packet_t pkt;

	if (len > sizeof(pkt.data)) {
		LOG_WRN("NUS RX message truncated (%u bytes)", len);
		len = sizeof(pkt.data);
	}

	pkt.size = len;
	memcpy(pkt.data, data, len);

	if (k_msgq_put(&recv_msgq, &pkt, K_NO_WAIT) != 0) {
		LOG_ERR("NUS RX queue full, %u bytes dropped", len);
	}
}

static struct bt_nus_cb nus_cb = {
	.received = nus_received,
};

void add_data_to_send_buffer(uint8_t *data, uint16_t size)
{
	ble_packet_t pkt;

	if (current_conn == NULL || size > sizeof(pkt.data)) {
		atomic_inc(&dropped);
		return;
	}

	pkt.size = size;
	memcpy(pkt.data, data, size);

	/* Whole packets only: drop it if the queue is full. */
	if (k_msgq_put(&send_msgq, &pkt, K_NO_WAIT) != 0) {
		atomic_inc(&dropped);
	}
}

uint32_t transport_dropped_packets(void)
{
	return atomic_get(&dropped);
}

static void send_thread(void *a, void *b, void *c)
{
	ble_packet_t pkt;

	while (1) {
		k_msgq_get(&send_msgq, &pkt, K_FOREVER);

		if (bt_nus_send(NULL, pkt.data, pkt.size) != 0) {
			atomic_inc(&dropped);
		}
	}
}

static void recv_thread(void *a, void *b, void *c)
{
	ble_packet_t pkt;

	while (1) {
		k_msgq_get(&recv_msgq, &pkt, K_FOREVER);
		transport_rx_bytes(pkt.data, pkt.size);
	}
}

K_THREAD_DEFINE(ble_send_tid, 2048, send_thread, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(ble_recv_tid, 2048, recv_thread, NULL, NULL, NULL, 6, 0, 0);

int transport_init(void)
{
	int err;

	k_work_init(&advertise_work, advertise);

	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return err;
	}

	err = bt_nus_init(&nus_cb);
	if (err) {
		LOG_ERR("NUS init failed (err %d)", err);
		return err;
	}

	k_work_submit(&advertise_work);
	return 0;
}
