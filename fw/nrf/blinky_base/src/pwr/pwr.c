/*
 * ----------------------------------------------------------------------
 *
 * File: pwr.c
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

/*
 * M8: CUT DOWN FROM SENSEI-SDK NRF/SYSTEM/PWR/PWR.C (@ D8F4662).
 *  - NO max77654_reset() BEFORE max77654_init(). THE RESET IS A SOFTWARE COLD
 *    RESET: IT SWITCHES ALL RAILS OFF AND BACK ON. ON THE SENSEI BOARD THE NRF
 *    HAS ITS OWN SUPPLY. ON THIS BOARD IT RUNS FROM SBB1 (DVDD_3.3), SO THE
 *    RESET WOULD RESTART THE NRF, WHICH WOULD RESET THE PMIC AGAIN: BOOT LOOP.
 *  - NO GAP9 AND LED GPIOS, NO pwr_bsp_init() (BIOGAP: GAP9 AND SOFT-RESET
 *    PINS ONLY), NO thread_pwr (BATTERY PERCENT COMES LATER), NO pwr_start()
 *    AND pwr_kill().
 *  - NEW: PMIC_IRQ INTERRUPT -> WORK ITEM THAT READS AND LOGS THE FLAGS.
 *  - NEW: pwr_bat_mV() MEASURES DIRECTLY INSTEAD OF READING thread_pwr'S VALUE.
 */

#include "pwr.h"

#include "bsp/pmic_bsp.h"
#include "config/pmic_default.h"
#include "pwr/pwr_common.h"

#include "max77654.h"

#include <string.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// ======== Defines/Variables ======================================================================

LOG_MODULE_REGISTER(pwr);

static const struct device *pwr_i2c = DEVICE_DT_GET(DT_ALIAS(i2ca));

/* M8: nIRQ OF THE PMIC (OPEN DRAIN, R34 PULL-UP). P0.05 IN THE OVERLAY. */
static const struct gpio_dt_spec pmic_irq_gpio = GPIO_DT_SPEC_GET(DT_NODELABEL(pmic_irq), gpios);
static struct gpio_callback pmic_irq_cb;

// PWR mutex:
// Has to be held to operate on the power subsys. For the public pwr interface this is managed automatically.
K_MUTEX_DEFINE(pwr_mutex);

// Driver handle:
struct max77654_h pmic_h = {
    .read_regs = pmic_bsp_read_regs,
    .write_regs = pmic_bsp_write_regs,
    .adc_read = pmic_bsp_adc_read,
    .log = pmic_bsp_log,
    .conf = PMIC_CONFIG_DEFAULT,
};

atomic_t pwr_is_initialised = ATOMIC_INIT(0);

// ======== Interrupt ==============================================================================

/* M8: THE INTERRUPT ONLY SCHEDULES WORK. I2C MUST NOT RUN IN INTERRUPT
 * CONTEXT, SAME IDEA AS data_rdy_gpio_cb() AND ppg_intb_isr(). */
static void pmic_irq_work_fn(struct k_work *work)
{
	ARG_UNUSED(work);
	struct max77654_int flags = {0};
	max77654_err_t err;

	if (k_mutex_lock(&pwr_mutex, PWR_MUTEX_TIMEOUT) < 0) {
		LOG_WRN("PMIC IRQ: mutex timeout");
		return;
	}
	/* READING THE INTERRUPT REGISTERS CLEARS THEM AND RELEASES nIRQ. */
	err = max77654_get_int_flags(&pmic_h, &flags);
	k_mutex_unlock(&pwr_mutex);

	if (err != E_MAX77654_SUCCESS) {
		LOG_ERR("PMIC IRQ: reading flags failed");
		return;
	}

	LOG_INF("PMIC IRQ: nEN_f=%d chgin=%d chgin_ctrl=%d chg=%d thm=%d tj_reg=%d sys_ctrl=%d",
		flags.nen_f, flags.chgin, flags.chgin_ctrl, flags.chg, flags.thm, flags.tj_reg,
		flags.sys_ctrl);
	pwr_log_status();
}

static K_WORK_DEFINE(pmic_irq_work, pmic_irq_work_fn);

static void pmic_irq_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);
	k_work_submit(&pmic_irq_work);
}

static int pmic_irq_init(void)
{
	if (!gpio_is_ready_dt(&pmic_irq_gpio)) {
		LOG_ERR("PMIC_IRQ GPIO not ready");
		return -1;
	}
	if (gpio_pin_configure_dt(&pmic_irq_gpio, GPIO_INPUT) < 0) {
		LOG_ERR("PMIC_IRQ GPIO init error");
		return -1;
	}
	gpio_init_callback(&pmic_irq_cb, pmic_irq_isr, BIT(pmic_irq_gpio.pin));
	gpio_add_callback(pmic_irq_gpio.port, &pmic_irq_cb);
	if (gpio_pin_interrupt_configure_dt(&pmic_irq_gpio, GPIO_INT_EDGE_TO_ACTIVE) < 0) {
		LOG_ERR("PMIC_IRQ interrupt config error");
		return -1;
	}
	return 0;
}

// ======== Functions ==============================================================================

int pwr_init() {

  if (!device_is_ready(pwr_i2c)) {
    LOG_ERR("I2C device %s is not ready\n", pwr_i2c->name);
    return -1;
  }

  int err = 0;

  if (k_mutex_lock(&pwr_mutex, PWR_MUTEX_TIMEOUT) < 0) {
    LOG_ERR("PWR Deadlock!");
    return -1;
  }

  /* M8: NO max77654_reset() HERE, SEE THE TOP OF THIS FILE. */

  if (max77654_init(&pmic_h) != E_MAX77654_SUCCESS) {
    LOG_ERR("Driver init failed.");
    err = -1;
    goto cleanup;
  }

  LOG_INF("driver init OK.");

  /* M8: WHY THE PMIC LAST RESET OR SWITCHED OFF (WATCHDOG, BUTTON, UVLO, ...).
   * READING CLEARS THE FLAGS. */
  struct max77654_erc erc = {0};
  if (max77654_get_erc_flags(&pmic_h, &erc) == E_MAX77654_SUCCESS) {
    LOG_INF("PMIC last event: wdt_rst=%d wdt_off=%d sft_crst=%d sft_off=%d mrst=%d "
            "sysuvlo=%d sysovlo=%d tovld=%d",
            erc.wdt_rst, erc.wdt_off, erc.sft_crst_f, erc.sft_off_f, erc.mrst, erc.sysuvlo,
            erc.sysovlo, erc.tovld);
  }

  /* M8: CLEAR OLD INTERRUPT FLAGS BEFORE ARMING, SO nIRQ IS HIGH. */
  struct max77654_int flags;
  (void)max77654_get_int_flags(&pmic_h, &flags);

  if (pmic_irq_init() < 0) {
    err = -1;
    goto cleanup;
  }

  atomic_set(&pwr_is_initialised, true);

cleanup:
  k_mutex_unlock(&pwr_mutex);
  return err;
}

uint32_t pwr_bat_mV() {
  uint32_t mV = 0;

  /* M8: MEASURED HERE, SENSEI READS A VALUE THAT thread_pwr UPDATES. */
  if (!atomic_get(&pwr_is_initialised)) {
    return 0;
  }
  if (k_mutex_lock(&pwr_mutex, PWR_MUTEX_TIMEOUT) < 0) {
    return 0;
  }
  if (max77654_measure(&pmic_h, MAX77654_BATT_V, &mV) != E_MAX77654_SUCCESS) {
    mV = 0;
  }
  k_mutex_unlock(&pwr_mutex);
  return mV;
}

void pwr_log_status(void)
{
	struct max77654_stat stat = {0};
	uint32_t bat_mV = 0, sys_mV = 0;
	max77654_err_t err = E_MAX77654_SUCCESS;

	if (!atomic_get(&pwr_is_initialised)) {
		return;
	}
	if (k_mutex_lock(&pwr_mutex, PWR_MUTEX_TIMEOUT) < 0) {
		return;
	}
	err |= max77654_get_stat(&pmic_h, &stat);
	err |= max77654_measure(&pmic_h, MAX77654_BATT_V, &bat_mV);
	err |= max77654_measure(&pmic_h, MAX77654_VSYS, &sys_mV);
	k_mutex_unlock(&pwr_mutex);

	if (err != E_MAX77654_SUCCESS) {
		LOG_WRN("PMIC status read failed");
		return;
	}

	/* CHG_DTLS: 0 OFF, 1 PREQUAL, 2 CC, 4 CV, 6 TOP-OFF, 8 DONE, 0xA-0xC FAULT.
	 * CHGIN_DTLS: 0 UVLO (NO USB), 3 OK. THM_DTLS: 0 OFF, 5 OK. */
	LOG_INF("PMIC: bat=%u mV sys=%u mV charging=%d chg=%d chgin=%d thm=%d", bat_mV, sys_mV,
		stat.charging_active, stat.charger_status, stat.chgin_status,
		stat.thermistor_status);
}
