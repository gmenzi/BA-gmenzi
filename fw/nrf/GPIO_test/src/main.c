/*
 * GPIO wire test for the SPI loopback wire P2.02 -> P2.04.
 * P2.02 drives a level, P2.04 reads it back. The pull on P2.04 is set
 * to the opposite level, so an open wire reads wrong.
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* Test 1: P2.02 -> P1.10 (P1.10 known good from M2) */
/* Test 2: P2.06 -> P2.04 (P2.06 known good from M2) */
#define TEST 1

#if TEST == 1
#define OUT_PORT DT_NODELABEL(gpio2)
#define OUT_PIN 2
#define IN_PORT DT_NODELABEL(gpio1)
#define IN_PIN 10
#else
#define OUT_PORT DT_NODELABEL(gpio2)
#define OUT_PIN 6
#define IN_PORT DT_NODELABEL(gpio2)
#define IN_PIN 4
#endif

static const struct device *const out_dev = DEVICE_DT_GET(OUT_PORT);
static const struct device *const in_dev = DEVICE_DT_GET(IN_PORT); 

static int check(int level)
{
	/* Pull P2.04 the other way, so only the wire can bring it to `level`. */
	gpio_flags_t pull = level ? GPIO_PULL_DOWN : GPIO_PULL_UP;

	gpio_pin_configure(in_dev, IN_PIN, GPIO_INPUT | pull);
	gpio_pin_set_raw(out_dev, OUT_PIN, level);
	k_msleep(1);

	return gpio_pin_get_raw(in_dev, IN_PIN);
}

int main(void)
{
	if (!device_is_ready(out_dev) || !device_is_ready(in_dev)) {
		printf("gpio2 not ready\n");
		return 0;
	}

	gpio_pin_configure(out_dev, OUT_PIN, GPIO_OUTPUT_LOW);

	printf("GPIO wire test: P2.02 -> P1.10\n");

	while (1) {
		int low = check(0);
		int high = check(1);

		printf("P2.02=0 -> P2.04=%d | P2.02=1 -> P2.04=%d | %s\n", low, high,
		       (low == 0 && high == 1) ? "WIRE OK" : "NO CONNECTION");

		k_msleep(1000);
	}

	return 0;
}