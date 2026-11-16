/*
 * ----------------------------------------------------------------------
 *
 * File: pmic_bsp.h
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

#ifndef PMIC_BSP_H_
#define PMIC_BSP_H_

#include "max77654.h"

/**
 * @brief Hardware-specific read-register function as required by the MAX77654 driver.
 * see max77654.h
 */
max77654_err_t pmic_bsp_read_regs(uint8_t reg_adr, uint32_t n, uint8_t *buf);

/**
 * @brief Hardware-specific write-register function as required by the MAX77654 driver.
 * see max77654.h
 */
max77654_err_t pmic_bsp_write_regs(uint8_t reg_adr, uint32_t n, const uint8_t *buf);

/**
 * @brief Hardware-specific log function as required by the MAX77654 driver.
 * see max77654.h
 */
void pmic_bsp_log(char *msg, bool is_err, bool has_int_arg, uint32_t arg);

/**
 * @brief Hardware-specific adc-read function as required by the MAX77654 driver.
 * see max77654.h
 */
max77654_err_t pmic_bsp_adc_read(struct max77654_adc_reading *reading);

/**
 * @brief Reset I2C port hotfix
 * I2CB switch causes a glitch which confuses the PMIC. This performs
 * a dummy read (which errors, but can be ignored) to reset the PMIC's I2C
 * interface.
 */
void pmic_i2c_reset_hotfix();

#endif /* PMIC_BSP_H_ */
