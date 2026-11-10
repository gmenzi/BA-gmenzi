/*
 * Command handling, cut down from BioGAP core/command_dispatcher.c.
 * USB has no message boundaries, so the first byte gives the message length.
 */

#include "commands.h"
#include "dummy_source.h"
#include "transport.h"
#include "wulpus_appl.h"

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

void handle_connectivity_command(const uint8_t *data, uint16_t size)
{
	switch (data[0]) {
	case START_PPG_STREAMING:
		printf("PPG start: %u Hz\n", data[2]);
		dummy_ppg_start();
		break;

	case STOP_PPG_STREAMING:
		printf("PPG stop\n");
		dummy_ppg_stop();
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
