// Copyright (c) 2022 Philip Schilk.
// SPDX-License-Identifier: Apache-2.0

/*
 * MAX77654 Driver
 *
 * Philipp Schilk
 * PBL, D-ITET, ETH Zürich
 */

#ifndef MAX77654_REG_H_
#define MAX77654_REG_H_

// ==== INT_GLBL0 ==============================================================
// Global Interrupt flag register 0.
#define MAX77654__REG_INT_GLBL0 (0x00U)                                // Register Address
#define MAX77654__REG_INT_GLBL0__FIELD_DOD0_R (0x80U)                  // Field Mask
#define MAX77654__REG_INT_GLBL0__FIELD_DOD0_R__CONST_DISABLED (0x00U)  // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_DOD0_R__CONST_ENABLED (0x01U)   // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_DOD1_R (0x40U)                  // Field Mask
#define MAX77654__REG_INT_GLBL0__FIELD_DOD1_R__CONST_DISABLED (0x00U)  // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_DOD1_R__CONST_ENABLED (0x01U)   // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_TJAL2_R (0x20U)                 // Field Mask
#define MAX77654__REG_INT_GLBL0__FIELD_TJAL2_R__CONST_DISABLED (0x00U) // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_TJAL2_R__CONST_ENABLED (0x01U)  // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_TJAL1_R (0x10U)                 // Field Mask
#define MAX77654__REG_INT_GLBL0__FIELD_TJAL1_R__CONST_DISABLED (0x00U) // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_TJAL1_R__CONST_ENABLED (0x01U)  // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_nEN_R (0x08U)                   // Field Mask
#define MAX77654__REG_INT_GLBL0__FIELD_nEN_R__CONST_DISABLED (0x00U)   // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_nEN_R__CONST_ENABLED (0x01U)    // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_nEN_F (0x04U)                   // Field Mask
#define MAX77654__REG_INT_GLBL0__FIELD_nEN_F__CONST_DISABLED (0x00U)   // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_nEN_F__CONST_ENABLED (0x01U)    // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_GPI0_R (0x02U)                  // Field Mask
#define MAX77654__REG_INT_GLBL0__FIELD_GPI0_R__CONST_DISABLED (0x00U)  // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_GPI0_R__CONST_ENABLED (0x01U)   // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_GPI0_F (0x01U)                  // Field Mask
#define MAX77654__REG_INT_GLBL0__FIELD_GPI0_F__CONST_DISABLED (0x00U)  // Constant
#define MAX77654__REG_INT_GLBL0__FIELD_GPI0_F__CONST_ENABLED (0x01U)   // Constant

// ==== INT_GLBL1 ==============================================================
// Global Interrupt flag register 1.
#define MAX77654__REG_INT_GLBL1 (0x04U)                               // Register Address
#define MAX77654__REG_INT_GLBL1__FIELD_LDO1_F (0x40U)                 // Field Mask
#define MAX77654__REG_INT_GLBL1__FIELD_LDO1_F__CONST_DISABLED (0x00U) // Constant
#define MAX77654__REG_INT_GLBL1__FIELD_LDO1_F__CONST_ENABLED (0x01U)  // Constant
#define MAX77654__REG_INT_GLBL1__FIELD_LDO0_F (0x20U)                 // Field Mask
#define MAX77654__REG_INT_GLBL1__FIELD_LDO0_F__CONST_DISABLED (0x00U) // Constant
#define MAX77654__REG_INT_GLBL1__FIELD_LDO0_F__CONST_ENABLED (0x01U)  // Constant
#define MAX77654__REG_INT_GLBL1__FIELD_SBB_TO (0x10U)                 // Field Mask
#define MAX77654__REG_INT_GLBL1__FIELD_GPI2_R (0x08U)                 // Field Mask
#define MAX77654__REG_INT_GLBL1__FIELD_GPI2_R__CONST_DISABLED (0x00U) // Constant
#define MAX77654__REG_INT_GLBL1__FIELD_GPI2_R__CONST_ENABLED (0x01U)  // Constant
#define MAX77654__REG_INT_GLBL1__FIELD_GPI2_F (0x04U)                 // Field Mask
#define MAX77654__REG_INT_GLBL1__FIELD_GPI2_F__CONST_DISABLED (0x00U) // Constant
#define MAX77654__REG_INT_GLBL1__FIELD_GPI2_F__CONST_ENABLED (0x01U)  // Constant
#define MAX77654__REG_INT_GLBL1__FIELD_GPI1_R (0x02U)                 // Field Mask
#define MAX77654__REG_INT_GLBL1__FIELD_GPI1_R__CONST_DISABLED (0x00U) // Constant
#define MAX77654__REG_INT_GLBL1__FIELD_GPI1_R__CONST_ENABLED (0x01U)  // Constant
#define MAX77654__REG_INT_GLBL1__FIELD_GPI1_F (0x01U)                 // Field Mask
#define MAX77654__REG_INT_GLBL1__FIELD_GPI1_F__CONST_DISABLED (0x00U) // Constant
#define MAX77654__REG_INT_GLBL1__FIELD_GPI1_F__CONST_ENABLED (0x01U)  // Constant

// ==== ERC_FLAG ===============================================================
// Fault Status Register
#define MAX77654__REG_ERC_FLAG (0x05U)                   // Register Address
#define MAX77654__REG_ERC_FLAG__FIELD_WDT_RST (0x80U)    // Field Mask
#define MAX77654__REG_ERC_FLAG__FIELD_WDT_OFF (0x40U)    // Field Mask
#define MAX77654__REG_ERC_FLAG__FIELD_SFT_CRST_F (0x20U) // Field Mask
#define MAX77654__REG_ERC_FLAG__FIELD_SFT_OFF_F (0x10U)  // Field Mask
#define MAX77654__REG_ERC_FLAG__FIELD_MRST (0x08U)       // Field Mask
#define MAX77654__REG_ERC_FLAG__FIELD_SYSUVLO (0x04U)    // Field Mask
#define MAX77654__REG_ERC_FLAG__FIELD_SYSOVLO (0x02U)    // Field Mask
#define MAX77654__REG_ERC_FLAG__FIELD_TOVLD (0x01U)      // Field Mask

// ==== STAT_GLBL ==============================================================
// Global Status
#define MAX77654__REG_STAT_GLBL (0x06U)                 // Register Address
#define MAX77654__REG_STAT_GLBL__FIELD_DIDM (0x80U)     // Field Mask
#define MAX77654__REG_STAT_GLBL__FIELD_BOK (0x40U)      // Field Mask
#define MAX77654__REG_STAT_GLBL__FIELD_DOD0_S (0x20U)   // Field Mask
#define MAX77654__REG_STAT_GLBL__FIELD_DOD1_S (0x10U)   // Field Mask
#define MAX77654__REG_STAT_GLBL__FIELD_TJAL2_S (0x08U)  // Field Mask
#define MAX77654__REG_STAT_GLBL__FIELD_TJAL1_S (0x04U)  // Field Mask
#define MAX77654__REG_STAT_GLBL__FIELD_STAT_EN (0x02U)  // Field Mask
#define MAX77654__REG_STAT_GLBL__FIELD_STAT_IRQ (0x01U) // Field Mask

// ==== INTM_GLBL1 =============================================================
// Interrupt Masking 1
#define MAX77654__REG_INTM_GLBL1 (0x08U)                                 // Register Address
#define MAX77654__REG_INTM_GLBL1__FIELD_LDO1_M (0x40U)                   // Field Mask
#define MAX77654__REG_INTM_GLBL1__FIELD_LDO1_M__CONST_UNMASKED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_LDO1_M__CONST_ENABLED (0x00U)    // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_LDO1_M__CONST_MASKED (0x01U)     // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_LDO1_M__CONST_DISABLED (0x01U)   // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_LDO0_M (0x20U)                   // Field Mask
#define MAX77654__REG_INTM_GLBL1__FIELD_LDO0_M__CONST_UNMASKED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_LDO0_M__CONST_ENABLED (0x00U)    // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_LDO0_M__CONST_MASKED (0x01U)     // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_LDO0_M__CONST_DISABLED (0x01U)   // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_SBB_TO_M (0x10U)                 // Field Mask
#define MAX77654__REG_INTM_GLBL1__FIELD_SBB_TO_M__CONST_UNMASKED (0x00U) // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_SBB_TO_M__CONST_ENABLED (0x00U)  // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_SBB_TO_M__CONST_MASKED (0x01U)   // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_SBB_TO_M__CONST_DISABLED (0x01U) // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI2_RM (0x08U)                  // Field Mask
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI2_RM__CONST_UNMASKED (0x00U)  // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI2_RM__CONST_ENABLED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI2_RM__CONST_MASKED (0x01U)    // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI2_RM__CONST_DISABLED (0x01U)  // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI2_FM (0x04U)                  // Field Mask
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI2_FM__CONST_UNMASKED (0x00U)  // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI2_FM__CONST_ENABLED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI2_FM__CONST_MASKED (0x01U)    // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI2_FM__CONST_DISABLED (0x01U)  // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI1_RM (0x02U)                  // Field Mask
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI1_RM__CONST_UNMASKED (0x00U)  // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI1_RM__CONST_ENABLED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI1_RM__CONST_MASKED (0x01U)    // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI1_RM__CONST_DISABLED (0x01U)  // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI1_FM (0x01U)                  // Field Mask
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI1_FM__CONST_UNMASKED (0x00U)  // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI1_FM__CONST_ENABLED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI1_FM__CONST_MASKED (0x01U)    // Constant
#define MAX77654__REG_INTM_GLBL1__FIELD_GPI1_FM__CONST_DISABLED (0x01U)  // Constant

// ==== INTM_GLBL0 =============================================================
// Interrupt Masking 0
#define MAX77654__REG_INTM_GLBL0 (0x09U)                                 // Register Address
#define MAX77654__REG_INTM_GLBL0__FIELD_DOD0_RM (0x80U)                  // Field Mask
#define MAX77654__REG_INTM_GLBL0__FIELD_DOD0_RM__CONST_UNMASKED (0x00U)  // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_DOD0_RM__CONST_ENABLED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_DOD0_RM__CONST_MASKED (0x01U)    // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_DOD0_RM__CONST_DISABLED (0x01U)  // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_DOD1_RM (0x40U)                  // Field Mask
#define MAX77654__REG_INTM_GLBL0__FIELD_DOD1_RM__CONST_UNMASKED (0x00U)  // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_DOD1_RM__CONST_ENABLED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_DOD1_RM__CONST_MASKED (0x01U)    // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_DOD1_RM__CONST_DISABLED (0x01U)  // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_TJAL2_RM (0x20U)                 // Field Mask
#define MAX77654__REG_INTM_GLBL0__FIELD_TJAL2_RM__CONST_UNMASKED (0x00U) // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_TJAL2_RM__CONST_ENABLED (0x00U)  // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_TJAL2_RM__CONST_MASKED (0x01U)   // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_TJAL2_RM__CONST_DISABLED (0x01U) // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_TJAL1_RM (0x10U)                 // Field Mask
#define MAX77654__REG_INTM_GLBL0__FIELD_TJAL1_RM__CONST_UNMASKED (0x00U) // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_TJAL1_RM__CONST_ENABLED (0x00U)  // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_TJAL1_RM__CONST_MASKED (0x01U)   // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_TJAL1_RM__CONST_DISABLED (0x01U) // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_nEN_RM (0x08U)                   // Field Mask
#define MAX77654__REG_INTM_GLBL0__FIELD_nEN_RM__CONST_UNMASKED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_nEN_RM__CONST_ENABLED (0x00U)    // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_nEN_RM__CONST_MASKED (0x01U)     // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_nEN_RM__CONST_DISABLED (0x01U)   // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_nEN_FM (0x04U)                   // Field Mask
#define MAX77654__REG_INTM_GLBL0__FIELD_nEN_FM__CONST_UNMASKED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_nEN_FM__CONST_ENABLED (0x00U)    // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_nEN_FM__CONST_MASKED (0x01U)     // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_nEN_FM__CONST_DISABLED (0x01U)   // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_GPI0_RM (0x02U)                  // Field Mask
#define MAX77654__REG_INTM_GLBL0__FIELD_GPI0_RM__CONST_UNMASKED (0x00U)  // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_GPI0_RM__CONST_ENABLED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_GPI0_RM__CONST_MASKED (0x01U)    // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_GPI0_RM__CONST_DISABLED (0x01U)  // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_GPI0_FM (0x01U)                  // Field Mask
#define MAX77654__REG_INTM_GLBL0__FIELD_GPI0_FM__CONST_UNMASKED (0x00U)  // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_GPI0_FM__CONST_ENABLED (0x00U)   // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_GPI0_FM__CONST_MASKED (0x01U)    // Constant
#define MAX77654__REG_INTM_GLBL0__FIELD_GPI0_FM__CONST_DISABLED (0x01U)  // Constant

// ==== CNFG_GLBL ==============================================================
// Global Configuration
#define MAX77654__REG_CNFG_GLBL (0x10U)                                  // Register Address
#define MAX77654__REG_CNFG_GLBL__FIELD_PU_DIS (0x80U)                    // Field Mask
#define MAX77654__REG_CNFG_GLBL__FIELD_PU_DIS__CONST_200K (0x00U)        // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_PU_DIS__CONST_10M (0x01U)         // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_T_MRST (0x40U)                    // Field Mask
#define MAX77654__REG_CNFG_GLBL__FIELD_T_MRST__CONST_8s (0x00U)          // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_T_MRST__CONST_16s (0x01U)         // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_SBIA_LPM (0x20U)                  // Field Mask
#define MAX77654__REG_CNFG_GLBL__FIELD_SBIA_EN (0x10U)                   // Field Mask
#define MAX77654__REG_CNFG_GLBL__FIELD_SBIA_EN__CONST_DISABLED (0x00U)   // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_SBIA_EN__CONST_ENABLED (0x01U)    // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_nEN_MODE (0x08U)                  // Field Mask
#define MAX77654__REG_CNFG_GLBL__FIELD_nEN_MODE__CONST_PUSHBTN (0x00U)   // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_nEN_MODE__CONST_SLIDE (0x01U)     // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_DBEN_nEN (0x04U)                  // Field Mask
#define MAX77654__REG_CNFG_GLBL__FIELD_DBEN_nEN__CONST_500us (0x00U)     // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_DBEN_nEN__CONST_30ms (0x01U)      // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_SFT_CTRL (0x03U)                  // Field Mask
#define MAX77654__REG_CNFG_GLBL__FIELD_SFT_CTRL__CONST_NO_ACTION (0x00U) // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_SFT_CTRL__CONST_SFT_CRST (0x01U)  // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_SFT_CTRL__CONST_SFT_OFF (0x02U)   // Constant
#define MAX77654__REG_CNFG_GLBL__FIELD_SFT_CTRL__CONST_FSM (0x03U)       // Constant

// ==== CNFG_GPIO0 =============================================================
// GPIO 0 Configuration
#define MAX77654__REG_CNFG_GPIO0 (0x11U)                                 // Register Address
#define MAX77654__REG_CNFG_GPIO0__FIELD_ALT (0x20U)                      // Field Mask
#define MAX77654__REG_CNFG_GPIO0__FIELD_ALT__CONST_DISABLED (0x00U)      // Constant
#define MAX77654__REG_CNFG_GPIO0__FIELD_ALT__CONST_ENABLED (0x01U)       // Constant
#define MAX77654__REG_CNFG_GPIO0__FIELD_DBEN_GPI (0x10U)                 // Field Mask
#define MAX77654__REG_CNFG_GPIO0__FIELD_DBEN_GPI__CONST_DISABLED (0x00U) // Constant
#define MAX77654__REG_CNFG_GPIO0__FIELD_DBEN_GPI__CONST_ENABLED (0x01U)  // Constant
#define MAX77654__REG_CNFG_GPIO0__FIELD_DO (0x08U)                       // Field Mask
#define MAX77654__REG_CNFG_GPIO0__FIELD_DRV (0x04U)                      // Field Mask
#define MAX77654__REG_CNFG_GPIO0__FIELD_DRV__CONST_OD (0x00U)            // Constant
#define MAX77654__REG_CNFG_GPIO0__FIELD_DRV__CONST_PP (0x01U)            // Constant
#define MAX77654__REG_CNFG_GPIO0__FIELD_DI (0x02U)                       // Field Mask
#define MAX77654__REG_CNFG_GPIO0__FIELD_DIR (0x01U)                      // Field Mask
#define MAX77654__REG_CNFG_GPIO0__FIELD_DIR__CONST_OUTPUT (0x00U)        // Constant
#define MAX77654__REG_CNFG_GPIO0__FIELD_DIR__CONST_INPUT (0x01U)         // Constant

// ==== CNFG_GPIO1 =============================================================
// GPIO 1 Configuration
#define MAX77654__REG_CNFG_GPIO1 (0x12U)                          // Register Address
#define MAX77654__REG_CNFG_GPIO1__FIELD_ALT (0x20U)               // Field Mask
#define MAX77654__REG_CNFG_GPIO1__FIELD_DBEN_GPI (0x10U)          // Field Mask
#define MAX77654__REG_CNFG_GPIO1__FIELD_DO (0x08U)                // Field Mask
#define MAX77654__REG_CNFG_GPIO1__FIELD_DRV (0x04U)               // Field Mask
#define MAX77654__REG_CNFG_GPIO1__FIELD_DRV__CONST_OD (0x00U)     // Constant
#define MAX77654__REG_CNFG_GPIO1__FIELD_DRV__CONST_PP (0x01U)     // Constant
#define MAX77654__REG_CNFG_GPIO1__FIELD_DI (0x02U)                // Field Mask
#define MAX77654__REG_CNFG_GPIO1__FIELD_DIR (0x01U)               // Field Mask
#define MAX77654__REG_CNFG_GPIO1__FIELD_DIR__CONST_OUTPUT (0x00U) // Constant
#define MAX77654__REG_CNFG_GPIO1__FIELD_DIR__CONST_INPUT (0x01U)  // Constant

// ==== CNFG_GPIO2 =============================================================
// GPIO 2 Configuration
#define MAX77654__REG_CNFG_GPIO2 (0x13U)                          // Register Address
#define MAX77654__REG_CNFG_GPIO2__FIELD_ALT (0x20U)               // Field Mask
#define MAX77654__REG_CNFG_GPIO2__FIELD_DBEN_GPI (0x10U)          // Field Mask
#define MAX77654__REG_CNFG_GPIO2__FIELD_DO (0x08U)                // Field Mask
#define MAX77654__REG_CNFG_GPIO2__FIELD_DRV (0x04U)               // Field Mask
#define MAX77654__REG_CNFG_GPIO2__FIELD_DRV__CONST_OD (0x00U)     // Constant
#define MAX77654__REG_CNFG_GPIO2__FIELD_DRV__CONST_PP (0x01U)     // Constant
#define MAX77654__REG_CNFG_GPIO2__FIELD_DI (0x02U)                // Field Mask
#define MAX77654__REG_CNFG_GPIO2__FIELD_DIR (0x01U)               // Field Mask
#define MAX77654__REG_CNFG_GPIO2__FIELD_DIR__CONST_OUTPUT (0x00U) // Constant
#define MAX77654__REG_CNFG_GPIO2__FIELD_DIR__CONST_INPUT (0x01U)  // Constant

// ==== CID ====================================================================
// Chip Identification Register
#define MAX77654__REG_CID (0x14U)                // Register Address
#define MAX77654__REG_CID__FIELD_CID_MSB (0x80U) // Field Mask
#define MAX77654__REG_CID__FIELD_CID_LSB (0x0FU) // Field Mask

// ==== CNFG_WDT ===============================================================
// Watchdog timer configurtion.
#define MAX77654__REG_CNFG_WDT (0x17U)                                  // Register Address
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_PER (0x30U)                   // Field Mask
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_PER__CONST_16s (0x00U)        // Constant
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_PER__CONST_32s (0x01U)        // Constant
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_PER__CONST_64s (0x02U)        // Constant
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_PER__CONST_128s (0x03U)       // Constant
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_MODE (0x08U)                  // Field Mask
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_MODE__CONST_PWR_OFF (0x00U)   // Constant
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_MODE__CONST_PWR_RESET (0x01U) // Constant
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_CLR (0x04U)                   // Field Mask
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_EN (0x02U)                    // Field Mask
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_EN__CONST_DISABLED (0x00U)    // Constant
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_EN__CONST_ENABLED (0x01U)     // Constant
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_LOCK (0x01U)                  // Field Mask
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_LOCK__CONST_DISABLED (0x00U)  // Constant
#define MAX77654__REG_CNFG_WDT__FIELD_WDT_LOCK__CONST_ENABLED (0x01U)   // Constant

// ==== INT_CHG ================================================================
// Charger Interrupt flag register
#define MAX77654__REG_INT_CHG (0x01U)                     // Register Address
#define MAX77654__REG_INT_CHG__FIELD_SYS_CNFG_I (0x40U)   // Field Mask
#define MAX77654__REG_INT_CHG__FIELD_SYS_CTRL_I (0x20U)   // Field Mask
#define MAX77654__REG_INT_CHG__FIELD_CHGIN_CTRL_I (0x10U) // Field Mask
#define MAX77654__REG_INT_CHG__FIELD_TJ_REG_I (0x08U)     // Field Mask
#define MAX77654__REG_INT_CHG__FIELD_CHGIN_I (0x04U)      // Field Mask
#define MAX77654__REG_INT_CHG__FIELD_CHG_I (0x02U)        // Field Mask
#define MAX77654__REG_INT_CHG__FIELD_THM_I (0x01U)        // Field Mask

// ==== STAT_CHG_A =============================================================
// Charger status register A.
#define MAX77654__REG_STAT_CHG_A (0x02U)                                 // Register Address
#define MAX77654__REG_STAT_CHG_A__FIELD_VCHGIN_MIN_STAT (0x40U)          // Field Mask
#define MAX77654__REG_STAT_CHG_A__FIELD_ICHGIN_LIM_STAT (0x20U)          // Field Mask
#define MAX77654__REG_STAT_CHG_A__FIELD_VSYS_MIN_STAT (0x10U)            // Field Mask
#define MAX77654__REG_STAT_CHG_A__FIELD_TJ_REG_STAT (0x08U)              // Field Mask
#define MAX77654__REG_STAT_CHG_A__FIELD_THM_DTLS (0x07U)                 // Field Mask
#define MAX77654__REG_STAT_CHG_A__FIELD_THM_DTLS__CONST_THM_OFF (0x00U)  // Constant
#define MAX77654__REG_STAT_CHG_A__FIELD_THM_DTLS__CONST_THM_COLD (0x01U) // Constant
#define MAX77654__REG_STAT_CHG_A__FIELD_THM_DTLS__CONST_THM_COOL (0x02U) // Constant
#define MAX77654__REG_STAT_CHG_A__FIELD_THM_DTLS__CONST_THM_WARM (0x03U) // Constant
#define MAX77654__REG_STAT_CHG_A__FIELD_THM_DTLS__CONST_THM_HOT (0x04U)  // Constant
#define MAX77654__REG_STAT_CHG_A__FIELD_THM_DTLS__CONST_THM_OK (0x05U)   // Constant

// ==== STAT_CHG_B =============================================================
// Charger status register B.
#define MAX77654__REG_STAT_CHG_B (0x03U)                                            // Register Address
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS (0xF0U)                            // Field Mask
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_OFF (0x00U)                 // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_PQ (0x01U)                  // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_CC (0x02U)                  // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_CC_JEITA (0x03U)            // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_CV (0x04U)                  // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_CV_JEITA (0x05U)            // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_TOPOFF (0x06U)              // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_TOPOFF_JEITA (0x07U)        // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_DONE (0x08U)                // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_DONE_JEITA (0x09U)          // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_PQ_TIMER_FAULT (0x0AU)      // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_FASTCHG_TIMER_FAULT (0x0BU) // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG_DTLS__CONST_BAT_TEMP_FAULT (0x0CU)      // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHGIN_DTLS (0x0CU)                          // Field Mask
#define MAX77654__REG_STAT_CHG_B__FIELD_CHGIN_DTLS__CONST_UVLO (0x00U)              // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHGIN_DTLS__CONST_OVP (0x01U)               // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHGIN_DTLS__CONST_DBNC (0x02U)              // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHGIN_DTLS__CONST_OK (0x03U)                // Constant
#define MAX77654__REG_STAT_CHG_B__FIELD_CHG (0x02U)                                 // Field Mask
#define MAX77654__REG_STAT_CHG_B__FIELD_TIME_SUS (0x01U)                            // Field Mask

// ==== INTM_CHG ===============================================================
// Charger Interrupt masking
#define MAX77654__REG_INTM_CHG (0x07U)                                     // Register Address
#define MAX77654__REG_INTM_CHG__FIELD_SYS_CNFG_I (0x40U)                   // Field Mask
#define MAX77654__REG_INTM_CHG__FIELD_SYS_CNFG_I__CONST_UNMASKED (0x00U)   // Constant
#define MAX77654__REG_INTM_CHG__FIELD_SYS_CNFG_I__CONST_ENABLED (0x00U)    // Constant
#define MAX77654__REG_INTM_CHG__FIELD_SYS_CNFG_I__CONST_MASKED (0x01U)     // Constant
#define MAX77654__REG_INTM_CHG__FIELD_SYS_CNFG_I__CONST_DISABLED (0x01U)   // Constant
#define MAX77654__REG_INTM_CHG__FIELD_SYS_CTRL_I (0x20U)                   // Field Mask
#define MAX77654__REG_INTM_CHG__FIELD_SYS_CTRL_I__CONST_UNMASKED (0x00U)   // Constant
#define MAX77654__REG_INTM_CHG__FIELD_SYS_CTRL_I__CONST_ENABLED (0x00U)    // Constant
#define MAX77654__REG_INTM_CHG__FIELD_SYS_CTRL_I__CONST_MASKED (0x01U)     // Constant
#define MAX77654__REG_INTM_CHG__FIELD_SYS_CTRL_I__CONST_DISABLED (0x01U)   // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHGIN_CTRL_I (0x10U)                 // Field Mask
#define MAX77654__REG_INTM_CHG__FIELD_CHGIN_CTRL_I__CONST_UNMASKED (0x00U) // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHGIN_CTRL_I__CONST_ENABLED (0x00U)  // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHGIN_CTRL_I__CONST_MASKED (0x01U)   // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHGIN_CTRL_I__CONST_DISABLED (0x01U) // Constant
#define MAX77654__REG_INTM_CHG__FIELD_TJ_REG_I (0x08U)                     // Field Mask
#define MAX77654__REG_INTM_CHG__FIELD_TJ_REG_I__CONST_UNMASKED (0x00U)     // Constant
#define MAX77654__REG_INTM_CHG__FIELD_TJ_REG_I__CONST_ENABLED (0x00U)      // Constant
#define MAX77654__REG_INTM_CHG__FIELD_TJ_REG_I__CONST_MASKED (0x01U)       // Constant
#define MAX77654__REG_INTM_CHG__FIELD_TJ_REG_I__CONST_DISABLED (0x01U)     // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHGIN_I (0x04U)                      // Field Mask
#define MAX77654__REG_INTM_CHG__FIELD_CHGIN_I__CONST_UNMASKED (0x00U)      // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHGIN_I__CONST_ENABLED (0x00U)       // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHGIN_I__CONST_MASKED (0x01U)        // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHGIN_I__CONST_DISABLED (0x01U)      // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHG_I (0x02U)                        // Field Mask
#define MAX77654__REG_INTM_CHG__FIELD_CHG_I__CONST_UNMASKED (0x00U)        // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHG_I__CONST_ENABLED (0x00U)         // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHG_I__CONST_MASKED (0x01U)          // Constant
#define MAX77654__REG_INTM_CHG__FIELD_CHG_I__CONST_DISABLED (0x01U)        // Constant
#define MAX77654__REG_INTM_CHG__FIELD_THM_I (0x01U)                        // Field Mask
#define MAX77654__REG_INTM_CHG__FIELD_THM_I__CONST_UNMASKED (0x00U)        // Constant
#define MAX77654__REG_INTM_CHG__FIELD_THM_I__CONST_ENABLED (0x00U)         // Constant
#define MAX77654__REG_INTM_CHG__FIELD_THM_I__CONST_MASKED (0x01U)          // Constant
#define MAX77654__REG_INTM_CHG__FIELD_THM_I__CONST_DISABLED (0x01U)        // Constant

// ==== CNFG_CHG_A =============================================================
// Charger Config register A.
#define MAX77654__REG_CNFG_CHG_A (0x20U)                              // Register Address
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_HOT (0xC0U)               // Field Mask
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_HOT__CONST_0V411 (0x00U)  // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_HOT__CONST_0V367 (0x01U)  // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_HOT__CONST_0V327 (0x02U)  // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_HOT__CONST_0V291 (0x03U)  // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_WARM (0x30U)              // Field Mask
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_WARM__CONST_0V511 (0x00U) // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_WARM__CONST_0V459 (0x01U) // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_WARM__CONST_0V411 (0x02U) // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_WARM__CONST_0V367 (0x03U) // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_COOL (0x0CU)              // Field Mask
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_COOL__CONST_0V923 (0x00U) // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_COOL__CONST_0V867 (0x01U) // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_COOL__CONST_0V807 (0x02U) // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_COOL__CONST_0V747 (0x03U) // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_COLD (0x03U)              // Field Mask
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_COLD__CONST_1V024 (0x00U) // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_COLD__CONST_0V976 (0x01U) // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_COLD__CONST_0V923 (0x02U) // Constant
#define MAX77654__REG_CNFG_CHG_A__FIELD_THM_COLD__CONST_0V867 (0x03U) // Constant

// ==== CNFG_CHG_B =============================================================
// Charger Config register B.
#define MAX77654__REG_CNFG_CHG_B (0x21U)                                // Register Address
#define MAX77654__REG_CNFG_CHG_B__FIELD_VCHGIN_MIN (0xE0U)              // Field Mask
#define MAX77654__REG_CNFG_CHG_B__FIELD_VCHGIN_MIN__CONST_4V0 (0x00U)   // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_VCHGIN_MIN__CONST_4V1 (0x01U)   // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_VCHGIN_MIN__CONST_4V2 (0x02U)   // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_VCHGIN_MIN__CONST_4V3 (0x03U)   // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_VCHGIN_MIN__CONST_4V4 (0x04U)   // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_VCHGIN_MIN__CONST_4V5 (0x05U)   // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_VCHGIN_MIN__CONST_4V6 (0x06U)   // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_VCHGIN_MIN__CONST_4V7 (0x07U)   // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_ICHGIN_LIM (0x1CU)              // Field Mask
#define MAX77654__REG_CNFG_CHG_B__FIELD_ICHGIN_LIM__CONST_95mA (0x00U)  // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_ICHGIN_LIM__CONST_190mA (0x01U) // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_ICHGIN_LIM__CONST_285mA (0x02U) // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_ICHGIN_LIM__CONST_380mA (0x03U) // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_ICHGIN_LIM__CONST_475mA (0x04U) // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_I_PQ (0x02U)                    // Field Mask
#define MAX77654__REG_CNFG_CHG_B__FIELD_I_PQ__CONST_10PERC (0x00U)      // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_I_PQ__CONST_20PERC (0x01U)      // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_CHG_EN (0x01U)                  // Field Mask
#define MAX77654__REG_CNFG_CHG_B__FIELD_CHG_EN__CONST_DISABLED (0x00U)  // Constant
#define MAX77654__REG_CNFG_CHG_B__FIELD_CHG_EN__CONST_ENABLED (0x01U)   // Constant

// ==== CNFG_CHG_C =============================================================
// Charger Config register C.
#define MAX77654__REG_CNFG_CHG_C (0x22U)                              // Register Address
#define MAX77654__REG_CNFG_CHG_C__FIELD_CHG_PQ (0xE0U)                // Field Mask
#define MAX77654__REG_CNFG_CHG_C__FIELD_CHG_PQ__CONST_2V3 (0x00U)     // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_CHG_PQ__CONST_2V4 (0x01U)     // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_CHG_PQ__CONST_2V5 (0x02U)     // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_CHG_PQ__CONST_2V6 (0x03U)     // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_CHG_PQ__CONST_2V7 (0x04U)     // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_CHG_PQ__CONST_2V8 (0x05U)     // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_CHG_PQ__CONST_2V9 (0x06U)     // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_CHG_PQ__CONST_3V0 (0x07U)     // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_I_TERM (0x18U)                // Field Mask
#define MAX77654__REG_CNFG_CHG_C__FIELD_I_TERM__CONST_5PERC (0x00U)   // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_I_TERM__CONST_7PERC5 (0x01U)  // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_I_TERM__CONST_10PERC (0x02U)  // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_I_TERM__CONST_15PERC (0x03U)  // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_T_TOPOFF (0x07U)              // Field Mask
#define MAX77654__REG_CNFG_CHG_C__FIELD_T_TOPOFF__CONST_0MIN (0x00U)  // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_T_TOPOFF__CONST_5MIN (0x01U)  // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_T_TOPOFF__CONST_10MIN (0x02U) // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_T_TOPOFF__CONST_15MIN (0x03U) // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_T_TOPOFF__CONST_20MIN (0x04U) // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_T_TOPOFF__CONST_25MIN (0x05U) // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_T_TOPOFF__CONST_30MIN (0x06U) // Constant
#define MAX77654__REG_CNFG_CHG_C__FIELD_T_TOPOFF__CONST_35MIN (0x07U) // Constant

// ==== CNFG_CHG_D =============================================================
// Charger Config register D.
#define MAX77654__REG_CNFG_CHG_D (0x23U)                              // Register Address
#define MAX77654__REG_CNFG_CHG_D__FIELD_TJ_REG (0xE0U)                // Field Mask
#define MAX77654__REG_CNFG_CHG_D__FIELD_TJ_REG__CONST_60degC (0x00U)  // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_TJ_REG__CONST_70degC (0x01U)  // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_TJ_REG__CONST_80degC (0x02U)  // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_TJ_REG__CONST_90degC (0x03U)  // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_TJ_REG__CONST_100degC (0x04U) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG (0x1FU)              // Field Mask
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V1 (0x00U)   // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V125 (0x01U) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V15 (0x02U)  // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V175 (0x03U) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V2 (0x04U)   // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V225 (0x05U) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V25 (0x06U)  // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V275 (0x07U) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V3 (0x08U)   // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V325 (0x09U) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V35 (0x0AU)  // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V375 (0x0BU) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V4 (0x0CU)   // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V425 (0x0DU) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V45 (0x0EU)  // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V475 (0x0FU) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V5 (0x10U)   // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V525 (0x11U) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V55 (0x12U)  // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V575 (0x13U) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V6 (0x14U)   // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V625 (0x15U) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V65 (0x16U)  // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V675 (0x17U) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V7 (0x18U)   // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V725 (0x19U) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V75 (0x1AU)  // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V775 (0x1BU) // Constant
#define MAX77654__REG_CNFG_CHG_D__FIELD_VSYS_REG__CONST_4V8 (0x1CU)   // Constant

// ==== CNFG_CHG_E =============================================================
// Charger Config register E.
#define MAX77654__REG_CNFG_CHG_E (0x24U)                              // Register Address
#define MAX77654__REG_CNFG_CHG_E__FIELD_CHG_CC (0xFCU)                // Field Mask
#define MAX77654__REG_CNFG_CHG_E__FIELD_T_FAST_CHG (0x03U)            // Field Mask
#define MAX77654__REG_CNFG_CHG_E__FIELD_T_FAST_CHG__CONST_OFF (0x00U) // Constant
#define MAX77654__REG_CNFG_CHG_E__FIELD_T_FAST_CHG__CONST_3H (0x01U)  // Constant
#define MAX77654__REG_CNFG_CHG_E__FIELD_T_FAST_CHG__CONST_5H (0x02U)  // Constant
#define MAX77654__REG_CNFG_CHG_E__FIELD_T_FAST_CHG__CONST_7H (0x03U)  // Constant

// ==== CNFG_CHG_F =============================================================
// Charger Config register F.
#define MAX77654__REG_CNFG_CHG_F (0x25U)                               // Register Address
#define MAX77654__REG_CNFG_CHG_F__FIELD_CHG_CC_JEITA (0xFCU)           // Field Mask
#define MAX77654__REG_CNFG_CHG_F__FIELD_THM_EN (0x02U)                 // Field Mask
#define MAX77654__REG_CNFG_CHG_F__FIELD_THM_EN__CONST_DISABLED (0x00U) // Constant
#define MAX77654__REG_CNFG_CHG_F__FIELD_THM_EN__CONST_ENABLED (0x01U)  // Constant

// ==== CNFG_CHG_G =============================================================
// Charger Config register G.
#define MAX77654__REG_CNFG_CHG_G (0x26U)               // Register Address
#define MAX77654__REG_CNFG_CHG_G__FIELD_CHG_CV (0xFCU) // Field Mask
#define MAX77654__REG_CNFG_CHG_G__FIELD_USBS (0x02U)   // Field Mask

// ==== CNFG_CHG_H =============================================================
// Charger Config register H.
#define MAX77654__REG_CNFG_CHG_H (0x27U)                     // Register Address
#define MAX77654__REG_CNFG_CHG_H__FIELD_CHG_CV_JEITA (0xFCU) // Field Mask

// ==== CNFG_CHG_I =============================================================
// Charger Config register I.
#define MAX77654__REG_CNFG_CHG_I (0x28U)                                          // Register Address
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE (0xF0U)                 // Field Mask
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE__CONST_8mA2 (0x00U)     // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE__CONST_40mA5 (0x01U)    // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE__CONST_72mA3 (0x02U)    // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE__CONST_103mA4 (0x03U)   // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE__CONST_134mA1 (0x04U)   // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE__CONST_164mA1 (0x05U)   // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE__CONST_193mA7 (0x06U)   // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE__CONST_222mA7 (0x07U)   // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE__CONST_251mA2 (0x08U)   // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE__CONST_279mA3 (0x09U)   // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_IMON_DISCHG_SCALE__CONST_300mA (0x0AU)    // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL (0x0FU)                           // Field Mask
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL__CONST_DISABLED (0x00U)           // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL__CONST_CHGIN_V (0x01U)            // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL__CONST_CHGIN_I (0x02U)            // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL__CONST_BATT_V (0x03U)             // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL__CONST_BATT_CHG_I (0x04U)         // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL__CONST_BATT_DISCHG_I (0x05U)      // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL__CONST_BATT_DISCHG_I_NULL (0x06U) // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL__CONST_THM (0x07U)                // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL__CONST_THM_BIAS (0x08U)           // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL__CONST_AGND (0x09U)               // Constant
#define MAX77654__REG_CNFG_CHG_I__FIELD_MUX_SEL__CONST_VSYS (0x0AU)               // Constant

// ==== CNFG_SBB0_A ============================================================
// SBB0 Config A.
#define MAX77654__REG_CNFG_SBB0_A (0x29U)               // Register Address
#define MAX77654__REG_CNFG_SBB0_A__FIELD_TV_SBB (0x7FU) // Field Mask

// ==== CNFG_SBB0_B ============================================================
// SBB0 Config B.
#define MAX77654__REG_CNFG_SBB0_B (0x2AU)                                  // Register Address
#define MAX77654__REG_CNFG_SBB0_B__FIELD_OP_MODE (0x40U)                   // Field Mask
#define MAX77654__REG_CNFG_SBB0_B__FIELD_OP_MODE__CONST_BUCK_BOOST (0x00U) // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_OP_MODE__CONST_BUCK (0x01U)       // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_IP_SBB (0x30U)                    // Field Mask
#define MAX77654__REG_CNFG_SBB0_B__FIELD_IP_SBB__CONST_1A (0x00U)          // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_IP_SBB__CONST_0A75 (0x01U)        // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_IP_SBB__CONST_0A5 (0x02U)         // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_IP_SBB__CONST_0A3 (0x03U)         // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_ADE_SBB (0x08U)                   // Field Mask
#define MAX77654__REG_CNFG_SBB0_B__FIELD_ADE_SBB__CONST_DISABLED (0x00U)   // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_ADE_SBB__CONST_ENABLED (0x01U)    // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_EN_SBB (0x07U)                    // Field Mask
#define MAX77654__REG_CNFG_SBB0_B__FIELD_EN_SBB__CONST_FPS_SLOT_0 (0x00U)  // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_EN_SBB__CONST_FPS_SLOT_1 (0x01U)  // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_EN_SBB__CONST_FPS_SLOT_2 (0x02U)  // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_EN_SBB__CONST_FPS_SLOT_3 (0x03U)  // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_EN_SBB__CONST_DISABLED (0x04U)    // Constant
#define MAX77654__REG_CNFG_SBB0_B__FIELD_EN_SBB__CONST_ENABLED (0x06U)     // Constant

// ==== CNFG_SBB1_A ============================================================
// SBB1 Config A.
#define MAX77654__REG_CNFG_SBB1_A (0x2BU)               // Register Address
#define MAX77654__REG_CNFG_SBB1_A__FIELD_TV_SBB (0x7FU) // Field Mask

// ==== CNFG_SBB1_B ============================================================
// SBB1 Config B.
#define MAX77654__REG_CNFG_SBB1_B (0x2CU)                                  // Register Address
#define MAX77654__REG_CNFG_SBB1_B__FIELD_OP_MODE (0x40U)                   // Field Mask
#define MAX77654__REG_CNFG_SBB1_B__FIELD_OP_MODE__CONST_BUCK_BOOST (0x00U) // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_OP_MODE__CONST_BUCK (0x01U)       // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_IP_SBB (0x30U)                    // Field Mask
#define MAX77654__REG_CNFG_SBB1_B__FIELD_IP_SBB__CONST_1A (0x00U)          // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_IP_SBB__CONST_0A75 (0x01U)        // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_IP_SBB__CONST_0A5 (0x02U)         // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_IP_SBB__CONST_0A3 (0x03U)         // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_ADE_SBB (0x08U)                   // Field Mask
#define MAX77654__REG_CNFG_SBB1_B__FIELD_ADE_SBB__CONST_DISABLED (0x00U)   // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_ADE_SBB__CONST_ENABLED (0x01U)    // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_EN_SBB (0x07U)                    // Field Mask
#define MAX77654__REG_CNFG_SBB1_B__FIELD_EN_SBB__CONST_FPS_SLOT_0 (0x00U)  // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_EN_SBB__CONST_FPS_SLOT_1 (0x01U)  // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_EN_SBB__CONST_FPS_SLOT_2 (0x02U)  // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_EN_SBB__CONST_FPS_SLOT_3 (0x03U)  // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_EN_SBB__CONST_DISABLED (0x04U)    // Constant
#define MAX77654__REG_CNFG_SBB1_B__FIELD_EN_SBB__CONST_ENABLED (0x06U)     // Constant

// ==== CNFG_SBB2_A ============================================================
// SBB2 Config A.
#define MAX77654__REG_CNFG_SBB2_A (0x2DU)               // Register Address
#define MAX77654__REG_CNFG_SBB2_A__FIELD_TV_SBB (0x7FU) // Field Mask

// ==== CNFG_SBB2_B ============================================================
// SBB2 Config B.
#define MAX77654__REG_CNFG_SBB2_B (0x2EU)                                  // Register Address
#define MAX77654__REG_CNFG_SBB2_B__FIELD_OP_MODE (0x40U)                   // Field Mask
#define MAX77654__REG_CNFG_SBB2_B__FIELD_OP_MODE__CONST_BUCK_BOOST (0x00U) // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_OP_MODE__CONST_BUCK (0x01U)       // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_IP_SBB (0x30U)                    // Field Mask
#define MAX77654__REG_CNFG_SBB2_B__FIELD_IP_SBB__CONST_1A (0x00U)          // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_IP_SBB__CONST_0A75 (0x01U)        // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_IP_SBB__CONST_0A5 (0x02U)         // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_IP_SBB__CONST_0A3 (0x03U)         // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_ADE_SBB (0x08U)                   // Field Mask
#define MAX77654__REG_CNFG_SBB2_B__FIELD_ADE_SBB__CONST_DISABLED (0x00U)   // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_ADE_SBB__CONST_ENABLED (0x01U)    // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_EN_SBB (0x07U)                    // Field Mask
#define MAX77654__REG_CNFG_SBB2_B__FIELD_EN_SBB__CONST_FPS_SLOT_0 (0x00U)  // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_EN_SBB__CONST_FPS_SLOT_1 (0x01U)  // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_EN_SBB__CONST_FPS_SLOT_2 (0x02U)  // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_EN_SBB__CONST_FPS_SLOT_3 (0x03U)  // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_EN_SBB__CONST_DISABLED (0x04U)    // Constant
#define MAX77654__REG_CNFG_SBB2_B__FIELD_EN_SBB__CONST_ENABLED (0x06U)     // Constant

// ==== CNFG_SBB_TOP ===========================================================
// SBB Top Config.
#define MAX77654__REG_CNFG_SBB_TOP (0x2FU)                               // Register Address
#define MAX77654__REG_CNFG_SBB_TOP__FIELD_ICHGIN_LIM_DEF (0x80U)         // Field Mask
#define MAX77654__REG_CNFG_SBB_TOP__FIELD_DRV_SBB (0x03U)                // Field Mask
#define MAX77654__REG_CNFG_SBB_TOP__FIELD_DRV_SBB__CONST_FASTEST (0x00U) // Constant
#define MAX77654__REG_CNFG_SBB_TOP__FIELD_DRV_SBB__CONST_FAST (0x01U)    // Constant
#define MAX77654__REG_CNFG_SBB_TOP__FIELD_DRV_SBB__CONST_SLOW (0x02U)    // Constant
#define MAX77654__REG_CNFG_SBB_TOP__FIELD_DRV_SBB__CONST_SLOWEST (0x03U) // Constant

// ==== CNFG_LDO0_A ============================================================
// LDO0 Config A.
#define MAX77654__REG_CNFG_LDO0_A (0x38U)               // Register Address
#define MAX77654__REG_CNFG_LDO0_A__FIELD_TV_LDO (0x7FU) // Field Mask

// ==== CNFG_LDO0_B ============================================================
// LDO0 Config B.
#define MAX77654__REG_CNFG_LDO0_B (0x39U)                                 // Register Address
#define MAX77654__REG_CNFG_LDO0_B__FIELD_OP_MODE (0x10U)                  // Field Mask
#define MAX77654__REG_CNFG_LDO0_B__FIELD_OP_MODE__CONST_LDO (0x00U)       // Constant
#define MAX77654__REG_CNFG_LDO0_B__FIELD_OP_MODE__CONST_SW (0x01U)        // Constant
#define MAX77654__REG_CNFG_LDO0_B__FIELD_ADE_LDO (0x08U)                  // Field Mask
#define MAX77654__REG_CNFG_LDO0_B__FIELD_ADE_LDO__CONST_DISABLED (0x00U)  // Constant
#define MAX77654__REG_CNFG_LDO0_B__FIELD_ADE_LDO__CONST_ENABLED (0x01U)   // Constant
#define MAX77654__REG_CNFG_LDO0_B__FIELD_EN_LDO (0x07U)                   // Field Mask
#define MAX77654__REG_CNFG_LDO0_B__FIELD_EN_LDO__CONST_FPS_SLOT_0 (0x00U) // Constant
#define MAX77654__REG_CNFG_LDO0_B__FIELD_EN_LDO__CONST_FPS_SLOT_1 (0x01U) // Constant
#define MAX77654__REG_CNFG_LDO0_B__FIELD_EN_LDO__CONST_FPS_SLOT_2 (0x02U) // Constant
#define MAX77654__REG_CNFG_LDO0_B__FIELD_EN_LDO__CONST_FPS_SLOT_3 (0x03U) // Constant
#define MAX77654__REG_CNFG_LDO0_B__FIELD_EN_LDO__CONST_DISABLED (0x04U)   // Constant
#define MAX77654__REG_CNFG_LDO0_B__FIELD_EN_LDO__CONST_ENABLED (0x06U)    // Constant

// ==== CNFG_LDO1_A ============================================================
// LDO1 Config A.
#define MAX77654__REG_CNFG_LDO1_A (0x3AU)               // Register Address
#define MAX77654__REG_CNFG_LDO1_A__FIELD_TV_LDO (0x7FU) // Field Mask

// ==== CNFG_LDO1_B ============================================================
// LDO0 Config B.
#define MAX77654__REG_CNFG_LDO1_B (0x3BU)                                 // Register Address
#define MAX77654__REG_CNFG_LDO1_B__FIELD_OP_MODE (0x10U)                  // Field Mask
#define MAX77654__REG_CNFG_LDO1_B__FIELD_OP_MODE__CONST_LDO (0x00U)       // Constant
#define MAX77654__REG_CNFG_LDO1_B__FIELD_OP_MODE__CONST_SW (0x01U)        // Constant
#define MAX77654__REG_CNFG_LDO1_B__FIELD_ADE_LDO (0x08U)                  // Field Mask
#define MAX77654__REG_CNFG_LDO1_B__FIELD_ADE_LDO__CONST_DISABLED (0x00U)  // Constant
#define MAX77654__REG_CNFG_LDO1_B__FIELD_ADE_LDO__CONST_ENABLED (0x01U)   // Constant
#define MAX77654__REG_CNFG_LDO1_B__FIELD_EN_LDO (0x07U)                   // Field Mask
#define MAX77654__REG_CNFG_LDO1_B__FIELD_EN_LDO__CONST_FPS_SLOT_0 (0x00U) // Constant
#define MAX77654__REG_CNFG_LDO1_B__FIELD_EN_LDO__CONST_FPS_SLOT_1 (0x01U) // Constant
#define MAX77654__REG_CNFG_LDO1_B__FIELD_EN_LDO__CONST_FPS_SLOT_2 (0x02U) // Constant
#define MAX77654__REG_CNFG_LDO1_B__FIELD_EN_LDO__CONST_FPS_SLOT_3 (0x03U) // Constant
#define MAX77654__REG_CNFG_LDO1_B__FIELD_EN_LDO__CONST_DISABLED (0x04U)   // Constant
#define MAX77654__REG_CNFG_LDO1_B__FIELD_EN_LDO__CONST_ENABLED (0x06U)    // Constant

#endif /* MAX77654_REG_H_ */
