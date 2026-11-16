// Copyright (c) 2022 Philip Schilk.
// SPDX-License-Identifier: Apache-2.0

#ifndef MAX77654_UTILS_H_
#define MAX77654_UTILS_H_

#include "max77654.h"

// ======== Register Macros  =================================================================

#define MAX77654_REG(_reg_) MAX77654__REG_##_reg_
#define MAX77654_REG_MASK(_reg_, _field_) MAX77654__REG_##_reg_##__FIELD_##_field_
#define MAX77654_REG_CONST(_reg_, _field_, _const_) MAX77654__REG_##_reg_##__FIELD_##_field_##__CONST_##_const_

// ======== Bit Manipulation =================================================================
#define MAX77654_LSB_GET(_v_) ((_v_) & -(_v_))
#define MAX77654_FIELD_GET(_mask_, _v_) (((_v_) & (_mask_)) / MAX77654_LSB_GET(_mask_))
#define MAX77654_FIELD_ALIGN(_mask_, _v_) (((_v_) * MAX77654_LSB_GET(_mask_)) & (_mask_))
#define MAX77654_FIELD_CLR(_mask_, _v_) ((_v_) & (~(_mask_)))
#define MAX77654_FIELD_SET(_mask_, _v_reg_, _v_field_)                                                                 \
  (MAX77654_FIELD_ALIGN(_mask_, _v_field_) | MAX77654_FIELD_CLR(_mask_, _v_reg_))

// ======== Register Bit Manipulation =================================================================
#define MAX77654_REG_FIELD_GET(_reg_, _field_, _v_) (MAX77654_FIELD_GET(MAX77654_REG_MASK(_reg_, _field_), _v_))
#define MAX77654_REG_FIELD_CLR(_reg_, _field_, _v_) (MAX77654_FIELD_CLR(MAX77654_REG_MASK(_reg_, _field_), _v_))
#define MAX77654_REG_FIELD_SET(_reg_, _field_, _v_reg_, _v_field_)                                                     \
  (MAX77654_FIELD_SET(MAX77654_REG_MASK(_reg_, _field_), _v_reg_, _v_field_))
#define MAX77654_REG_FIELD_SET_CONST(_reg_, _field_, _v_reg_, _const_)                                                 \
  MAX77654_REG_FIELD_SET(_reg_, _field_, _v_reg_, MAX77654_REG_CONST(_reg_, _field_, _const_))

#endif /* MAX77654_UTILS_H_ */
