/*
 * Command handling, cut down from BioGAP core/command_dispatcher.c.
 * USB has no message boundaries, so the first byte gives the message length.
 */

#include "commands.h"
#include "transport.h"
#include "wulpus_appl.h"

/* M7: REAL SENSOR (PPG_NEW_APPL.C) OR FAKE DATA (DUMMY_SOURCE.C),
 * PICKED WITH CONFIG_APP_PPG_REAL IN KCONFIG. */
#if defined(CONFIG_APP_PPG_REAL)
#include "ppg_new/ppg_new_appl.h"
#else
#include "dummy_source.h"
#endif

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>

#define START_PPG_STREAMING 39
#define STOP_PPG_STREAMING 40
#define START_WULPUS_STREAMING 41
#define STOP_WULPUS_STREAMING 42

#define MSP_CONFIG_BYTE 0xFA
#define MSP_RESTART_BYTE 0xFB
#define PPG_START_LEN 12

static bool wulpus_restart_rcv;

static uint8_t rx_msg[MSP_RESTART_PCK_LEN];
static uint16_t rx_pos;
static uint16_t rx_need;

static uint16_t message_length(uint8_t first)
{
	switch (first) {
	case START_PPG_STREAMING:
		return PPG_START_LEN;
	case STOP_PPG_STREAMING:
	case START_WULPUS_STREAMING:
	case STOP_WULPUS_STREAMING:
		return 1;
	case MSP_CONFIG_BYTE:
	case MSP_RESTART_BYTE:
		return MSP_RESTART_PCK_LEN;
	default:
		return 0;
	}
}

#if defined(CONFIG_APP_PPG_REAL)
/* COPIED FROM BIOGAP CORE/COMMAND_DISPATCHER.C, CASE START_PPG_STREAMING.
 * THE PARSER ABOVE ALWAYS DELIVERS ALL 12 BYTES, SO THE DEFAULTS ONLY
 * MATTER IF PPG_START_LEN IS EVER CHANGED. */
static void ppg_start_from_command(const uint8_t *data, uint16_t size)
{
      /*
       * Payload layout (bytes after command code 39):
       *   [1]  sensor_mask
       *   [2]  sample_rate_hz
       *   [3]  led_green     (0x00 = disable)
       *   [4]  led_ir        (0x00 = disable)
       *   [5]  led_red       (0x00 = disable)
       *   [6]  led_range     (0-3 index)
       *   [7]  tint          (0-3 index)
       *   [8]  adc_range     (0-3 index)
       *   [9]  sample_avg    (0-7 index)
       *   [10] alc_enable    (0/1)
       *   [11] proximity_enable (0/1)
       * Total packet size: 12 bytes (cmd + 11 config bytes).
       * If the payload is shorter than expected (e.g. the WiFi shield's
       * fixed-size command transaction, which only ever supplies the
       * opcode), missing bytes fall back to these defaults.
       */
      const uint8_t *d = data;
      uint16_t       n = size;

      ppg_config_t pcfg = {
        .sensor_mask      = (n >  1) ? d[1]  : 0x01,
        .sample_rate_hz   = (n >  2) ? d[2]  : 125,
        .led_green        = (n >  3) ? d[3]  : 0x7F,
        .led_ir           = (n >  4) ? d[4]  : 0x7F,
        .led_red          = (n >  5) ? d[5]  : 0x7F,
        .led_range        = (n >  6) ? d[6]  : 3,   /* default 32k */
        .tint             = (n >  7) ? d[7]  : 3,   /* default 117.3 µs */
        .adc_range        = (n >  8) ? d[8]  : 3,   /* default 32k */
        .sample_avg       = (n >  9) ? d[9]  : 0,   /* default 1x */
        .alc_enable       = (n > 10) ? d[10] : 1,   /* default on */
        .proximity_enable = (n > 11) ? d[11] : 0,   /* default off */
      };

      ppg_new_start_streaming(&pcfg);
}
#endif

void handle_connectivity_command(const uint8_t *data, uint16_t size)
{
	switch (data[0]) {
	case START_PPG_STREAMING:
		printf("PPG start: %u Hz\n", data[2]);
#if defined(CONFIG_APP_PPG_REAL)
		ppg_start_from_command(data, size);
#else
		dummy_ppg_start();
#endif
		break;

	case STOP_PPG_STREAMING:
		printf("PPG stop\n");
#if defined(CONFIG_APP_PPG_REAL)
		ppg_new_stop_streaming();
#else
		dummy_ppg_stop();
#endif
		break;

	case START_WULPUS_STREAMING:
		printf("WULPUS start\n");
		wulpus_restart_rcv = true;
		break;

	case STOP_WULPUS_STREAMING:
		printf("WULPUS stop\n");
		wulpus_stop();
		break;

	default:
		if (wulpus_restart_rcv) {
			printf("MSP package 0x%02X, %u bytes\n", data[0], size);
			wulpus_set_msp_config(data, size);
		}
		break;
	}
}

/**
 * Handles received bytes from the transport layer.
 * @param data Pointer to the received data.
 * @param size Number of bytes received.
 */
void transport_rx_bytes(const uint8_t *data, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++) {
		if (rx_pos == 0) {
			rx_need = message_length(data[i]);
			if (rx_need == 0) {
				printf("Unknown byte: 0x%02X\n", data[i]);
				continue;
			}
		}
		/*bytes are collected in rx_msg until rx_need (=complete message) is reached*/
		rx_msg[rx_pos++] = data[i];

		if (rx_pos == rx_need) {
			handle_connectivity_command(rx_msg, rx_need);
			rx_pos = 0;
		}
	}
}

int commands_init(void)
{
	return 0;
}
