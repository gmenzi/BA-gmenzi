/*
 * USB CDC ACM link to BioGUI.
 * TX keeps whole packets only, so the 211-byte framing in BioGUI stays aligned.
 * this file is the door to the PC. 
 * Senden:    add_data_to_send_buffer() → Ringpuffer → USB → PC
 * Empfangen: PC → USB → Ringpuffer → rx_thread → transport_rx_bytes()
 * Wenn der Computer nicht schnell genug ist, werden Pakete verworfen. Die Anzahl der verworfenen Pakete kann mit transport_dropped_packets() abgefragt werden.
 */

#include "transport.h"

#include <sample_usbd.h>
#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/ring_buffer.h>
#include <zephyr/usb/usbd.h>

#define TX_BUF_SIZE 8192
#define RX_BUF_SIZE 1024

static const struct device *const uart_dev = DEVICE_DT_GET(DT_NODELABEL(cdc_acm_uart0));

RING_BUF_DECLARE(tx_rb, TX_BUF_SIZE);
RING_BUF_DECLARE(rx_rb, RX_BUF_SIZE);

static K_MUTEX_DEFINE(tx_lock);
static K_SEM_DEFINE(rx_sem, 0, 1);
static atomic_t dropped;
static struct usbd_context *usbd_ctx;

static void usb_msg_cb(struct usbd_context *const ctx, const struct usbd_msg *msg)
/*reacts to connecting/disconnecting USB-cable*/
{
	if (!usbd_can_detect_vbus(ctx)) {
		return;
	}

	if (msg->type == USBD_MSG_VBUS_READY) {
		usbd_enable(ctx);
	} else if (msg->type == USBD_MSG_VBUS_REMOVED) {
		usbd_disable(ctx);
	}
}

static void uart_cb(const struct device *dev, void *user_data)
/*writes bytes between USB and ring buffer*/
{
	while (uart_irq_update(dev) && uart_irq_is_pending(dev)) {
		if (uart_irq_rx_ready(dev)) {
			uint8_t buf[64];
			int len = uart_fifo_read(dev, buf, sizeof(buf));

			if (len > 0) {
				ring_buf_put(&rx_rb, buf, len);
				k_sem_give(&rx_sem);
			}
		}

		if (uart_irq_tx_ready(dev)) {
			uint8_t *p;
			uint32_t len = ring_buf_get_claim(&tx_rb, &p, 512);

			if (len == 0) {
				uart_irq_tx_disable(dev);
			} else {
				int sent = uart_fifo_fill(dev, p, len);

				ring_buf_get_finish(&tx_rb, sent > 0 ? sent : 0);
			}
		}
	}
}

void add_data_to_send_buffer(uint8_t *data, uint16_t size)
{
	k_mutex_lock(&tx_lock, K_FOREVER);

	if (ring_buf_space_get(&tx_rb) < size) {
		atomic_inc(&dropped);
	} else {
		ring_buf_put(&tx_rb, data, size);
	}

	k_mutex_unlock(&tx_lock);
	uart_irq_tx_enable(uart_dev);
}

uint32_t transport_dropped_packets(void)
{
	return atomic_get(&dropped);
}

static void rx_thread(void *a, void *b, void *c)
{
	uint8_t buf[128];

	while (1) {
		k_sem_take(&rx_sem, K_FOREVER);

		uint32_t len;

		while ((len = ring_buf_get(&rx_rb, buf, sizeof(buf))) > 0) {
			transport_rx_bytes(buf, len);
		}
	}
}

K_THREAD_DEFINE(rx_tid, 1536, rx_thread, NULL, NULL, NULL, 6, 0, 0);

int transport_init(void)
{
	if (!device_is_ready(uart_dev)) {
		printf("USB device not ready\n");
		return -ENODEV;
	}

	usbd_ctx = sample_usbd_init_device(usb_msg_cb);
	if (usbd_ctx == NULL) {
		printf("USB init failed\n");
		return -ENODEV;
	}

	if (!usbd_can_detect_vbus(usbd_ctx)) {
		int ret = usbd_enable(usbd_ctx);

		if (ret != 0) {
			printf("USB enable failed: %d\n", ret);
			return ret;
		}
	}

	uart_irq_callback_set(uart_dev, uart_cb);
	uart_irq_rx_enable(uart_dev);

	return 0;
}
