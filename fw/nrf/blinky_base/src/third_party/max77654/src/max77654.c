// Copyright (c) 2022 Philip Schilk.
// SPDX-License-Identifier: Apache-2.0

#include <stdint.h>
#include <string.h>

#include "max77654.h"
#include "max77654_reg.h"
#include "max77654_utils.h"
// ======== Macro Quick Access =================================================================

#define REG(_reg_) MAX77654_REG(_reg_)
#define REG_MASK(_reg_, _field_) MAX77654_REG_MASK(_reg_, _field_)
#define REG_CONST(_reg_, _field_, _const_) MAX77654_REG_CONST(_reg_, _field_, _const_)

#define REG_FIELD_GET(_reg_, _field_, _v_) MAX77654_REG_FIELD_GET(_reg_, _field_, _v_)
#define REG_FIELD_CLR(_reg_, _field_, _v_reg_) MAX77654_REG_FIELD_CLR(_reg_, _field_, _v_reg_)
#define REG_FIELD_SET(_reg_, _field_, _v_reg_, _v_field_) MAX77654_REG_FIELD_SET(_reg_, _field_, _v_reg_, _v_field_)
#define REG_FIELD_SET_CONST(_reg_, _field_, _v_reg_, _const_)                                                          \
  MAX77654_REG_FIELD_SET_CONST(_reg_, _field_, _v_reg_, _const_)

#define DRVR_LOG(msg)                                                                                                  \
  if (h != 0 && h->log != 0) {                                                                                         \
    h->log(msg, false, false, 0);                                                                                      \
  }

#define DRVR_LOG_VAR(msg, val)                                                                                         \
  if (h != 0 && h->log != 0) {                                                                                         \
    h->log(msg, false, true, val);                                                                                     \
  }

#define DRVR_ERR(msg)                                                                                                  \
  if (h != 0 && h->log != 0) {                                                                                         \
    h->log(msg, true, false, 0);                                                                                       \
  }

#define DRVR_ERR_VAR(msg, val)                                                                                         \
  if (h != 0 && h->log != 0) {                                                                                         \
    h->log(msg, true, true, val);                                                                                      \
  }

// ======== Private Prototypes =================================================================

static max77654_err_t check_connection(const struct max77654_h *h);
static max77654_err_t validate_handle(const struct max77654_h *h);
static max77654_err_t encode_mV(const struct max77654_h *h, uint32_t mV, bool is_ldo, uint8_t *code);

static max77654_err_t basic_measurement_e3(const struct max77654_h *h, max77654_measure_t m, uint64_t gain_num,
                                           uint64_t gain_denum, uint32_t *result);
static max77654_err_t bat_i_measurement(const struct max77654_h *h, uint8_t scale, uint32_t *result);

// ======== Public Functions =================================================================

max77654_err_t max77654_reset(const struct max77654_h *h) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  // CNFG_GLBL:
  uint8_t cnfg_glbl = 0;
  cnfg_glbl = REG_FIELD_SET_CONST(CNFG_GLBL, SFT_CTRL, cnfg_glbl, SFT_CRST);
  err |= h->write_regs(REG(CNFG_GLBL), 1, &cnfg_glbl);

  return err;
}

max77654_err_t max77654_power_off(const struct max77654_h *h) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  // CNFG_GLBL:
  uint8_t cnfg_glbl = 0;
  cnfg_glbl = REG_FIELD_SET_CONST(CNFG_GLBL, SFT_CTRL, cnfg_glbl, SFT_OFF);
  err |= h->write_regs(REG(CNFG_GLBL), 1, &cnfg_glbl);

  return err;
}

max77654_err_t max77654_factory_ship_mode(const struct max77654_h *h) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  // CNFG_GLBL:
  uint8_t cnfg_glbl = 0;
  cnfg_glbl = REG_FIELD_SET_CONST(CNFG_GLBL, SFT_CTRL, cnfg_glbl, FSM);
  err |= h->write_regs(REG(CNFG_GLBL), 1, &cnfg_glbl);

  return err;
}

max77654_err_t max77654_init(const struct max77654_h *h) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  err |= check_connection(h);
  if (err)
    return err;

  err |= max77654_config(h);
  if (err)
    return err;

  return err;
}

max77654_err_t max77654_config(const struct max77654_h *h) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  err |= max77654_config_global(h);
  if (err)
    return err;

  err |= max77654_config_charger(h);
  if (err)
    return err;

  for (uint32_t i = 0; i < 3; i++) {
    err |= max77654_config_sbb(h, i);
    if (err)
      return err;
  }

  for (uint32_t i = 0; i < 2; i++) {
    err |= max77654_config_ldo(h, i);
    if (err)
      return err;
  }

  err |= max77654_config_int(h);
  if (err)
    return err;

  for (uint32_t i = 0; i < 3; i++) {
    err |= max77654_config_gpio(h, i);
    if (err)
      return err;
  }

  if (err)
    return err;

  return err;
}

max77654_err_t max77654_config_global(const struct max77654_h *h) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  // CNFG_GLBL:
  uint8_t cnfg_glbl = 0;
  cnfg_glbl = REG_FIELD_SET(CNFG_GLBL, DBEN_nEN, cnfg_glbl, h->conf.nEN_debounce_time);
  cnfg_glbl = REG_FIELD_SET(CNFG_GLBL, nEN_MODE, cnfg_glbl, h->conf.nEN_mode);
  cnfg_glbl = REG_FIELD_SET(CNFG_GLBL, PU_DIS, cnfg_glbl, h->conf.nEN_pu_strength);
  cnfg_glbl = REG_FIELD_SET(CNFG_GLBL, SBIA_EN, cnfg_glbl, h->conf.main_bias_force_enable);
  cnfg_glbl = REG_FIELD_SET(CNFG_GLBL, SBIA_LPM, cnfg_glbl, h->conf.main_bias_low_power_mode);
  cnfg_glbl = REG_FIELD_SET(CNFG_GLBL, T_MRST, cnfg_glbl, h->conf.manual_reset_period);
  cnfg_glbl = REG_FIELD_SET_CONST(CNFG_GLBL, SFT_CTRL, cnfg_glbl, NO_ACTION);
  err |= h->write_regs(REG(CNFG_GLBL), 1, &cnfg_glbl);
  if (err)
    return err;

  // CNFG_SBB_TOP:
  uint8_t cnfg_sbb_top = 0;
  cnfg_sbb_top = REG_FIELD_SET(CNFG_SBB_TOP, DRV_SBB, cnfg_sbb_top, h->conf.sbb_drive_speed);
  err |= h->write_regs(REG(CNFG_SBB_TOP), 1, &cnfg_sbb_top);
  if (err)
    return err;

  return err;
}

max77654_err_t max77654_config_charger(const struct max77654_h *h) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  // CNFG_CHG_A:
  uint8_t cnfg_chg_a = 0;
  cnfg_chg_a = REG_FIELD_SET(CNFG_CHG_A, THM_HOT, cnfg_chg_a, h->conf.th_hot);
  cnfg_chg_a = REG_FIELD_SET(CNFG_CHG_A, THM_WARM, cnfg_chg_a, h->conf.th_warm);
  cnfg_chg_a = REG_FIELD_SET(CNFG_CHG_A, THM_COOL, cnfg_chg_a, h->conf.th_cold);
  cnfg_chg_a = REG_FIELD_SET(CNFG_CHG_A, THM_COLD, cnfg_chg_a, h->conf.th_cool);
  err |= h->write_regs(REG(CNFG_CHG_A), 1, &cnfg_chg_a);
  if (err)
    return err;

  // CNFG_CHG_B:
  uint8_t cnfg_chg_b = 0;
  cnfg_chg_b = REG_FIELD_SET(CNFG_CHG_B, CHG_EN, cnfg_chg_b, h->conf.charger_enabled);
  cnfg_chg_b = REG_FIELD_SET(CNFG_CHG_B, I_PQ, cnfg_chg_b, h->conf.preq_i);
  cnfg_chg_b = REG_FIELD_SET(CNFG_CHG_B, ICHGIN_LIM, cnfg_chg_b, h->conf.chgin_i_lim);
  cnfg_chg_b = REG_FIELD_SET(CNFG_CHG_B, VCHGIN_MIN, cnfg_chg_b, h->conf.chgin_v_min);
  err |= h->write_regs(REG(CNFG_CHG_B), 1, &cnfg_chg_b);
  if (err)
    return err;

  // CNFG_CHG_C:
  uint8_t cnfg_chg_c = 0;
  cnfg_chg_c = REG_FIELD_SET(CNFG_CHG_C, T_TOPOFF, cnfg_chg_c, h->conf.topoff_t);
  cnfg_chg_c = REG_FIELD_SET(CNFG_CHG_C, I_TERM, cnfg_chg_c, h->conf.termnation_i);
  cnfg_chg_c = REG_FIELD_SET(CNFG_CHG_C, CHG_PQ, cnfg_chg_c, h->conf.preq_v_th);
  err |= h->write_regs(REG(CNFG_CHG_C), 1, &cnfg_chg_c);
  if (err)
    return err;

  // CNFG_CHG_D:
  uint8_t cnfg_chg_d = 0;
  cnfg_chg_d = REG_FIELD_SET(CNFG_CHG_D, TJ_REG, cnfg_chg_d, h->conf.j_temp_reg);
  cnfg_chg_d = REG_FIELD_SET(CNFG_CHG_D, VSYS_REG, cnfg_chg_d, h->conf.vsys_regulation);
  err |= h->write_regs(REG(CNFG_CHG_D), 1, &cnfg_chg_d);
  if (err)
    return err;

  // CNFG_CHG_E:
  uint8_t cnfg_chg_e = 0;
  cnfg_chg_e = REG_FIELD_SET(CNFG_CHG_E, CHG_CC, cnfg_chg_e, h->conf.fast_chg_cc);
  cnfg_chg_e = REG_FIELD_SET(CNFG_CHG_E, T_FAST_CHG, cnfg_chg_e, h->conf.t_fast_chg);
  err |= h->write_regs(REG(CNFG_CHG_E), 1, &cnfg_chg_e);
  if (err)
    return err;

  // CNFG_CHG_F:
  uint8_t cnfg_chg_f = 0;
  cnfg_chg_f = REG_FIELD_SET(CNFG_CHG_F, CHG_CC_JEITA, cnfg_chg_f, h->conf.fast_chg_cc_jeita);
  cnfg_chg_f = REG_FIELD_SET(CNFG_CHG_F, THM_EN, cnfg_chg_f, h->conf.thm_en);
  err |= h->write_regs(REG(CNFG_CHG_F), 1, &cnfg_chg_f);
  if (err)
    return err;

  // CNFG_CHG_G:
  uint8_t cnfg_chg_g = 0;
  cnfg_chg_g = REG_FIELD_SET(CNFG_CHG_G, CHG_CV, cnfg_chg_g, h->conf.fast_chg_cv);
  cnfg_chg_g = REG_FIELD_SET(CNFG_CHG_G, USBS, cnfg_chg_g, h->conf.USB_suspend);
  err |= h->write_regs(REG(CNFG_CHG_G), 1, &cnfg_chg_g);
  if (err)
    return err;

  // CNFG_CHG_H:
  uint8_t cnfg_chg_h = 0;
  cnfg_chg_h = REG_FIELD_SET(CNFG_CHG_H, CHG_CV_JEITA, cnfg_chg_h, h->conf.fast_chg_cv_jeita);
  err |= h->write_regs(REG(CNFG_CHG_H), 1, &cnfg_chg_h);
  if (err)
    return err;

  // CNFG_CHG_I:
  uint8_t cnfg_chg_i = 0;
  cnfg_chg_i = REG_FIELD_SET_CONST(CNFG_CHG_I, MUX_SEL, cnfg_chg_i, DISABLED);
  err |= h->write_regs(REG(CNFG_CHG_I), 1, &cnfg_chg_i);

  return err;
}

max77654_err_t max77654_config_wdt(const struct max77654_h *h) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  // CNFG_WDT:
  uint8_t cnfg_wdt = 0;
  cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_PER, cnfg_wdt, h->conf.wdt_period);
  cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_MODE, cnfg_wdt, h->conf.wdt_action_on_ovf);
  cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_CLR, cnfg_wdt, 1);
  cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_EN, cnfg_wdt, h->conf.wdt_enable);
  cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_LOCK, cnfg_wdt, 1);

  if (h->conf.wdt_enable) {
    // Lock does not prevent enable
    err |= h->write_regs(REG(CNFG_WDT), 1, &cnfg_wdt);
  } else {
    // Lock prevents disabeling. First, disable lock:
    cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_LOCK, cnfg_wdt, 0);
    err |= h->write_regs(REG(CNFG_WDT), 1, &cnfg_wdt);
    // Disable the WDT:
    err |= h->write_regs(REG(CNFG_WDT), 1, &cnfg_wdt);
    // Re-enable the lock:
    cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_LOCK, cnfg_wdt, 1);
    err |= h->write_regs(REG(CNFG_WDT), 1, &cnfg_wdt);
  }

  return err;
}

max77654_err_t max77654_config_sbb(const struct max77654_h *h, max77654_sbb_t sbb) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  // Select the right registers, based on the SBB selected:
  uint8_t adr_cnfg_a;
  uint8_t adr_cnfg_b;

  switch (sbb) {
  case MAX77654_SBB0:
    adr_cnfg_a = REG(CNFG_SBB0_A);
    adr_cnfg_b = REG(CNFG_SBB0_B);
    break;
  case MAX77654_SBB1:
    adr_cnfg_a = REG(CNFG_SBB1_A);
    adr_cnfg_b = REG(CNFG_SBB1_B);
    break;
  case MAX77654_SBB2:
    adr_cnfg_a = REG(CNFG_SBB2_A);
    adr_cnfg_b = REG(CNFG_SBB2_B);
    break;
  default:
    err |= E_MAX77654_CONFIG_ERR;
    return err;
  }

  const struct max77654_sbb_conf *sbb_conf = &(h->conf.sbb_conf[sbb]);

  // Convert voltage to code if sbb enabled:
  uint8_t tv_code;
  if (sbb_conf->en != MAX77654_REG_OFF) {
    err |= encode_mV(h, sbb_conf->output_voltage_mV, false, &tv_code);
    if (err)
      return err;
  } else {
    tv_code = 0;
  }

  // Construct register values:
  // Note: CNFG_SBBx_A/B are identical for SBB0-3. Using SBB0 to construct
  // values.

  // CNFG_SBB0_A:
  uint8_t cnfg_sbb_a = 0;
  cnfg_sbb_a = REG_FIELD_SET(CNFG_SBB0_A, TV_SBB, cnfg_sbb_a, tv_code);

  // CNFG_SBB0_B:
  uint8_t cnfg_sbb_b = 0;
  cnfg_sbb_b = REG_FIELD_SET(CNFG_SBB0_B, EN_SBB, cnfg_sbb_b, sbb_conf->en);
  cnfg_sbb_b = REG_FIELD_SET(CNFG_SBB0_B, ADE_SBB, cnfg_sbb_b, sbb_conf->active_discharge);
  cnfg_sbb_b = REG_FIELD_SET(CNFG_SBB0_B, IP_SBB, cnfg_sbb_b, sbb_conf->peak_current);
  cnfg_sbb_b = REG_FIELD_SET(CNFG_SBB0_B, OP_MODE, cnfg_sbb_b, sbb_conf->mode);

  // Write to register:
  // Make sure to adjust voltage first: In case the regulator is enabled
  // by this configuration, this prevents a previously configured (and possibly
  // higher voltage) being present on the output.
  err |= h->write_regs(adr_cnfg_a, 1, &cnfg_sbb_a); // voltage.
  err |= h->write_regs(adr_cnfg_b, 1, &cnfg_sbb_b); // enable and other settings.

  return err;
}

max77654_err_t max77654_config_ldo(const struct max77654_h *h, max77654_ldo_t ldo) {

  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  // Select the right registers, based on the LDO selected:
  uint8_t adr_cnfg_a;
  uint8_t adr_cnfg_b;

  switch (ldo) {
  case MAX77654_LDO0:
    adr_cnfg_a = REG(CNFG_LDO0_A);
    adr_cnfg_b = REG(CNFG_LDO0_B);
    break;
  case MAX77654_LDO1:
    adr_cnfg_a = REG(CNFG_LDO1_A);
    adr_cnfg_b = REG(CNFG_LDO1_B);
    break;
  default:
    DRVR_ERR("Invalid regulator..");
    err |= E_MAX77654_CONFIG_ERR;
    return err;
  }

  const struct max77654_ldo_conf *ldo_conf = &(h->conf.ldo_conf[ldo]);

  // Convert voltage to code if ldo is enabled and not a switch enabled:
  uint8_t tv_code;
  if (ldo_conf->en != MAX77654_REG_OFF && ldo_conf->mode != MAX77654_LDO_MODE_SW) {
    err |= encode_mV(h, ldo_conf->output_voltage_mV, true, &tv_code);
    if (err)
      return err;
  } else {
    tv_code = 0;
  }

  // Construct register values:
  // Note: CNFG_LDOx_A/B are identical for LDO0-1. Using LDO0 to construct
  // values.

  // CNFG_LDO0_A:
  uint8_t cnfg_ldo_a = 0;
  cnfg_ldo_a = REG_FIELD_SET(CNFG_LDO0_A, TV_LDO, cnfg_ldo_a, tv_code);

  // CNFG_LDO0_B:
  uint8_t cnfg_ldo_b = 0;
  cnfg_ldo_b = REG_FIELD_SET(CNFG_LDO0_B, EN_LDO, cnfg_ldo_b, ldo_conf->en);
  cnfg_ldo_b = REG_FIELD_SET(CNFG_LDO0_B, ADE_LDO, cnfg_ldo_b, ldo_conf->active_discharge);
  cnfg_ldo_b = REG_FIELD_SET(CNFG_LDO0_B, OP_MODE, cnfg_ldo_b, ldo_conf->mode);

  // Write to register:
  // Make sure to adjust voltage first: In case the regulator is enabled
  // by this configuration, this prevents a previously configured (and possibly
  // higher voltage) being present on the output.
  err |= h->write_regs(adr_cnfg_a, 1, &cnfg_ldo_a); // voltage.
  err |= h->write_regs(adr_cnfg_b, 1, &cnfg_ldo_b); // enable and other settings.

  return err;
}

max77654_err_t max77654_config_int(const struct max77654_h *h) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  uint8_t intm_glbl0 = 0;
  uint8_t intm_glbl1 = 0;
  uint8_t intm_chg = 0;

  intm_glbl0 = REG_FIELD_SET(INTM_GLBL0, DOD0_RM, intm_glbl0, !(h->conf.int_en.dod0_r));
  intm_glbl0 = REG_FIELD_SET(INTM_GLBL0, DOD1_RM, intm_glbl0, !(h->conf.int_en.dod1_r));
  intm_glbl0 = REG_FIELD_SET(INTM_GLBL0, TJAL1_RM, intm_glbl0, !(h->conf.int_en.tjal1_r));
  intm_glbl0 = REG_FIELD_SET(INTM_GLBL0, TJAL2_RM, intm_glbl0, !(h->conf.int_en.tjal2_r));
  intm_glbl0 = REG_FIELD_SET(INTM_GLBL0, nEN_RM, intm_glbl0, !(h->conf.int_en.nen_r));
  intm_glbl0 = REG_FIELD_SET(INTM_GLBL0, nEN_FM, intm_glbl0, !(h->conf.int_en.nen_f));
  intm_glbl0 = REG_FIELD_SET(INTM_GLBL0, GPI0_RM, intm_glbl0, !(h->conf.int_en.gpi0_r));
  intm_glbl0 = REG_FIELD_SET(INTM_GLBL0, GPI0_FM, intm_glbl0, !(h->conf.int_en.gpi0_f));

  intm_glbl1 = REG_FIELD_SET(INTM_GLBL1, LDO0_M, intm_glbl1, !(h->conf.int_en.ldo0_f));
  intm_glbl1 = REG_FIELD_SET(INTM_GLBL1, LDO1_M, intm_glbl1, !(h->conf.int_en.ldo1_f));
  intm_glbl1 = REG_FIELD_SET(INTM_GLBL1, GPI1_RM, intm_glbl1, !(h->conf.int_en.gpi1_r));
  intm_glbl1 = REG_FIELD_SET(INTM_GLBL1, GPI1_FM, intm_glbl1, !(h->conf.int_en.gpi1_f));
  intm_glbl1 = REG_FIELD_SET(INTM_GLBL1, GPI2_RM, intm_glbl1, !(h->conf.int_en.gpi2_r));
  intm_glbl1 = REG_FIELD_SET(INTM_GLBL1, GPI2_FM, intm_glbl1, !(h->conf.int_en.gpi2_f));
  intm_glbl1 = REG_FIELD_SET(INTM_GLBL1, SBB_TO_M, intm_glbl1, !(h->conf.int_en.sbb_to));

  intm_chg = REG_FIELD_SET(INTM_CHG, THM_I, intm_chg, !(h->conf.int_en.thm));
  intm_chg = REG_FIELD_SET(INTM_CHG, CHG_I, intm_chg, !(h->conf.int_en.chg));
  intm_chg = REG_FIELD_SET(INTM_CHG, CHGIN_I, intm_chg, !(h->conf.int_en.chgin));
  intm_chg = REG_FIELD_SET(INTM_CHG, TJ_REG_I, intm_chg, !(h->conf.int_en.tj_reg));
  intm_chg = REG_FIELD_SET(INTM_CHG, CHGIN_CTRL_I, intm_chg, !(h->conf.int_en.chgin_ctrl));
  intm_chg = REG_FIELD_SET(INTM_CHG, SYS_CTRL_I, intm_chg, !(h->conf.int_en.sys_ctrl));
  intm_chg = REG_FIELD_SET(INTM_CHG, SYS_CNFG_I, intm_chg, !(h->conf.int_en.sys_cnfg));

  err |= h->write_regs(REG(INTM_GLBL0), 1, &intm_glbl0);
  err |= h->write_regs(REG(INTM_GLBL1), 1, &intm_glbl1);
  err |= h->write_regs(REG(INTM_CHG), 1, &intm_chg);

  return err;
}

max77654_err_t max77654_config_gpio(const struct max77654_h *h, max77654_gpio_t gpio) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  uint8_t adr;
  switch (gpio) {
  case MAX77654_GPIO0:
    adr = REG(CNFG_GPIO0);
    break;
  case MAX77654_GPIO1:
    adr = REG(CNFG_GPIO1);
    break;
  case MAX77654_GPIO2:
    adr = REG(CNFG_GPIO2);
    break;
  default:
    err |= E_MAX77654_CONFIG_ERR;
    return err;
  }

  const struct max77654_gpio_conf *conf = &(h->conf.gpio_conf[gpio]);

  // CNFG_GPIO0:
  // Registers identical for all gpios. using GPIO0 to consruct:
  uint8_t cnfg_gpio = 0;
  cnfg_gpio = REG_FIELD_SET(CNFG_GPIO0, DIR, cnfg_gpio, conf->direction);
  cnfg_gpio = REG_FIELD_SET(CNFG_GPIO0, DRV, cnfg_gpio, conf->output_drive_type);
  cnfg_gpio = REG_FIELD_SET(CNFG_GPIO0, DO, cnfg_gpio, conf->output_value);
  cnfg_gpio = REG_FIELD_SET(CNFG_GPIO0, DBEN_GPI, cnfg_gpio, conf->input_debounce_enable);
  cnfg_gpio = REG_FIELD_SET(CNFG_GPIO0, ALT, cnfg_gpio, conf->alternate_mode);

  err |= h->write_regs(adr, 1, &cnfg_gpio);

  return err;
}

max77654_err_t max77654_get_int_flags(const struct max77654_h *h, struct max77654_int *flags) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  uint8_t int_glbl0 = 0;
  err |= h->read_regs(REG(INT_GLBL0), 1, &int_glbl0);
  if (err)
    return err;

  uint8_t int_glbl1 = 0;
  err |= h->read_regs(REG(INT_GLBL1), 1, &int_glbl1);
  if (err)
    return err;

  uint8_t int_chg = 0;
  err |= h->read_regs(REG(INT_CHG), 1, &int_chg);
  if (err)
    return err;

  flags->dod0_r = REG_FIELD_GET(INT_GLBL0, DOD0_R, int_glbl0);
  flags->dod1_r = REG_FIELD_GET(INT_GLBL0, DOD1_R, int_glbl0);
  flags->tjal1_r = REG_FIELD_GET(INT_GLBL0, TJAL1_R, int_glbl0);
  flags->tjal2_r = REG_FIELD_GET(INT_GLBL0, TJAL2_R, int_glbl0);
  flags->nen_r = REG_FIELD_GET(INT_GLBL0, nEN_R, int_glbl0);
  flags->nen_f = REG_FIELD_GET(INT_GLBL0, nEN_F, int_glbl0);
  flags->gpi0_r = REG_FIELD_GET(INT_GLBL0, GPI0_R, int_glbl0);
  flags->gpi0_f = REG_FIELD_GET(INT_GLBL0, GPI0_F, int_glbl0);

  flags->ldo0_f = REG_FIELD_GET(INT_GLBL1, LDO0_F, int_glbl1);
  flags->ldo1_f = REG_FIELD_GET(INT_GLBL1, LDO1_F, int_glbl1);
  flags->gpi1_r = REG_FIELD_GET(INT_GLBL1, GPI1_R, int_glbl1);
  flags->gpi1_f = REG_FIELD_GET(INT_GLBL1, GPI1_F, int_glbl1);
  flags->gpi2_r = REG_FIELD_GET(INT_GLBL1, GPI2_F, int_glbl1);
  flags->gpi2_f = REG_FIELD_GET(INT_GLBL1, GPI2_F, int_glbl1);
  flags->sbb_to = REG_FIELD_GET(INT_GLBL1, SBB_TO, int_glbl1);

  flags->thm = REG_FIELD_GET(INT_CHG, THM_I, int_chg);
  flags->chg = REG_FIELD_GET(INT_CHG, CHG_I, int_chg);
  flags->chgin = REG_FIELD_GET(INT_CHG, CHGIN_I, int_chg);
  flags->tj_reg = REG_FIELD_GET(INT_CHG, TJ_REG_I, int_chg);
  flags->chgin_ctrl = REG_FIELD_GET(INT_CHG, CHGIN_CTRL_I, int_chg);
  flags->sys_ctrl = REG_FIELD_GET(INT_CHG, SYS_CTRL_I, int_chg);
  flags->sys_cnfg = REG_FIELD_GET(INT_CHG, SYS_CNFG_I, int_chg);

  return err;
}

max77654_err_t max77654_get_erc_flags(const struct max77654_h *h, struct max77654_erc *flags) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  uint8_t ercflag = 0;

  err |= h->read_regs(REG(ERC_FLAG), 1, &ercflag);
  if (err)
    return err;

  flags->wdt_rst = REG_FIELD_GET(ERC_FLAG, WDT_RST, ercflag);
  flags->wdt_rst = REG_FIELD_GET(ERC_FLAG, WDT_OFF, ercflag);
  flags->sft_crst_f = REG_FIELD_GET(ERC_FLAG, SFT_CRST_F, ercflag);
  flags->sft_off_f = REG_FIELD_GET(ERC_FLAG, SFT_OFF_F, ercflag);
  flags->mrst = REG_FIELD_GET(ERC_FLAG, MRST, ercflag);
  flags->sysuvlo = REG_FIELD_GET(ERC_FLAG, SYSUVLO, ercflag);
  flags->sysovlo = REG_FIELD_GET(ERC_FLAG, SYSOVLO, ercflag);
  flags->tovld = REG_FIELD_GET(ERC_FLAG, TOVLD, ercflag);

  return err;
}

max77654_err_t max77654_get_stat(const struct max77654_h *h, struct max77654_stat *stat) {

  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  uint8_t stat_glbl = 0;
  err |= h->read_regs(REG(STAT_GLBL), 1, &stat_glbl);
  if (err)
    return err;

  uint8_t stat_chg_a = 0;
  err |= h->read_regs(REG(STAT_CHG_A), 1, &stat_chg_a);
  if (err)
    return err;

  uint8_t stat_chg_b = 0;
  err |= h->read_regs(REG(STAT_CHG_B), 1, &stat_chg_b);
  if (err)
    return err;

  stat->main_bias_ok = REG_FIELD_GET(STAT_GLBL, BOK, stat_glbl);
  stat->ldo0_in_dropout = REG_FIELD_GET(STAT_GLBL, DOD0_S, stat_glbl);
  stat->ldo1_in_dropout = REG_FIELD_GET(STAT_GLBL, DOD1_S, stat_glbl);
  stat->thermal_alarm_1 = REG_FIELD_GET(STAT_GLBL, TJAL1_S, stat_glbl);
  stat->thermal_alarm_2 = REG_FIELD_GET(STAT_GLBL, TJAL2_S, stat_glbl);
  stat->nEN_active = REG_FIELD_GET(STAT_GLBL, STAT_EN, stat_glbl);
  stat->nIRQ_active = REG_FIELD_GET(STAT_GLBL, STAT_IRQ, stat_glbl);

  stat->min_vchgin_regulation = REG_FIELD_GET(STAT_CHG_A, VCHGIN_MIN_STAT, stat_chg_a);
  stat->max_ichgin_regulation = REG_FIELD_GET(STAT_CHG_A, ICHGIN_LIM_STAT, stat_chg_a);
  stat->min_vsys_regulation = REG_FIELD_GET(STAT_CHG_A, VSYS_MIN_STAT, stat_chg_a);
  stat->max_junction_temp_regulation = REG_FIELD_GET(STAT_CHG_A, TJ_REG_STAT, stat_chg_a);

  stat->charging_active = REG_FIELD_GET(STAT_CHG_B, CHG, stat_chg_b);
  stat->charger_status = REG_FIELD_GET(STAT_CHG_B, CHG_DTLS, stat_chg_b);
  stat->chgin_status = REG_FIELD_GET(STAT_CHG_B, CHGIN_DTLS, stat_chg_b);
  stat->charger_timer_suspended = REG_FIELD_GET(STAT_CHG_B, TIME_SUS, stat_chg_b);

  if (stat->chgin_status == MAX77654_CHGIN_DTLS_OK) {
    stat->thermistor_status = REG_FIELD_GET(STAT_CHG_A, THM_DTLS, stat_chg_a);
  } else {
    stat->thermistor_status = MAX77654_THM_DTLS_OFF;
  }

  return err;
}

max77654_err_t max77654_read_gpio(const struct max77654_h *h, max77654_gpio_t gpio, bool *state) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  uint8_t adr;
  switch (gpio) {
  case MAX77654_GPIO0:
    adr = REG(CNFG_GPIO0);
    break;
  case MAX77654_GPIO1:
    adr = REG(CNFG_GPIO1);
    break;
  case MAX77654_GPIO2:
    adr = REG(CNFG_GPIO2);
    break;
  default:
    err = E_MAX77654_CONFIG_ERR;
    return err;
  }

  uint8_t cnfg_gpio = 0;
  err |= h->read_regs(adr, 1, &cnfg_gpio);

  // CNFG_GPIO identical for all gpios. Using 0:
  *state = REG_FIELD_GET(CNFG_GPIO0, DI, cnfg_gpio);

  return err;
}

static const uint8_t scale_lookup[] = {
    [MAX77654_BATT_I_8MA2] = REG_CONST(CNFG_CHG_I, IMON_DISCHG_SCALE, 8mA2),
    [MAX77654_BATT_I_40MA5] = REG_CONST(CNFG_CHG_I, IMON_DISCHG_SCALE, 40mA5),
    [MAX77654_BATT_I_72MA3] = REG_CONST(CNFG_CHG_I, IMON_DISCHG_SCALE, 72mA3),
    [MAX77654_BATT_I_103MA4] = REG_CONST(CNFG_CHG_I, IMON_DISCHG_SCALE, 103mA4),
    [MAX77654_BATT_I_134MA1] = REG_CONST(CNFG_CHG_I, IMON_DISCHG_SCALE, 134mA1),
    [MAX77654_BATT_I_164MA1] = REG_CONST(CNFG_CHG_I, IMON_DISCHG_SCALE, 164mA1),
    [MAX77654_BATT_I_193MA7] = REG_CONST(CNFG_CHG_I, IMON_DISCHG_SCALE, 193mA7),
    [MAX77654_BATT_I_222MA7] = REG_CONST(CNFG_CHG_I, IMON_DISCHG_SCALE, 222mA7),
    [MAX77654_BATT_I_251MA2] = REG_CONST(CNFG_CHG_I, IMON_DISCHG_SCALE, 251mA2),
    [MAX77654_BATT_I_279MA3] = REG_CONST(CNFG_CHG_I, IMON_DISCHG_SCALE, 279mA3),
    [MAX77654_BATT_I_300MA] = REG_CONST(CNFG_CHG_I, IMON_DISCHG_SCALE, 300mA),
};

max77654_err_t max77654_measure(const struct max77654_h *h, max77654_measure_t m, uint32_t *result) {
  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  if (h->adc_read == 0) {
    DRVR_ERR("adc_read is nullptr!")
    err |= E_MAX77654_NULLPTR_ERR;
  }
  if (err)
    return err;

  uint8_t cnfg_chg_i = 0;

  switch (m) {
  case MAX77654_CHGIN_V:;
    // Charging Input Voltage, in mV
    // Gain = 0.167 V/V = 167/1000
    const uint64_t gain_vchgin_num = 167;
    const uint64_t gain_vchgin_denum = 1000;
    err |= basic_measurement_e3(h, MAX77654_CHGIN_V, gain_vchgin_num, gain_vchgin_denum, result);
    break;

  case MAX77654_CHGIN_I:;
    // Charging Input Current, in mA
    // Gain = 2.632 A/V = 2632/1000
    const uint64_t gain_ichgin_num = 2632;
    const uint64_t gain_ichgin_denum = 1000;
    err |= basic_measurement_e3(h, MAX77654_CHGIN_I, gain_ichgin_num, gain_ichgin_denum, result);
    break;

  case MAX77654_BATT_V:;
    // Battery Voltage, in mV
    // Gain = 0.272 V/V = 272/1000
    const uint64_t gain_vbatt_num = 272;
    const uint64_t gain_vbatt_denum = 1000;
    err |= basic_measurement_e3(h, MAX77654_BATT_V, gain_vbatt_num, gain_vbatt_denum, result);
    break;

  case MAX77654_BATT_I_CHG:;
    // Battery Chargin Current, in percent of I_FAST_CHG
    // 0 = 0%, 100 = 100%
    // According to Datasheet: Gain = Vf = 1.25 V/V = 1250/1000
    // Because basic_measurement works in mV, this would return 1000
    // at full-scale. Therefor we use:
    // Gain = 12.5 = 1250/100
    const uint64_t gain_ichgbatt_num = 1250;
    const uint64_t gain_ichgbatt_denum = 100;
    err |= basic_measurement_e3(h, MAX77654_BATT_I_CHG, gain_ichgbatt_num, gain_ichgbatt_denum, result);
    break;

  case MAX77654_BATT_I_NULL:;
    // Battery pin discharge current null voltag, in mV
    // Gain = 1 = 1/1
    const uint64_t gain_ibatt_null_num = 1;
    const uint64_t gain_ibatt_null_denum = 1;
    err |= basic_measurement_e3(h, MAX77654_BATT_I_NULL, gain_ibatt_null_num, gain_ibatt_null_denum, result);
    break;

  case MAX77654_THM:;
    // THM pin voltgate, in mV
    // Gain = 1 = 1/1
    const uint64_t gain_th_num = 1;
    const uint64_t gain_th_denum = 1;
    err |= basic_measurement_e3(h, MAX77654_THM, gain_th_num, gain_th_denum, result);
    break;

  case MAX77654_TBIAS:;
    // TBIAS pin voltgate, in mV
    // Gain = 1 = 1/1
    const uint64_t gain_tbias_num = 1;
    const uint64_t gain_tbias_denum = 1;
    err |= basic_measurement_e3(h, MAX77654_TBIAS, gain_tbias_num, gain_tbias_denum, result);
    break;

  case MAX77654_AGND:;
    // AGND pin voltgate, in mV
    // Gain = 1 = 1/1
    const uint64_t gain_agnd_num = 1;
    const uint64_t gain_agnd_denum = 1;
    err |= basic_measurement_e3(h, MAX77654_AGND, gain_agnd_num, gain_agnd_denum, result);
    break;

  case MAX77654_VSYS:;
    // VSYS pin voltgate, in mV
    // Gain = 0.26V = 260/1000
    const uint64_t gain_vsys_num = 260;
    const uint64_t gain_vsys_denum = 1000;
    err |= basic_measurement_e3(h, MAX77654_VSYS, gain_vsys_num, gain_vsys_denum, result);
    break;

  case MAX77654_BATT_I_8MA2:
  case MAX77654_BATT_I_40MA5:
  case MAX77654_BATT_I_72MA3:
  case MAX77654_BATT_I_103MA4:
  case MAX77654_BATT_I_134MA1:
  case MAX77654_BATT_I_164MA1:
  case MAX77654_BATT_I_193MA7:
  case MAX77654_BATT_I_222MA7:
  case MAX77654_BATT_I_251MA2:
  case MAX77654_BATT_I_279MA3:
  case MAX77654_BATT_I_300MA:
    err |= bat_i_measurement(h, scale_lookup[m], result);
    break;

  default:
    return E_MAX77654_ERR;
  }

  // Return AMUX to OFF position, to save power:
  cnfg_chg_i = REG_FIELD_SET_CONST(CNFG_CHG_I, MUX_SEL, cnfg_chg_i, DISABLED);
  err |= h->write_regs(REG(CNFG_CHG_I), 1, &cnfg_chg_i);
  return err;
}

max77654_err_t max77654_clr_wdt(const struct max77654_h *h) {

  max77654_err_t err = E_MAX77654_SUCCESS;
  err |= validate_handle(h);
  if (err)
    return err;

  // CNFG_WDT:
  uint8_t cnfg_wdt = 0;
  cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_PER, cnfg_wdt, h->conf.wdt_period);
  cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_MODE, cnfg_wdt, h->conf.wdt_action_on_ovf);
  cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_CLR, cnfg_wdt, 1);
  cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_EN, cnfg_wdt, h->conf.wdt_enable);
  cnfg_wdt = REG_FIELD_SET(CNFG_WDT, WDT_LOCK, cnfg_wdt, 1);

  err |= h->write_regs(REG(CNFG_WDT), 1, &cnfg_wdt);
  return err;
}

// ======== Private Functions =================================================================

max77654_err_t check_connection(const struct max77654_h *h) {

  max77654_err_t err = E_MAX77654_SUCCESS;

  // Check PART_ID register
  uint8_t cid_reg = 0;
  err |= h->read_regs(REG(CID), 1, &cid_reg);
  if (err) {
    DRVR_ERR("Failed to read CID register!");
    return err;
  }

  uint8_t cid = (REG_FIELD_GET(CID, CID_MSB, cid_reg) << 4) | REG_FIELD_GET(CID, CID_LSB, cid_reg);
  DRVR_LOG_VAR("MAX77654 CID: ", (int)cid);

  if (cid != h->conf.cid) {
    DRVR_ERR_VAR("MAX77654 CID should be:", (int)h->conf.cid);
    err |= E_MAX77654_COM_ERR;
    return err;
  }

  return err;
}

max77654_err_t validate_handle(const struct max77654_h *h) {
  max77654_err_t err = E_MAX77654_SUCCESS;

  if (h == 0) {
    return E_MAX77654_NULLPTR_ERR;
  }

  if (h->read_regs == 0) {
    DRVR_ERR("read_regs is nullptr!")
    err |= E_MAX77654_NULLPTR_ERR;
  }

  if (h->write_regs == 0) {
    DRVR_ERR("write_regs is nullptr!")
    err |= E_MAX77654_NULLPTR_ERR;
  }

  return err;
}

static max77654_err_t encode_mV(const struct max77654_h *h, uint32_t mV, bool is_ldo, uint8_t *code) {
  if (is_ldo) {

    if (mV < 800 || mV > 3975 || (mV % 25) != 0) {
      DRVR_ERR_VAR("MAX77654 LDO Output voltage must be between 800mV and 3975mV, in 25mV steps. Is: ", mV);
      return E_MAX77654_CONFIG_ERR;
    }

    *code = (mV - 800) / 25;

  } else {

    if (mV < 800 || mV > 5500 || (mV % 50) != 0) {
      DRVR_ERR_VAR("MAX77654 SBB Output voltage must be between 800mV and 5500mV, in 50mV steps. Is: ", mV);
      return E_MAX77654_CONFIG_ERR;
    }

    *code = (mV - 800) / 50;
  }

  return E_MAX77654_SUCCESS;
}

static max77654_err_t basic_measurement_e3(const struct max77654_h *h, max77654_measure_t m, uint64_t gain_num,
                                           uint64_t gain_denum, uint32_t *result) {
  max77654_err_t err = E_MAX77654_SUCCESS;

  uint8_t cnfg_chg_i = 0;

  // Set AMUX to desired position:
  cnfg_chg_i = REG_FIELD_SET(CNFG_CHG_I, MUX_SEL, cnfg_chg_i, m);
  err |= h->write_regs(REG(CNFG_CHG_I), 1, &cnfg_chg_i);
  if (err)
    return err;

  // ADC read:
  struct max77654_adc_reading adc_result = {0};
  err |= h->adc_read(&adc_result);
  if (err)
    return err;

  // Perform conversion:
  // result = v_amux / gain = v_amxu * gain_denum / gain_num
  // FIXME: This does not have to be done at 64bit - but I am too lazy to figure work
  // out the order of ops to prevent overflow right now,
  uint64_t r = adc_result.reading;
  uint64_t mV_fs = adc_result.adc_fullscale_mV;
  uint64_t max = adc_result.adc_max_reading;

  uint64_t result64 = (r * mV_fs * gain_denum) / (max * gain_num);

  *result = (uint32_t)result64;

  return E_MAX77654_SUCCESS;
}

static max77654_err_t bat_i_measurement(const struct max77654_h *h, uint8_t scale, uint32_t *result) {
  max77654_err_t err = E_MAX77654_SUCCESS;

  // Set I_MON_DISCHG_SCALE
  uint8_t cnfg_chg_i = 0;
  cnfg_chg_i = REG_FIELD_SET(CNFG_CHG_I, IMON_DISCHG_SCALE, cnfg_chg_i, scale);

  // Battery Current, in percent of I_MON_DISCHG_SCALE
  // 0 = 0%, 100 = 100%
  // First measure the battery current null voltage:
  cnfg_chg_i = REG_FIELD_SET_CONST(CNFG_CHG_I, MUX_SEL, cnfg_chg_i, BATT_DISCHG_I_NULL);
  err |= h->write_regs(REG(CNFG_CHG_I), 1, &cnfg_chg_i);
  if (err)
    return err;

  struct max77654_adc_reading adc_batt_i_null = {0};
  err |= h->adc_read(&adc_batt_i_null);
  if (err)
    return err;

  // Next measure the output voltage:
  cnfg_chg_i = REG_FIELD_SET_CONST(CNFG_CHG_I, MUX_SEL, cnfg_chg_i, BATT_DISCHG_I);
  err |= h->write_regs(REG(CNFG_CHG_I), 1, &cnfg_chg_i);
  if (err)
    return err;

  struct max77654_adc_reading adc_batt_i = {0};
  err |= h->adc_read(&adc_batt_i);
  if (err)
    return err;

  // Calculate result:
  // First check if batt_i read less than batt_i_null:
  uint32_t mv_i = (adc_batt_i.reading * adc_batt_i.adc_fullscale_mV) / adc_batt_i.adc_max_reading;
  uint32_t mv_null = (adc_batt_i_null.reading * adc_batt_i_null.adc_fullscale_mV) / adc_batt_i_null.adc_max_reading;
  if (mv_i < mv_null) {
    // This can happen due to ADC noise.
    // Set result to 0, as battery current is to small to measure and the calculation
    // below would be wrong.
    *result = 0;
  } else {

    // Done at 64b precisions, to avoid overflows of intermediat results.
    uint64_t r_i = adc_batt_i.reading;              // Reading of current
    uint64_t max_i = adc_batt_i.adc_max_reading;    // Max ADC reading for current
    uint64_t mV_fs_i = adc_batt_i.adc_fullscale_mV; // ADC Fullscale in mV for current

    uint64_t r_n = adc_batt_i_null.reading;              // Reading of null
    uint64_t max_n = adc_batt_i_null.adc_max_reading;    // Max ADC reading for null
    uint64_t mV_fs_n = adc_batt_i_null.adc_fullscale_mV; // ADC Fullscale in mV for null

    uint64_t mV_fs = 1250; // AMUX output fullscale voltage in mV

    uint64_t result64 =
        (r_i * mV_fs_i * max_n * 100 - r_n * mV_fs_n * max_i * 100) / (mV_fs * max_n * max_i - r_n * mV_fs_n * max_i);

    // return result as 32b:
    *result = (uint32_t)result64;
  }
  return err;
}
