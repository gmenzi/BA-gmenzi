// Copyright (c) 2022 Philip Schilk.
// SPDX-License-Identifier: Apache-2.0

#ifndef MAX77654_BASE_H_
#define MAX77654_BASE_H_

/*! \file max77654_base.h
    \brief Main driver header.
*/

#include <stdbool.h>
#include <stdint.h>

// ======== General Definitions ================================================

/**
 * @brief Driver error codes
 */
typedef enum {
  E_MAX77654_SUCCESS = 0,            //!< Success
  E_MAX77654_NULLPTR_ERR = (1 << 0), //!< Nullpointer Error
  E_MAX77654_COM_ERR = (1 << 1),     //!< Communication Error
  E_MAX77654_CONFIG_ERR = (1 << 2),  //!< Configuration Error: Invalid config!
  E_MAX77654_ERR = (1 << 3),         //!< Generic Error
} max77654_err_t;

#define MAX77654_I2C_ADR_7B (0x48U) //!< I2C Address

// ======== Configuration ======================================================

/**
 * @brief An ADC reading to be passed to the driver.
 */
struct max77654_adc_reading {
  uint32_t reading;          //!< ADC output reading.
  uint32_t adc_max_reading;  //!< Maximum ADC reading.
  uint32_t adc_fullscale_mV; //!< ADC fullscale range in millivolt.
};

/**
 * @brief Possible Measurements that can be performed by @ref max77654_measure.
 */
typedef enum {
  MAX77654_CHGIN_V = 0x1U,     //! Measure CHGIN voltage in mV
  MAX77654_CHGIN_I = 0x2U,     //! Measure CHGIN current in mA
  MAX77654_BATT_V = 0x3U,      //! Measure battery voltage in mV
  MAX77654_BATT_I_CHG = 0x4U,  //! Measure battery charge current in percent of fast charge current
  MAX77654_BATT_I_NULL = 0x6U, //! Measure battery discharge current null voltage in mV
  MAX77654_THM = 0x7U,         //! Measure thermistor voltage in mV
  MAX77654_TBIAS = 0x8U,       //! Measure thermistor bias voltage in mV
  MAX77654_AGND = 0x9U,        //! Measure AGND in mV
  MAX77654_VSYS = 0xAU,        //! Measure VSYS in mV
  MAX77654_BATT_I_8MA2,        //! Measure battery discharge current (0 = 0mA, 100 = 8.2mA)
  MAX77654_BATT_I_40MA5,       //! Measure battery discharge current (0 = 0mA, 100 = 40.5mA)
  MAX77654_BATT_I_72MA3,       //! Measure battery discharge current (0 = 0mA, 100 = 72.3mA)
  MAX77654_BATT_I_103MA4,      //! Measure battery discharge current (0 = 0mA, 100 = 103.4mA)
  MAX77654_BATT_I_134MA1,      //! Measure battery discharge current (0 = 0mA, 100 = 134.1mA)
  MAX77654_BATT_I_164MA1,      //! Measure battery discharge current (0 = 0mA, 100 = 164.1mA)
  MAX77654_BATT_I_193MA7,      //! Measure battery discharge current (0 = 0mA, 100 = 193.7mA)
  MAX77654_BATT_I_222MA7,      //! Measure battery discharge current (0 = 0mA, 100 = 222.7mA)
  MAX77654_BATT_I_251MA2,      //! Measure battery discharge current (0 = 0mA, 100 = 251.2mA)
  MAX77654_BATT_I_279MA3,      //! Measure battery discharge current (0 = 0mA, 100 = 279.3mA)
  MAX77654_BATT_I_300MA,       //! Measure battery discharge current (0 = 0mA, 100 = 300mA)
} max77654_measure_t;

/**
 * @brief Operating mode for the internal switchmode converters.
 */
typedef enum {
  MAX77654_SBB_MODE_BUCKBOOST = 0x0U, //! Buck-Boost Mode
  MAX77654_SBB_MODE_BUCK = 0x1U,      //! Buck Mode. More efficient, but requires at least 700mV overhead.
} max77654_sbb_mode_t;

/**
 * @brief Switchmode regulator selection.
 */
typedef enum {
  MAX77654_SBB0 = 0x0U, //! Regulator 0
  MAX77654_SBB1 = 0x1U, //! Regulator 1
  MAX77654_SBB2 = 0x2U, //! Regulator 2
} max77654_sbb_t;

/**
 * @brief GPIO pin selection.
 */
typedef enum {
  MAX77654_GPIO0 = 0x0U, //! Pin 0
  MAX77654_GPIO1 = 0x1U, //! Pin 1
  MAX77654_GPIO2 = 0x2U, //! Pin 2
} max77654_gpio_t;

/**
 * @brief LDO regulator selection.
 */
typedef enum {
  MAX77654_LDO0 = 0x0U, //! Regulator 0
  MAX77654_LDO1 = 0x1U, //! Regulator 1
} max77654_ldo_t;

/**
 * @brief Switchmode converter inductor current limit
 */
typedef enum {
  MAX77654_SBB_PEAK_CURRENT_1A = 0x0U,   //! 1A Inductor peak current limit
  MAX77654_SBB_PEAK_CURRENT_0A75 = 0x1U, //! 0.75A Inductor peak current limit
  MAX77654_SBB_PEAK_CURRENT_0A5 = 0x2U,  //! 0.5A Inductor peak current limit
  MAX77654_SBB_PEAK_CURRENT_0A33 = 0x3U, //! 0.33A Inductor peak current limit
} max77654_sbb_peak_current_t;

/**
 * @brief Regulator Enable control.
 */
typedef enum {
  MAX77654_REG_FPS_SLOT_0 = 0x0U, //! Regulator enabled in slot 0 of the automatic startup
  MAX77654_REG_FPS_SLOT_1 = 0x1U, //! Regulator enabled in slot 1 of the automatic startup
  MAX77654_REG_FPS_SLOT_2 = 0x2U, //! Regulator enabled in slot 2 of the automatic startup
  MAX77654_REG_FPS_SLOT_3 = 0x3U, //! Regulator enabled in slot 3 of the automatic startup
  MAX77654_REG_OFF = 0x4U,        //! Regulator off
  MAX77654_REG_ON = 0x6U,         //! Regulator permanently on
} max77654_en_t;

/**
 * @brief Operating mode for the internal LDO converters.
 */
typedef enum {
  MAX77654_LDO_MODE_LDO = 0x0U, //! Operating as LDO regulator
  MAX77654_LDO_MODE_SW = 0x1U,  //! Operating as power switch
} max77654_ldo_mode_t;

/**
 * @brief Inductor drive speed.
 */
typedef enum {
  MAX77654_SBB_DRIVE_FASTEST = 0x0U, //! Fastest drive
  MAX77654_SBB_DRIVE_FAST = 0x1U,    //! Fast drive
  MAX77654_SBB_DRIVE_SLOW = 0x2U,    //! Slow drive
  MAX77654_SBB_DRIVE_SLOWEST = 0x3U, //! Slowest drive
} max77654_sbb_drive_t;

/**
 * @brief nEN pin internal pull-up selection.
 */
typedef enum {
  MAX77654_nEN_PU_200k = 0x0U, //! 200kOhm
  MAX77654_nEN_PU_10M = 0x1U,  //! 10MOhm
} max77654_nen_pu_t;

/**
 * @brief nEN pin debounce time.
 */
typedef enum {
  MAX77654_nEN_DEBOUNCE_500us = 0x0U, //! 500us
  MAX77654_nEN_DEBOUNCE_30ms = 0x1U,  //! 30ms
} max77654_nen_db_t;

/**
 * @brief nEN pin mode.
 */
typedef enum {
  MAX77654_nEN_PUSH_BUTTON = 0x0U,  //! nEN connected to a momentary switch
  MAX77654_nEN_SLIDE_SWITCH = 0x1U, //! nEN conneted to a slide switch
} max77654_nen_mode_t;

/**
 * @brief nEN Manual reset period.
 */
typedef enum {
  MAX77654_MANUAL_RESET_8s = 0x0U,  //! Manual reset if nEN active for 8 seconds
  MAX77654_MANUAL_RESET_16s = 0x8U, //! Manual reset if nEN active for 16 seconds
} max77654_mrst_t;

/**
 * @brief GPIO Operating Mode.
 */
typedef enum {
  MAX77654_GPIO_OUTPUT = 0x0U, //! Output
  MAX77654_GPIO_INPUT = 0x1U,  //! Input
} max77654_gpio_dir_t;

/**
 * @brief GPIO Output drive Mode.
 */
typedef enum {
  MAX77654_GPIO_OD = 0x0U,   //! Open-drain
  MAX77654_GPIO_PUPU = 0x1U, //! Push-pull
} max77654_gpio_drive_t;

/**
 * @brief Thermistor 'HOT' threshold.
 */
typedef enum {
  MAX77654_THM_HOT_0V411 = 0U, //! 0.411 Volt
  MAX77654_THM_HOT_0V367 = 1U, //! 0.367 Volt
  MAX77654_THM_HOT_0V327 = 2U, //! 0.327 Volt
  MAX77654_THM_HOT_0V291 = 3U, //! 0.291 Volt
} max77654_thm_hot_t;

/**
 * @brief Thermistor 'WARM' threshold.
 */
typedef enum {
  MAX77654_THM_WARM_0V511 = 0U, //! 0.511 Volt
  MAX77654_THM_WARM_0V459 = 1U, //! 0.459 Volt
  MAX77654_THM_WARM_0V411 = 2U, //! 0.411 Volt
  MAX77654_THM_WARM_0V367 = 3U, //! 0.367 Volt
} max77654_thm_warm_t;

/**
 * @brief Thermistor 'COOL' threshold.
 */
typedef enum {
  MAX77654_THM_COOL_0V923 = 0U, //! 0.923 Volt
  MAX77654_THM_COOL_0V867 = 1U, //! 0.867 Volt
  MAX77654_THM_COOL_0V807 = 2U, //! 0.807 Volt
  MAX77654_THM_COOL_0V747 = 3U, //! 0.747 Volt
} max77654_thm_cool_t;

/**
 * @brief Thermistor 'COLD' threshold.
 */
typedef enum {
  MAX77654_THM_COLD_1V024 = 0x0U, //! 1.024 Volt
  MAX77654_THM_COLD_0V976 = 0x1U, //! 0.976 Volt
  MAX77654_THM_COLD_0V923 = 0x2U, //! 0.923 Volt
  MAX77654_THM_COLD_0V867 = 0x3U, //! 0.867 Volt
} max77654_thm_cold_t;

/**
 * @brief Vchgin minimum accepted voltage.
 */
typedef enum {
  MAX77654_VCHGIN_MIN_4V = 0x0U,  //! 4 Volt
  MAX77654_VCHGIN_MIN_4V1 = 0x1U, //! 4.1 Volt
  MAX77654_VCHGIN_MIN_4V2 = 0x2U, //! 4.2 Volt
  MAX77654_VCHGIN_MIN_4V3 = 0x3U, //! 4.3 Volt
  MAX77654_VCHGIN_MIN_4V4 = 0x4U, //! 4.4 Volt
  MAX77654_VCHGIN_MIN_4V5 = 0x5U, //! 4.5 Volt
  MAX77654_VCHGIN_MIN_4V6 = 0x6U, //! 4.6 Volt
  MAX77654_VCHGIN_MIN_4V7 = 0x7U, //! 4.7 Volt
} max77654_vchgin_min_t;

/**
 * @brief Ichgin current limit.
 */
typedef enum {
  MAX77654_ICHGIN_LIM_0A095 = 0x0U, //! 0.095 Amps
  MAX77654_ICHGIN_LIM_0A190 = 0x1U, //! 0.190 Amps
  MAX77654_ICHGIN_LIM_0A285 = 0x2U, //! 0.285 Amps
  MAX77654_ICHGIN_LIM_0A380 = 0x3U, //! 0.380 Amps
  MAX77654_ICHGIN_LIM_0A475 = 0x4U, //! 0.475 Amps
} max77654_ichgin_lim_t;

/**
 * @brief Charge current during battery pre-qualification.
 */
typedef enum {
  MAX77654_I_PQ_10PERCENT = 0x0U, //! 10% of fast-charge CC.
  MAX77654_I_PQ_20PERCENT = 0x1U, //! 20% of fast-charge CC.
} max77654_i_pq_t;

/**
 * @brief Battery voltage at which to transition to normal charging from pre-qualification.
 */
typedef enum {
  MAX77654_CHG_PQ_2V3 = 0x0U, //! 2.3 Volt
  MAX77654_CHG_PQ_2V4 = 0x1U, //! 2.4 Volt
  MAX77654_CHG_PQ_2V5 = 0x2U, //! 2.5 Volt
  MAX77654_CHG_PQ_2V6 = 0x3U, //! 2.6 Volt
  MAX77654_CHG_PQ_2V7 = 0x4U, //! 2.7 Volt
  MAX77654_CHG_PQ_2V8 = 0x5U, //! 2.8 Volt
  MAX77654_CHG_PQ_2V9 = 0x6U, //! 2.9 Volt
  MAX77654_CHG_PQ_3V0 = 0x7U, //! 3.0 Volt
} max77654_chg_pq_t;

/**
 * @brief Percentage of fast-charging current at which to terminate charging.
 */
typedef enum {
  MAX77654_I_TERM_5PERCENT = 0x0U,  //! 5% of fast-charge CC
  MAX77654_I_TERM_7PERCENT = 0x1U,  //! 7% of fast-charge CC
  MAX77654_I_TERM_10PERCENT = 0x2U, //! 10% of fast-charge CC
  MAX77654_I_TERM_15PERCENT = 0x3U, //! 15% of fast-charge CC
} max77654_i_term_t;

/**
 * @brief Top-off charging period.
 */
typedef enum {
  MAX77654_T_TOPOFF_0MIN = 0x0U,  //! 0 minutes
  MAX77654_T_TOPOFF_5MIN = 0x1U,  //! 5 minutes
  MAX77654_T_TOPOFF_10MIN = 0x2U, //! 10 minutes
  MAX77654_T_TOPOFF_15MIN = 0x3U, //! 15 minutes
  MAX77654_T_TOPOFF_20MIN = 0x4U, //! 20 minutes
  MAX77654_T_TOPOFF_25MIN = 0x5U, //! 25 minutes
  MAX77654_T_TOPOFF_30MIN = 0x6U, //! 30 minutes
  MAX77654_T_TOPOFF_35MIN = 0x7U, //! 35 minutes
} max77654_t_topoff_t;

/**
 * @brief Maximum Vsys voltage.
 */
typedef enum {
  MAX77654_VSYS_4V1 = 0x00U,   //! VSYS at 4.1V
  MAX77654_VSYS_4V125 = 0x01U, //! VSYS at 4.125V
  MAX77654_VSYS_4V15 = 0x02U,  //! VSYS at 4.15V
  MAX77654_VSYS_4V175 = 0x03U, //! VSYS at 4.175V
  MAX77654_VSYS_4V2 = 0x04U,   //! VSYS at 4.2V
  MAX77654_VSYS_4V225 = 0x05U, //! VSYS at 4.225V
  MAX77654_VSYS_4V25 = 0x06U,  //! VSYS at 4.25V
  MAX77654_VSYS_4V275 = 0x07U, //! VSYS at 4.275V
  MAX77654_VSYS_4V3 = 0x08U,   //! VSYS at 4.3V
  MAX77654_VSYS_4V325 = 0x09U, //! VSYS at 4.325V
  MAX77654_VSYS_4V35 = 0x0AU,  //! VSYS at 4.35V
  MAX77654_VSYS_4V375 = 0x0BU, //! VSYS at 4.375V
  MAX77654_VSYS_4V4 = 0x0CU,   //! VSYS at 4.4V
  MAX77654_VSYS_4V425 = 0x0DU, //! VSYS at 4.425V
  MAX77654_VSYS_4V45 = 0x0EU,  //! VSYS at 4.45V
  MAX77654_VSYS_4V475 = 0x0FU, //! VSYS at 4.475V
  MAX77654_VSYS_4V5 = 0x10U,   //! VSYS at 4.5V
  MAX77654_VSYS_4V525 = 0x11U, //! VSYS at 4.525V
  MAX77654_VSYS_4V55 = 0x12U,  //! VSYS at 4.55V
  MAX77654_VSYS_4V575 = 0x13U, //! VSYS at 4.575V
  MAX77654_VSYS_4V6 = 0x14U,   //! VSYS at 4.6V
  MAX77654_VSYS_4V625 = 0x15U, //! VSYS at 4.625V
  MAX77654_VSYS_4V65 = 0x16U,  //! VSYS at 4.65V
  MAX77654_VSYS_4V675 = 0x17U, //! VSYS at 4.675V
  MAX77654_VSYS_4V7 = 0x18U,   //! VSYS at 4.7V
  MAX77654_VSYS_4V725 = 0x19U, //! VSYS at 4.725V
  MAX77654_VSYS_4V75 = 0x1AU,  //! VSYS at 4.75V
  MAX77654_VSYS_4V775 = 0x1BU, //! VSYS at 4.775V
  MAX77654_VSYS_4V8 = 0x1CU,   //! VSYS at 4.8V
} max77654_vsys_reg_t;

/**
 * @brief Maximum junction temperature.
 */
typedef enum {
  MAX77654_TJ_REG_60C = 0x0U,  //! 60 degC
  MAX77654_TJ_REG_70C = 0x1U,  //! 70 degC
  MAX77654_TJ_REG_80C = 0x2U,  //! 80 degC
  MAX77654_TJ_REG_90C = 0x3U,  //! 90 degC
  MAX77654_TJ_REG_100C = 0x4U, //! 100 degC
} max77654_tj_reg_t;

/**
 * @brief Charging current in CC mode.
 */
typedef enum {
  MAX77654_CHG_CC_7MA5 = 0x00U,   //! 7.5mA
  MAX77654_CHG_CC_15MA = 0x01U,   //! 15mA
  MAX77654_CHG_CC_22MA5 = 0x02U,  //! 22.5mA
  MAX77654_CHG_CC_30MA = 0x03U,   //! 30mA
  MAX77654_CHG_CC_37MA5 = 0x04U,  //! 37.5mA
  MAX77654_CHG_CC_45MA = 0x05U,   //! 45mA
  MAX77654_CHG_CC_52MA5 = 0x06U,  //! 52.5mA
  MAX77654_CHG_CC_60MA = 0x07U,   //! 60mA
  MAX77654_CHG_CC_67MA5 = 0x08U,  //! 67.5mA
  MAX77654_CHG_CC_75MA = 0x09U,   //! 75mA
  MAX77654_CHG_CC_82MA5 = 0x0AU,  //! 82.5mA
  MAX77654_CHG_CC_90MA = 0x0BU,   //! 90mA
  MAX77654_CHG_CC_97MA5 = 0x0CU,  //! 97.5mA
  MAX77654_CHG_CC_105MA = 0x0DU,  //! 105mA
  MAX77654_CHG_CC_112MA5 = 0x0EU, //! 112.5mA
  MAX77654_CHG_CC_120MA = 0x0FU,  //! 120mA
  MAX77654_CHG_CC_127MA5 = 0x10U, //! 127.5mA
  MAX77654_CHG_CC_135MA = 0x11U,  //! 135mA
  MAX77654_CHG_CC_142MA5 = 0x12U, //! 142.5mA
  MAX77654_CHG_CC_150MA = 0x13U,  //! 150mA
  MAX77654_CHG_CC_157MA5 = 0x14U, //! 157.5mA
  MAX77654_CHG_CC_165MA = 0x15U,  //! 165mA
  MAX77654_CHG_CC_172MA5 = 0x16U, //! 172.5mA
  MAX77654_CHG_CC_180MA = 0x17U,  //! 180mA
  MAX77654_CHG_CC_187MA5 = 0x18U, //! 187.5mA
  MAX77654_CHG_CC_195MA = 0x19U,  //! 195mA
  MAX77654_CHG_CC_202MA5 = 0x1AU, //! 202.5mA
  MAX77654_CHG_CC_210MA = 0x1BU,  //! 210mA
  MAX77654_CHG_CC_217MA5 = 0x1CU, //! 217.5mA
  MAX77654_CHG_CC_225MA = 0x1DU,  //! 225mA
  MAX77654_CHG_CC_232MA5 = 0x1EU, //! 232.5mA
  MAX77654_CHG_CC_240MA = 0x1FU,  //! 240mA
  MAX77654_CHG_CC_247MA5 = 0x20U, //! 247.5mA
  MAX77654_CHG_CC_255MA = 0x21U,  //! 255mA
  MAX77654_CHG_CC_262MA5 = 0x22U, //! 262.5mA
  MAX77654_CHG_CC_270MA = 0x23U,  //! 270mA
  MAX77654_CHG_CC_277MA5 = 0x24U, //! 277.5mA
  MAX77654_CHG_CC_285MA = 0x25U,  //! 285mA
  MAX77654_CHG_CC_292MA5 = 0x26U, //! 292.5mA
  MAX77654_CHG_CC_300MA = 0x27U,  //! 300mA
} max77654_chg_cc_t;

/**
 * @brief Charging voltage in CV mode.
 */
typedef enum {
  MAX77654_CHG_CV_3V6 = 0x00U,   //! 3.6V
  MAX77654_CHG_CV_3V625 = 0x01U, //! 3.625V
  MAX77654_CHG_CV_3V65 = 0x02U,  //! 3.65V
  MAX77654_CHG_CV_3V675 = 0x03U, //! 3.675V
  MAX77654_CHG_CV_3V7 = 0x04U,   //! 3.7V
  MAX77654_CHG_CV_3V725 = 0x05U, //! 3.725V
  MAX77654_CHG_CV_3V75 = 0x06U,  //! 3.75V
  MAX77654_CHG_CV_3V775 = 0x07U, //! 3.775V
  MAX77654_CHG_CV_3V8 = 0x08U,   //! 3.8V
  MAX77654_CHG_CV_3V825 = 0x09U, //! 3.825V
  MAX77654_CHG_CV_3V85 = 0x0AU,  //! 3.85V
  MAX77654_CHG_CV_3V875 = 0x0BU, //! 3.875V
  MAX77654_CHG_CV_3V9 = 0x0CU,   //! 3.9V
  MAX77654_CHG_CV_3V925 = 0x0DU, //! 3.925V
  MAX77654_CHG_CV_3V95 = 0x0EU,  //! 3.95V
  MAX77654_CHG_CV_3V975 = 0x0FU, //! 3.975V
  MAX77654_CHG_CV_4V = 0x10U,    //! 4V
  MAX77654_CHG_CV_4V025 = 0x11U, //! 4.025V
  MAX77654_CHG_CV_4V05 = 0x12U,  //! 4.05V
  MAX77654_CHG_CV_4V075 = 0x13U, //! 4.075V
  MAX77654_CHG_CV_4V1 = 0x14U,   //! 4.1V
  MAX77654_CHG_CV_4V125 = 0x15U, //! 4.125V
  MAX77654_CHG_CV_4V15 = 0x16U,  //! 4.15V
  MAX77654_CHG_CV_4V175 = 0x17U, //! 4.175V
  MAX77654_CHG_CV_4V2 = 0x18U,   //! 4.2V
  MAX77654_CHG_CV_4V225 = 0x19U, //! 4.225V
  MAX77654_CHG_CV_4V25 = 0x1AU,  //! 4.25V
  MAX77654_CHG_CV_4V275 = 0x1BU, //! 4.275V
  MAX77654_CHG_CV_4V3 = 0x1CU,   //! 4.3V
  MAX77654_CHG_CV_4V325 = 0x1DU, //! 4.325V
  MAX77654_CHG_CV_4V35 = 0x1EU,  //! 4.35V
  MAX77654_CHG_CV_4V375 = 0x1FU, //! 4.375V
  MAX77654_CHG_CV_4V4 = 0x20U,   //! 4.4V
  MAX77654_CHG_CV_4V425 = 0x21U, //! 4.425V
  MAX77654_CHG_CV_4V45 = 0x22U,  //! 4.45V
  MAX77654_CHG_CV_4V475 = 0x23U, //! 4.475V
  MAX77654_CHG_CV_4V5 = 0x24U,   //! 4.5V
  MAX77654_CHG_CV_4V525 = 0x25U, //! 4.525V
  MAX77654_CHG_CV_4V55 = 0x26U,  //! 4.55V
  MAX77654_CHG_CV_4V575 = 0x27U, //! 4.575V
  MAX77654_CHG_CV_4V6 = 0x28U,   //! 4.6V
} max77654_chg_cv_t;

/**
 * @brief Fast-charging safety timer.
 */
typedef enum {
  MAX77654_T_FAST_CHG_OFF = 0x0U, //! Off
  MAX77654_T_FAST_CHG_3H = 0x1U,  //! 3h
  MAX77654_T_FAST_CHG_5H = 0x2U,  //! 5h
  MAX77654_T_FAST_CHG_7H = 0x3U,  //! 7h
} max77654_t_fast_chg_t;

/**
 * @brief Thermistor status.
 */
typedef enum {
  MAX77654_THM_DTLS_OFF = 0x0U,  //! Thermistor off (disabled or no chgin)
  MAX77654_THM_DTLS_COLD = 0x1U, //! Thermistor cold
  MAX77654_THM_DTLS_COOL = 0x2U, //! Thermistor cool
  MAX77654_THM_DTLS_WARM = 0x3U, //! Thermistor warm
  MAX77654_THM_DTLS_HOT = 0x4U,  //! Thermistor hot
  MAX77654_THM_DTLS_OK = 0x5U,   //! Thermistor ok
} max77654_thm_dtls_t;

/**
 * @brief Charger status.
 */
typedef enum {
  MAX77654_CHG_DTLS_OFF = 0x0U,                    //! Not charging
  MAX77654_CHG_DTLS_PREQUAL = 0x1U,                //! In pre-qualification
  MAX77654_CHG_DTLS_CC = 0x2U,                     //! Constant-current charging
  MAX77654_CHG_DTLS_CC_JEITA = 0x3U,               //! Constant-current charging in JEITA temperature mode
  MAX77654_CHG_DTLS_CV = 0x4U,                     //! Constant-voltage charging
  MAX77654_CHG_DTLS_CV_JEITA = 0x5U,               //! Constant-voltage charging in JEITA temperature mode
  MAX77654_CHG_DTLS_TOP_OFF = 0x6U,                //! Top-off charging mode
  MAX77654_CHG_DTLS_TOP_OFF_JEITA = 0x7U,          //! Top-off charging in JETIA temperature mode
  MAX77654_CHG_DTLS_DONE = 0x8U,                   //! Charging done
  MAX77654_CHG_DTLS_DONE_JEITA = 0x9U,             //! Charging done in JEITA temperature mode
  MAX77654_CHG_DTLS_PREQUAL_TIMER_FAULT = 0xAU,    //! Pre-qualification timer fault
  MAX77654_CHG_DTLS_FASTCHARGE_TIMER_FAULT = 0xBU, //! Fast-charge timer fault
  MAX77654_CHG_DTLS_BAT_TEMPERATURE_FAULT = 0xCU,  //! Battery temperature fault
} max77654_chg_dtls_t;

/**
 * @brief Charge-in stats.
 */
typedef enum {
  MAX77654_CHGIN_DTLS_UVLO = 0x0U,     //! Under-voltage
  MAX77654_CHGIN_DTLS_OVP = 0x1U,      //! Over-voltage
  MAX77654_CHGIN_DTLS_DEBOUNCE = 0x2U, //! De-bouncing
  MAX77654_CHGIN_DTLS_OK = 0x3U,       //! OK
} max77654_chgin_dtls_t;

/**
 * @brief Watchdog timer period.
 */
typedef enum {
  MAX77654_WDT_16s = 0x0,  //! 16 seconds
  MAX77654_WDT_32s = 0x1,  //! 32 seconds
  MAX77654_WDT_64s = 0x2,  //! 64 seconds
  MAX77654_WDT_128s = 0x3, //! 128 seconds
} max77654_wdt_period_t;

/**
 * @brief Action on watchdog timer overflow
 */
typedef enum {
  MAX77654_WDT_DO_PWR_OFF = 0x0U,   //! Power-off
  MAX77654_WDT_DO_PWR_RESET = 0x1U, //! Power-reset
} max77654_wdt_action_t;

/**
 * @brief Switchmode converter configuration.
 */
struct max77654_sbb_conf {
  max77654_sbb_mode_t mode;                 //! Regulator mode (Buck-Boost/Buck)
  max77654_sbb_peak_current_t peak_current; //! Inductor peak current limit
  bool active_discharge;                    //! Active discharge enable
  max77654_en_t en;                         //! Regulator enable
  uint32_t output_voltage_mV;               //! Output voltage in millivolt (800mV-5500mV, in 50mV steps)
};

/**
 * @brief LDO converter configuration.
 */
struct max77654_ldo_conf {
  max77654_ldo_mode_t mode;   //! Regulator mode (LDO/Switch)
  bool active_discharge;      //! Active discharge enable
  max77654_en_t en;           //! Regulator enable
  uint32_t output_voltage_mV; //! Output voltage in millivolt (800mV-3975mV, in 25mV steps)
};

/**
 * @brief GPIO Pin Configuration.
 */
struct max77654_gpio_conf {
  bool alternate_mode;                     //! Use pin alternate-function, not I/O
  max77654_gpio_dir_t direction;           //! Input/output mode selection
  bool input_debounce_enable;              // Debouncing (in input mode)
  max77654_gpio_drive_t output_drive_type; // Drive type (in output mode)
  bool output_value;                       // Output value (in output mode)
};

/**
 * @brief MAX77654 interrupts.
 */
struct max77654_int {
  bool dod0_r : 1;     //! LDO0 experienced dropout.
  bool dod1_r : 1;     //! LDO1 experienced dropout.
  bool ldo0_f : 1;     //! LDO0 experienced fault.
  bool ldo1_f : 1;     //! LDO1 experienced fault.
  bool tjal1_r : 1;    //! Thermal alarm 1.
  bool tjal2_r : 1;    //! Thermal alarm 2.
  bool nen_r : 1;      //! nEN rising edge.
  bool nen_f : 1;      //! nEN falling edge.
  bool gpi0_r : 1;     //! GPI0 rising edge.
  bool gpi0_f : 1;     //! GPI0 falling edge.
  bool gpi1_r : 1;     //! GPI1 rising edge.
  bool gpi1_f : 1;     //! GPI1 falling edge.
  bool gpi2_r : 1;     //! GPI2 rising edge.
  bool gpi2_f : 1;     //! GPI2 falling edge.
  bool sbb_to : 1;     //! SBB Timeout.
  bool sys_cnfg : 1;   //! System voltage configuration error interrupt.
  bool sys_ctrl : 1;   //! Vsys control error interrupt.
  bool chgin_ctrl : 1; //! CHGIN under-voltage/over-current interrupt.
  bool tj_reg : 1;     //! Maximum junction temp. interrupt.
  bool chgin : 1;      //! CHGIN status change interrupt.
  bool chg : 1;        //! CHG status change interrupt.
  bool thm : 1;        //! Thermistor status change interrupt.
};

/**
 * @brief Fault/Error flags.
 */
struct max77654_erc {
  bool wdt_rst : 1;    //! Reset caused by watchdog timeout.
  bool wdt_off : 1;    //! Power-off caused by watchdog timeout.
  bool sft_crst_f : 1; //! Reset due to software.
  bool sft_off_f : 1;  //! Power-off due to software.
  bool mrst : 1;       //! Reset due to manual reset timer.
  bool sysuvlo : 1;    //! System voltage under-voltage-lockout.
  bool sysovlo : 1;    //! System voltage over-voltage-lockout.
  bool tovld : 1;      //! System thermal overload.
};

/**
 * @brief Status flags.
 */
struct max77654_stat {
  bool main_bias_ok : 1;                 //! Main bias is OK
  bool ldo0_in_dropout : 1;              //! LDO0 is in drop-out
  bool ldo1_in_dropout : 1;              //! LDO1 is in drop-out
  bool thermal_alarm_1 : 1;              //! Thermal alarm 1
  bool thermal_alarm_2 : 1;              //! Thermal alarm 2
  bool nEN_active : 1;                   //! nEN pin active
  bool nIRQ_active : 1;                  //! nIRQ pin active
  bool min_vchgin_regulation : 1;        //! Minimum input voltage regulation
  bool max_ichgin_regulation : 1;        //! Maximum input current regulation
  bool min_vsys_regulation : 1;          //! Minimim vsys voltage regulation
  bool max_junction_temp_regulation : 1; //! Maximum junction temperature regulation
  max77654_thm_dtls_t thermistor_status; //! Thermistor status
  bool charging_active : 1;              //! Currently charging
  max77654_chg_dtls_t charger_status;    //! Charger status
  max77654_chgin_dtls_t chgin_status;    //! Charger input status
  bool charger_timer_suspended : 1;      //! Charger's active timer suspended
};

/**
 * @brief Driver configuration.
 * Collection of all MAX77654 configuration options.
 * These configurations are applied by calling @ref max77654_init or @ref max77654_config,
 * or can be partially applied/updated by calling the relevant max77654_config_* function.
 */
struct max77654_conf {
  uint8_t cid; //! Device ID. Variant/model dependent. Required - @ref max77654_init will fail if not set correctly

  // === Global configuration ===
  bool main_bias_force_enable;   //! Force-enable the main bias.
  bool main_bias_low_power_mode; //! Put main bias into low-power mode.

  max77654_vsys_reg_t vsys_regulation; //! Maximum/regulation voltage for VSYS.

  max77654_sbb_drive_t sbb_drive_speed; //! SBB Drive speed.

  max77654_nen_mode_t nEN_mode;        //! nEN pin mode (momentary/slide switch).
  max77654_nen_pu_t nEN_pu_strength;   //! nEN internal pull-up strength.
  max77654_nen_db_t nEN_debounce_time; //! nEN debounce time
  max77654_mrst_t manual_reset_period; //! Period nEN has to be active for a manual reset.

  // === Watchdog timer configuration ===
  bool wdt_enable;                         //! Watchdog timer enable
  max77654_wdt_period_t wdt_period;        //! Watchdog timer period
  max77654_wdt_action_t wdt_action_on_ovf; //! Action performed when Watchdog timer overflows.

  // === Charger configuration ===
  bool charger_enabled; //! Enable the charger.
  bool USB_suspend;     //! Place the charger into USB suspend mode.

  max77654_vchgin_min_t chgin_v_min; //! Minimum CHGIN regulation voltage.
  max77654_ichgin_lim_t chgin_i_lim; //! CHGIN IN current limit.
  max77654_i_term_t termnation_i;    //! Termination current.
  max77654_t_topoff_t topoff_t;      //! Topoff timer period.

  max77654_chg_cc_t fast_chg_cc;       //! Fast-charge constant current setting.
  max77654_chg_cc_t fast_chg_cc_jeita; //! JEITA fast-charge constant current setting.
  max77654_chg_cv_t fast_chg_cv;       //! Fast-charge battery voltage regulation.
  max77654_chg_cv_t fast_chg_cv_jeita; //! JEITA fast-charge battery voltage regulation.
  max77654_t_fast_chg_t t_fast_chg;    //! Fast-charge safety timer.

  max77654_i_pq_t preq_i;      //! Prequalifaction charge current, as a percent of the fast-charging current
  max77654_chg_pq_t preq_v_th; //! Prequalifaction voltage threshold

  max77654_tj_reg_t j_temp_reg; //! Junction regulation termperature (Thermistor is powered by charger).
  bool thm_en;                  //! Enable the thermistor
  max77654_thm_hot_t th_hot;    //! Thermister threshold for 'hot'.
  max77654_thm_warm_t th_warm;  //! Thermister threshold for 'warm'.
  max77654_thm_cool_t th_cool;  //! Thermister threshold for 'cool'.
  max77654_thm_cold_t th_cold;  //! Thermister threshold for 'cold'.

  // === SBB configuration ===
  struct max77654_sbb_conf sbb_conf[3]; //! Configuration for regulators SBB0-2

  // === LDO configuration ===
  struct max77654_ldo_conf ldo_conf[2]; //! Configuration for regulators LDO0-1

  // === Interrupt configuration ===
  struct max77654_int int_en; //! Interrupt enable.

  // === GPIO configuration ===
  struct max77654_gpio_conf gpio_conf[3]; //! Configuration for GPIO pins 0-2
};

// ======== Handle Definition ==================================================

/**
 * @brief Device Handle struct.
 * Represents a single physical device. Holds the hardware-specific interface functions,
 * and device configuration.
 */
struct max77654_h {
  // === Interface function pointers. Required. ===
  /**
   * @brief Pointer to the device-specific read-register functions
   * @warning Required!
   *
   * @param reg_adr Register address at which to start reading.
   * @param n Number of registers to read.
   * @param buf Buffer to store results.
   * @return @ref E_MAX77654_SUCCESS if read was successful, otherwise @ref E_MAX77654_COM_ERR.
   */
  max77654_err_t (*read_regs)(uint8_t reg_adr, uint32_t n, uint8_t *buf);

  /**
   * @brief Pointer to the device-specific write-register functions
   * @warning Required!
   *
   * @param reg_adr Register address at which to start writing.
   * @param n Number of registers to write.
   * @param buf Buffer of data to hold.
   * @return @ref E_MAX77654_SUCCESS if write was successful, otherwise @ref E_MAX77654_COM_ERR.
   */
  max77654_err_t (*write_regs)(uint8_t reg_adr, uint32_t n, const uint8_t *buf);

  // === Interface function pointers. Optional. ===
  /**
   * @brief Pointer to the device-specific read-adc function measuring the AMUX pin output.
   * @note Optional. Only required for @ref max77654_measure.
   *
   * @param reading buffer to store the reading result in.
   * @return @ref E_MAX77654_SUCCESS if write was successful, otherwise @ref E_MAX77654_COM_ERR.
   */
  max77654_err_t (*adc_read)(struct max77654_adc_reading *reading);

  /**
   * @brief Pointer to logging function.
   * Called by the driver to log status and error messages, with an optional integer
   * variable to log. Note that the string does not contain any formatting specifiers,
   * and should be logged as follows (if has_int_arg is true):
   *
   * printf("%s: %s %i", is_err ? "ERR" : "LOG", msg, arg);
   *
   * @param msg the log message
   * @param is_err indicates if this is an error message
   * @param has_int_arg indicates if this message is accompanied by an integer variable to log.
   * @param arg the integer variable to log if has_int_arg is true.
   */
  void (*log)(char *msg, bool is_err, bool has_int_arg, uint32_t arg);

  // === Configuration. Required. ===
  /**
   * @brief Device configuration.
   * These configurations are applied by calling @ref max77654_init or @ref max77654_config,
   * or can be partially applied/updated by calling the relevant max77654_config_* function.
   */
  struct max77654_conf conf;
};

// ======== Function Prototypes ================================================

/**
 * @brief Reset the device.
 *
 * @param h Pointer to the device-specific handle struct.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_reset(const struct max77654_h *h);

/**
 * @brief Power-off the device.
 *
 * @param h Pointer to the device-specific handle struct.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_power_off(const struct max77654_h *h);

/**
 * @brief Place the device into factory ship mode.
 *
 * @param h Pointer to the device-specific handle struct.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_factory_ship_mode(const struct max77654_h *h);

/**
 * @brief Initialize the device.
 * Validates the driver handle, Tests the connection by reading and validating the CID value,
 * and applies the complete device configuration within the handle to the device.
 *
 * @note This function should be called before any other driver functions are called.
 * @note A misconfigured 'cid' value in the configuration struct will cause this function to fail.
 *
 * @param h Pointer to the device-specific handle struct.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_init(const struct max77654_h *h);

/**
 * @brief Configures the device.
 * Applies the complete configuration within the handle struct to the device, just as @ref max77654_init does,
 * but skips some of the validation and checks. Useful for re-configuring the device after the
 * first configuration.
 *
 * @param h Pointer to the device-specific handle struct.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_config(const struct max77654_h *h);

/**
 * @brief Configures the global resources.
 * Applies the global resources configurations from within the handle struct to the device.
 *
 * @param h Pointer to the device-specific handle struct.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_config_global(const struct max77654_h *h);

/**
 * @brief Configures the charger.
 * Applies the charger configurations from within the handle struct to the device.
 *
 * @param h Pointer to the device-specific handle struct.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_config_charger(const struct max77654_h *h);

/**
 * @brief Configures the watchdog timer.
 * Applies the watchdog timer configurations from within the handle struct to the device.
 *
 * @param h Pointer to the device-specific handle struct.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_config_wdt(const struct max77654_h *h);

/**
 * @brief Configures an individual switchmode converter.
 * Applies the configuration for a single switchmode convert from within the handle struct to the device.
 *
 * @param h Pointer to the device-specific handle struct.
 * @param sbb Regulator to configure.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_config_sbb(const struct max77654_h *h, max77654_sbb_t sbb);

/**
 * @brief Configures an individual LDO converter.
 * Applies the configuration for a single LDO convert from within the handle struct to the device.
 *
 * @param h Pointer to the device-specific handle struct.
 * @param ldo Regulator to configure.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_config_ldo(const struct max77654_h *h, max77654_ldo_t ldo);

/**
 * @brief Configures the device interrupts.
 * Applies the interrupt configuration from within the handle struct to the device.
 *
 * @param h Pointer to the device-specific handle struct.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_config_int(const struct max77654_h *h);

/**
 * @brief Configures an individual LDO converter.
 * Applies the configuration for a single LDO convert from within the handle struct to the device.
 *
 * @param h Pointer to the device-specific handle struct.
 * @param gpio Pin to configure.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_config_gpio(const struct max77654_h *h, max77654_gpio_t gpio);

/**
 * @brief Retrieves the current state of the interrupt flags from the device.
 *
 * @param h Pointer to the device-specific handle struct.
 * @param flags Buffer to store current flags in.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_get_int_flags(const struct max77654_h *h, struct max77654_int *flags);

/**
 * @brief Retrieves the current state of the error/fault flags from the device.
 *
 * @param h Pointer to the device-specific handle struct.
 * @param flags Buffer to store current flags in.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_get_erc_flags(const struct max77654_h *h, struct max77654_erc *flags);

/**
 * @brief Retrieves the current device state.
 *
 * @param h Pointer to the device-specific handle struct.
 * @param stat Buffer to store current status in.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_get_stat(const struct max77654_h *h, struct max77654_stat *stat);

/**
 * @brief Reads the current value of a GPIO pin.
 * @note Only valid if pin is configured as input.
 *
 * @param h Pointer to the device-specific handle struct.
 * @param gpio Pin to read.
 * @param state Buffer to store current state in.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_read_gpio(const struct max77654_h *h, max77654_gpio_t gpio, bool *state);

/**
 * @brief Measure an analog parameter from the device.
 * For a list of possible measurements, see @ref max77654_measure_t.
 * @note Can only be used if the 'adc_read' function is provided in the handle.
 *
 * @param h Pointer to the device-specific handle struct.
 * @param m Measurement to perform.
 * @param result Buffer to store result in.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_measure(const struct max77654_h *h, max77654_measure_t m, uint32_t *result);

/**
 * @brief Clear the watchdog timer
 *
 * @param h Pointer to the device-specific handle struct.
 * @return @ref E_MAX77654_SUCCESS if successful, otherwise an error code from @ref max77654_err_t.
 */
max77654_err_t max77654_clr_wdt(const struct max77654_h *h);

#endif /* MAX77654_BASE_H_ */
