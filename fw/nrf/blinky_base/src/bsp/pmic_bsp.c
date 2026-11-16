/* M8: COPIED UNCHANGED FROM SENSEI-SDK NRF/SYSTEM/BSP (@ D8F4662). I2C ALIAS i2ca AND THE ADC CHANNEL COME FROM THE OVERLAY. */

/*
 * ----------------------------------------------------------------------
 *
 * File: pmic_bsp.c
 *
 * Last edited: 30.10.2025
 *
 * Copyright (c) 2024 ETH Zurich and University of Bologna
 *
 * Authors:
 * - Philipp Schilk (schilkp@ethz.ch), ETH Zurich
 * - Philip Wiese (wiesep@iis.ee.ethz.ch), ETH Zurich
 *
 * ----------------------------------------------------------------------
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "bsp/pmic_bsp.h"

#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// ======== Defines/Variables ======================================================================

LOG_MODULE_REGISTER(pmic_bsp);

static const struct device *pwr_i2c = DEVICE_DT_GET(DT_ALIAS(i2ca));

#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)
static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET_BY_IDX(ZEPHYR_USER_NODE, 0);

// ======== Functions ==============================================================================

void pmic_bsp_log(char *msg, bool is_err, bool has_int_arg, uint32_t arg) {
  if (is_err) {
    if (has_int_arg) {
      LOG_ERR("%s %i", msg, arg);
    } else {
      LOG_ERR("%s", msg);
    }
  } else {
    if (has_int_arg) {
      LOG_INF("%s %i", msg, arg);
    } else {
      LOG_INF("%s", msg);
    }
  }
}

max77654_err_t pmic_bsp_read_regs(uint8_t reg_adr, uint32_t n, uint8_t *buf) {
  if (i2c_burst_read(pwr_i2c, MAX77654_I2C_ADR_7B, reg_adr, buf, n) != 0) {
    return E_MAX77654_COM_ERR;
  }
  return E_MAX77654_SUCCESS;
}

max77654_err_t pmic_bsp_write_regs(uint8_t reg_adr, uint32_t n, const uint8_t *buf) {
  if (i2c_burst_write(pwr_i2c, MAX77654_I2C_ADR_7B, reg_adr, buf, n) != 0) {
    return E_MAX77654_COM_ERR;
  }
  return E_MAX77654_SUCCESS;
}

max77654_err_t pmic_bsp_adc_read(struct max77654_adc_reading *reading) {

  k_sleep(K_MSEC(1));

  if (adc_channel_setup_dt(&adc_channel)) {
    LOG_ERR("ADC Init failed!");
    return E_MAX77654_COM_ERR;
  };

  int16_t buf;
  struct adc_sequence sequence = {
      .buffer = &buf,
      .buffer_size = sizeof(buf), // buffer size in bytes, not number of samples
  };

  adc_sequence_init_dt(&adc_channel, &sequence);
  if (adc_read(adc_channel.dev, &sequence) < 0) {
    LOG_ERR("ADC Reading error!");
    return E_MAX77654_COM_ERR;
  }

  reading->reading = buf < 0 ? 0 : buf;
  reading->adc_fullscale_mV = adc_channel.vref_mv;
  reading->adc_max_reading = (0x1 << adc_channel.resolution) - 1;

  return E_MAX77654_SUCCESS;
}

void pmic_i2c_reset_hotfix() {
  uint8_t buf = 0;
  LOG_ERR("Ignore I2C error - PMIC HOTFIX");
  i2c_burst_write(pwr_i2c, 0, 0, &buf, 1);
}
