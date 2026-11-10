/*
 * Fake ultrasound frames and PPG packets in the final BLE packet format.
 * Replaced by wulpus_appl.c in M5 and ppg_new in M7.
 */

#include "dummy_source.h"
#include "transport.h"

#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/byteorder.h>

#define PKT_SIZE 211

#define US_FRAME_SIZE 804
#define US_CHUNK_SIZE 201
#define US_CHUNKS 4
#define US_SAMPLES 400
#define US_PERIOD_MS 33

#define PPG_HEADER 0x70
#define PPG_TRAILER 0x71
#define PPG_SAMPLES 4
#define PPG_LEDS 3
#define PPG_PERIOD_MS 32

static atomic_t us_on;
static atomic_t ppg_on;
static atomic_t us_frames;
static atomic_t ppg_packets;

static uint8_t us_frame[US_FRAME_SIZE];

void dummy_us_start(void)
/*writes value into atomic_t us_on*/
{
	atomic_set(&us_on, 1);
}

void dummy_us_stop(void)
/*writes value into atomic_t us_on*/
{
	atomic_set(&us_on, 0);
}

void dummy_ppg_start(void)
/*writes value into atomic_t ppg_on*/
{
	atomic_set(&ppg_on, 1);
}

void dummy_ppg_stop(void)
/*writes value into atomic_t ppg_on*/
{
	atomic_set(&ppg_on, 0);
}

uint32_t dummy_us_frames(void)
{
	return atomic_get(&us_frames);
}

uint32_t dummy_ppg_packets(void)
{
	return atomic_get(&ppg_packets);
}

static void build_us_frame(uint16_t frame_nr)
{
	us_frame[0] = 0xFF;
	us_frame[1] = 0;
	sys_put_le16(frame_nr, &us_frame[2]); /*writes value into us_frame[2] and us_frame[3], frame_nr = 16bit value, &us_frame[2] is the address of the first byte */

	for (int i = 0; i < US_SAMPLES; i++) {
		int16_t sample = (int16_t)((((i + frame_nr) % US_SAMPLES) * 5) - 1000); /*calculates fake ultrasound sample*/

		sys_put_le16((uint16_t)sample, &us_frame[4 + (2 * i)]);
	}
}

static void us_thread(void *a, void *b, void *c)
{
	static uint8_t pkt[PKT_SIZE];
	uint16_t frame_nr = 0;

	while (1) {
		k_msleep(US_PERIOD_MS);

		if (!atomic_get(&us_on)) {
			continue;
		}

		uint32_t ts_us = k_cyc_to_us_floor32(k_cycle_get_32());

		build_us_frame(frame_nr);

		for (int i = 0; i < US_CHUNKS; i++) {
			memset(pkt, 0, sizeof(pkt)); 			/*sets all bytes in pkt to 0*/
			pkt[0] = 0x10 + i; 						/*writes value into pkt[0]*/
			sys_put_le16(frame_nr, &pkt[1]); 		/*writes value into pkt[1] and pkt[2]*/
			sys_put_le32(ts_us, &pkt[3]); 			/*writes value into pkt[3], pkt[4], pkt[5] and pkt[6]*/
			memcpy(&pkt[7], &us_frame[i * US_CHUNK_SIZE], US_CHUNK_SIZE); /*copies data from us_frame to pkt*/
			add_data_to_send_buffer(pkt, PKT_SIZE);
		}

		frame_nr++;
		atomic_inc(&us_frames);
	}
}

static void ppg_thread(void *a, void *b, void *c)
{
	static uint8_t pkt[PKT_SIZE];
	uint16_t counter = 0;
	uint32_t sample_nr = 0;

	while (1) {
		k_msleep(PPG_PERIOD_MS);

		if (!atomic_get(&ppg_on)) {
			continue;
		}

		memset(pkt, 0, sizeof(pkt));
		pkt[0] = PPG_HEADER;
		sys_put_le16(counter, &pkt[1]);
		sys_put_le32(k_cyc_to_us_floor32(k_cycle_get_32()), &pkt[3]);
		pkt[7] = 0x01;
		pkt[8] = 0x07;
		pkt[9] = 0;

		uint8_t *out = &pkt[10];

		for (int s = 0; s < PPG_SAMPLES; s++) {
			/* One triangle per second, like a slow pulse. */
			uint32_t pos = sample_nr % 125;
			uint16_t wave = (uint16_t)((pos < 62 ? pos : 125 - pos) * 100);

			for (int led = 0; led < PPG_LEDS; led++) {
				sys_put_le16((uint16_t)(20000 + (led * 5000) + wave), out);
				out += 2;
			}
			sample_nr++;
		}

		pkt[PKT_SIZE - 1] = PPG_TRAILER;
		add_data_to_send_buffer(pkt, PKT_SIZE);

		counter++;
		atomic_inc(&ppg_packets);
	}
}

K_THREAD_DEFINE(us_tid, 1536, us_thread, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(ppg_tid, 1536, ppg_thread, NULL, NULL, NULL, 5, 0, 0);
