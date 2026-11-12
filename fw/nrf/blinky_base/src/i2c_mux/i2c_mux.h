/*
 * STUB FOR BIOGAP i2c_mux/I2C_MUX.H (TCA9548A DRIVER).
 *
 * THE PATCH (AND THE LAB'S PPG BOARD) HAS ONE MAXM86161 WIRED DIRECTLY TO
 * I2C, WITHOUT A MULTIPLEXER. PPG_NEW_APPL.C STILL CALLS THE MUX FUNCTIONS,
 * SO THEY EXIST HERE AS EMPTY FUNCTIONS THAT RETURN 0. THAT WAY
 * PPG_NEW_APPL.C STAYS ALMOST UNCHANGED.
 *
 * I2C_MUX_MAX_CHANNELS = 1: THE ARRAYS IN PPG_NEW_APPL.C ARE SIZED FOR ONE
 * SENSOR, AND THE CHANNEL LOOPS RUN ONCE (CHANNEL 0, SENSOR_MASK BIT 0).
 * THE FUNCTION NAMES AND PARAMETERS ARE THE SAME AS IN BIOGAP.
 */

#ifndef I2C_MUX_H
#define I2C_MUX_H

#include <stdint.h>
#include <zephyr/device.h>

#define I2C_MUX_ADDR_DEFAULT  0x70U
#define I2C_MUX_MAX_CHANNELS  1U

static inline int i2c_mux_init(const struct device *i2c_dev, uint8_t mux_addr)
{
	ARG_UNUSED(i2c_dev);
	ARG_UNUSED(mux_addr);
	return 0;
}

static inline int i2c_mux_select_channel(uint8_t channel)
{
	ARG_UNUSED(channel);
	return 0;
}

static inline int i2c_mux_deselect_all(void)
{
	return 0;
}

static inline int i2c_mux_broadcast(uint8_t channel_mask)
{
	ARG_UNUSED(channel_mask);
	return 0;
}

#endif /* I2C_MUX_H */
