/*
 * ----------------------------------------------------------------------
 *
 * File: wulpus_appl.h
 *
 * Copyright (C) 2026, ETH Zurich
 *
 * Authors:
 * - Sebastian Frey (sefrey@iis.ee.ethz.ch), ETH Zurich
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

/**
 * @file wulpus_appl.h
 * @brief WULPUS ultrasound sensor interface for nRF54LM20
 *
 * Bridges SPI frames from the MSP430 ultrasound controller to the
 * data link, compatible with the original WULPUS protocol. Ported from
 * BioGAP. Pins are defined in the devicetree overlay.
 */

#ifndef WULPUS_APPL_H
#define WULPUS_APPL_H

#include <stdbool.h>
#include <stdint.h>

#define MSP_RESTART_PCK_LEN 105   
/**
 * @brief Initialize the WULPUS sensor interface.
 *
 * Configures the HOST_DATA_RDY GPIO interrupt and the HOST_LINK_RDY
 * output (initially LOW), and checks the SPI device.
 * Spawns the SPI acquisition and BLE forwarding threads.
 * Must be called once during application startup.
 */
void wulpus_init(void);

/**
 * @brief Provide MSP430 configuration and start streaming.
 *
 * Stores @p config as the SPI TX payload (repeated in all 4 SPI
 * transfers per US frame) and asserts HOST_LINK_RDY HIGH to tell
 * the MSP430 the BLE link is ready.  Called either from the BLE
 * receive handler when the WULPUS dongle sends raw config bytes, or
 * from the START_WULPUS_STREAMING command handler.
 *
 * @param config  Config bytes to forward to MSP430 via SPI TX.
 *                May be NULL or len 0 to use a zeroed TX buffer.
 * @param len     Number of bytes (clamped to WULPUS_BYTES_PER_XFER).
 */
void wulpus_set_msp_config(const uint8_t *config, uint16_t len);

/**
 * @brief Stop WULPUS streaming and de-assert HOST_LINK_RDY.
 *
 * Clears the active flag so incoming HOST_DATA_RDY edges are ignored,
 * then drives HOST_LINK_RDY LOW to inform the MSP430.
 */
void wulpus_stop(void);

/**
 * @brief Return true while WULPUS is actively streaming.
 */
bool wulpus_is_streaming(void);

/** @brief Number of frames fetched over SPI. */
uint32_t wulpus_frames(void);

/** @brief Loopback comparison errors, only counted in the loopback test. */
uint32_t wulpus_spi_errors(void);

#endif /* WULPUS_APPL_H */
