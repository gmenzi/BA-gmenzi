/*
 * M5: real WULPUS SPI module, PPG still dummy.
 * With WULPUS_LOOPBACK_TEST, a thread plays the MSP on DATA_READY.
 * M7: REAL PPG SENSOR (MAXM86161) WITH CONFIG_APP_PPG_REAL=Y, OTHERWISE DUMMY.
 * M8: PMIC FIRST, BECAUSE THE PPG'S V_LED COMES FROM SBB2.
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>

#include "commands.h"
#include "transport.h"
#include "wulpus_appl.h"

#if defined(CONFIG_APP_PPG_REAL)
#include "ppg_new/ppg_new_appl.h"
#else
#include "dummy_source.h"
#endif

#if defined(CONFIG_APP_PMIC)
#include "pwr/pwr.h"
#endif

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

#endif

int main(void)
{
	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

#if defined(CONFIG_APP_PMIC)
	/* M8: RAILS FIRST. ON THE OWN BOARD SBB2 (V_LED) MUST BE UP BEFORE
	 * ppg_new_init() READS THE SENSOR. ON THE DK THERE IS NO MAX77654: INIT
	 * FAILS AND EVERYTHING ELSE RUNS ON. */
	if (pwr_init() != 0) {
		printf("PMIC init failed - running without PMIC (expected on the DK)\n");
	} else {
		k_msleep(10); /* M8: LET V_LED SETTLE */
		pwr_log_status();
	}
#endif

	wulpus_init();

#if defined(CONFIG_APP_PPG_REAL)
	/* SAME AS BIOGAP MAIN.C: IF THE SENSOR DOESN'T ANSWER, LOG IT AND KEEP
	 * RUNNING, SO ULTRASOUND STILL WORKS. */
	if (ppg_new_init() != 0) {
		printf("PPG init failed - PPG streaming disabled\n");
	}
#endif

	if (commands_init() != 0 || transport_init() != 0) {
		return 0;
	}

	printf("M8 started\n");

	uint32_t loops = 0;

	while (1) {
#if defined(CONFIG_APP_PMIC)
		/* M8: BATTERY AND CHARGER STATUS EVERY 10 S. NO-OP IF THE PMIC IS MISSING. */
		if (loops++ % 10 == 0) {
			pwr_log_status();
		}
#endif
		gpio_pin_toggle_dt(&led);
#if defined(CONFIG_APP_PPG_REAL)
		/* PPG_NEW HAS NO PACKET COUNTER GETTER. STATE: 0 IDLE, 1 STARTING,
		 * 2 STREAMING, 3 STOPPING, 4 ERROR. */
		printf("frames=%u spi_errors=%u ppg_state=%d dropped=%u\n", wulpus_frames(),
		       wulpus_spi_errors(), (int)ppg_new_get_state(),
		       transport_dropped_packets());
#else
		printf("frames=%u spi_errors=%u ppg_packets=%u dropped=%u\n", wulpus_frames(),
		       wulpus_spi_errors(), dummy_ppg_packets(), transport_dropped_packets());
#endif
		k_msleep(1000);
	}

	return 0;
}
