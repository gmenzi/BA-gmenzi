/*
 * M5: real WULPUS SPI module, PPG still dummy.
 * With WULPUS_LOOPBACK_TEST, a thread plays the MSP on DATA_READY.
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>

#include "commands.h"
#include "dummy_source.h"
#include "transport_usb.h"
#include "wulpus_appl.h"

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

#if defined(WULPUS_LOOPBACK_TEST)
static const struct gpio_dt_spec fake_data_ready =
	GPIO_DT_SPEC_GET(DT_NODELABEL(uups_trig), gpios);

static void fake_msp_thread(void *a, void *b, void *c)
{
	gpio_pin_configure_dt(&fake_data_ready, GPIO_OUTPUT_INACTIVE);

	while (1) {
		k_msleep(33);

		if (wulpus_is_streaming()) {
			gpio_pin_set_dt(&fake_data_ready, 1);
			k_busy_wait(50);
			gpio_pin_set_dt(&fake_data_ready, 0);
		}
	}
}

K_THREAD_DEFINE(fake_msp_tid, 1024, fake_msp_thread, NULL, NULL, NULL, 7, 0, 0);

static void auto_start(void)
{
	static uint8_t pkg[MSP_RESTART_PCK_LEN];

	memset(pkg, 0x55, sizeof(pkg));
	pkg[0] = 0xFA;
	wulpus_set_msp_config(pkg, sizeof(pkg));
}

#endif

int main(void)
{
	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

	wulpus_init();

	if (commands_init() != 0 || usb_transport_init() != 0) {
		return 0;
	}

	printf("M5 WULPUS SPI started\n");

	#if defined(WULPUS_LOOPBACK_TEST)
		k_msleep(1000);
		auto_start();
	#endif

	while (1) {
		gpio_pin_toggle_dt(&led);
		printf("frames=%u spi_errors=%u ppg_packets=%u dropped=%u\n", wulpus_frames(),
		       wulpus_spi_errors(), dummy_ppg_packets(), usb_dropped_packets());
		k_msleep(1000);
	}

	return 0;
}
