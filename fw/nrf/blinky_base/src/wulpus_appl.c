/*
 * ----------------------------------------------------------------------
 *
 * File: wulpus_appl.c
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
 * @file wulpus_appl.c
 * @brief WULPUS ultrasound sensor interface – nRF54LM20 / Zephyr port
 *
 * Ported from BioGAP Firmware/src_NRF/sensors/wulpus/wulpus_appl.c
 * (pulp-bio/BioGAP main @ 3cfb671) to NCS v3.4.2 and the nRF54LM20A.
 * 
 * Changes: 
 * - Zephyr SPI driver instead of nrfx (in a later step consider 
 * also not using general API, but like this it is easier to transfer it 
 * to own board. The general API is easier to get right), 
 * - ESP/Wi-Fi path removed (not needed),
 * - rail control removed, (my PMIC has other rails. So same control wouldnt work) => adapted in M8
 * - frames only forwarded after a config package, and a
 * DATA_READY level check when the link goes high.
 *
 * It preserves the full BLE / SPI packet protocol
 * so the existing WULPUS receiver dongle works without modification.
 *
 * Protocol summary:
 *   - MSP430 signals a new US frame by asserting HOST_DATA_RDY (rising edge).
 *   - nRF5340 performs 4 × 201-byte full-duplex SPI transfers over SPI_B.
 *   - The received frame (4 × 201 bytes) is forwarded via BLE NUS as four
 *     211-byte notifications, each = 7-byte header/counter/timestamp prefix
 *     + one 201-byte SPI chunk + 3 reserved bytes (see WULPUS_META_* and the
 *     BLE forwarding thread).
 *   - Ring buffer holds up to WULPUS_MAX_FRAMES complete US frames.
 *   - The same TX buffer (MSP430 configuration) is used for all 4 SPI TXs.
 *
 * SPI and control pins are defined in the devicetree overlay.
 */

#include "wulpus_appl.h"
#include "transport.h"

#include <string.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(wulpus, LOG_LEVEL_INF);

/*==============================================================================
 * Protocol constants – must stay in sync with old nRF52 firmware
 *============================================================================*/

/** @brief Bytes per SPI transfer (TX and RX) */
#define WULPUS_BYTES_PER_XFER   201

/** @brief BLE packet headers for the four transfer chunks of one US frame */
#define WULPUS_BLE_HDR_XFER_0   0x10
#define WULPUS_BLE_HDR_XFER_1   0x11
#define WULPUS_BLE_HDR_XFER_2   0x12
#define WULPUS_BLE_HDR_XFER_3   0x13

/** @brief SPI transfers per US frame */
#define WULPUS_NUMBER_OF_XFERS  4

/** @brief Maximum number of buffered US frames */
#define WULPUS_MAX_FRAMES       35

/*==============================================================================
 * Device tree node references (GPIOs defined in the project overlay)
 *============================================================================*/

#define WULPUS_DATA_RDY_NODE  DT_NODELABEL(data_ready)
#define WULPUS_LINK_RDY_NODE  DT_NODELABEL(link_ready)
#define WULPUS_SPI_NODE       DT_NODELABEL(msp430)

/* MSP430 SPI slave: speed is set in the overlay, mode 1 (CPOL=0, CPHA=1). */
#define WULPUS_SPI_OPERATION  (SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | \
                               SPI_TRANSFER_MSB | SPI_MODE_CPHA)

/*==============================================================================
 * Buffer type (identical layout to old ArrayList_type)
 *============================================================================*/

typedef struct {
    uint8_t buffer[WULPUS_BYTES_PER_XFER];
} wulpus_xfer_buf_t;

/*==============================================================================
 * Ring buffer
 *
 * Laid out as a flat array of (WULPUS_NUMBER_OF_XFERS * WULPUS_MAX_FRAMES)
 * contiguous slots.  Frame N occupies slots [N*4 .. N*4+3].
 *
 * Each slot's 201 SPI bytes are copied into one BLE packet at offset
 * WULPUS_SPI_OFF (byte 7), behind the header/counter/timestamp prefix.
 *============================================================================*/
static wulpus_xfer_buf_t m_rx_buf[WULPUS_NUMBER_OF_XFERS * WULPUS_MAX_FRAMES];

/** @brief Per-frame acquisition timestamp (µs), captured in the SPI thread when
 *  the frame is received, so the ring-buffer queueing delay does not skew it. */
static uint32_t m_frame_ts[WULPUS_MAX_FRAMES];

/** @brief SPI TX buffer – holds MSP430 configuration, repeated for all 4 TXs */
static uint8_t m_tx_buf[WULPUS_BYTES_PER_XFER];

/*==============================================================================
 * Ring buffer indices
 *   buffer_counter  – next write slot  (incremented by SPI thread)
 *   current_buffer  – next read slot   (incremented by BLE thread)
 *   buffer_content  – frames available
 *============================================================================*/
static volatile int buffer_counter = 0;
static volatile int current_buffer = 0;
static volatile int buffer_content = 0;

/*
 * Per-frame BLE packet counter, written into each frame's padding tail
 * (mirrors the EEG/EMG packet counter). Reset in wulpus_set_msp_config()
 * whenever a new config/restart is received, so each streaming session
 * starts from 0 like the ExG counter.
 */
static uint16_t wulpus_frame_counter = 0;

/*==============================================================================
 * State flags
 *============================================================================*/
static volatile bool msp_conf_received = false;
static volatile bool wulpus_active     = false;

/*
 * Frames are only forwarded after a 0xFA config package. A 0xFB restart needs
 * SPI transfers to reach the MSP430, but those frames are not sensor data.
 */
static volatile bool wulpus_forward    = false;

static atomic_t wulpus_frame_total;
static atomic_t wulpus_loopback_errors;

/*
 * Write position for BLE-fragmented MSP430 config assembly.
 * The WULPUS PRO dongle sends a 105-byte config split as two BLE NUS
 * notifications (64 + 41 bytes) because its nRF52 NUS MTU is 64 bytes.
 * A new config always starts with byte 0xFA (250) or 0xFB (251).
 * Continuation fragments start at other byte values and are appended.
 */
static uint16_t m_tx_write_pos = 0;

/*==============================================================================
 * SPI device
 *============================================================================*/
static const struct spi_dt_spec wulpus_spi =
    SPI_DT_SPEC_GET(WULPUS_SPI_NODE, WULPUS_SPI_OPERATION);

/*==============================================================================
 * Synchronization
 *============================================================================*/

/** @brief Given by GPIO ISR when HOST_DATA_RDY rises */
K_SEM_DEFINE(wulpus_data_rdy_sem, 0, 1);

/** @brief Given by SPI thread when a frame is ready to forward over BLE */
K_SEM_DEFINE(wulpus_ble_ready_sem, 0, WULPUS_MAX_FRAMES);

/*==============================================================================
 * GPIO
 *============================================================================*/
static const struct gpio_dt_spec host_data_rdy =
    GPIO_DT_SPEC_GET(WULPUS_DATA_RDY_NODE, gpios);

static const struct gpio_dt_spec host_link_rdy =
    GPIO_DT_SPEC_GET(WULPUS_LINK_RDY_NODE, gpios);

static struct gpio_callback data_rdy_cb_data;

/*==============================================================================
 * GPIO callback – HOST_DATA_RDY rising edge
 * Signals the SPI acquisition thread that the MSP430 has a new US frame.
 *============================================================================*/
static void data_rdy_gpio_cb(const struct device *dev,
                             struct gpio_callback *cb,
                             uint32_t pins)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(cb);
    ARG_UNUSED(pins);

    if (wulpus_active) {
        k_sem_give(&wulpus_data_rdy_sem);
    }
}

/*==============================================================================
 * SPI acquisition thread
 *
 * Waits for HOST_DATA_RDY, then performs 4 consecutive 201-byte SPI
 * transfers.  Each transfer fills the next slot of the ring buffer.
 * On completion the BLE forwarding thread is signalled.
 *============================================================================*/
#define WULPUS_SPI_STACK_SIZE  2048
#define WULPUS_SPI_PRIORITY    4

static void wulpus_spi_thread(void *a, void *b, void *c)
{
    ARG_UNUSED(a); ARG_UNUSED(b); ARG_UNUSED(c);

    LOG_INF("WULPUS SPI thread started");

    while (1) {
        k_sem_take(&wulpus_data_rdy_sem, K_FOREVER);

        /* Timestamp frame acquisition here (≈ first sample), not at BLE-forward
         * time, so the ring-buffer queueing delay doesn't skew it. Stored for
         * the slot being filled; the BLE thread reads it back per frame. */
        m_frame_ts[buffer_counter] = k_cyc_to_us_floor32(k_cycle_get_32());

        for (int i = 0; i < WULPUS_NUMBER_OF_XFERS; i++) {
            uint8_t *rx = m_rx_buf[buffer_counter * WULPUS_NUMBER_OF_XFERS + i].buffer;

            const struct spi_buf tx_buf = { .buf = m_tx_buf, .len = WULPUS_BYTES_PER_XFER };
            const struct spi_buf_set tx = { .buffers = &tx_buf, .count = 1 };
            const struct spi_buf rx_buf = { .buf = rx, .len = WULPUS_BYTES_PER_XFER };
            const struct spi_buf_set rx_set = { .buffers = &rx_buf, .count = 1 };

            int err = spi_transceive_dt(&wulpus_spi, &tx, &rx_set); 
            if (err != 0) {
                LOG_ERR("SPI xfer %d failed: %d", i, err);
                continue;
            }

#if defined(WULPUS_LOOPBACK_TEST)
            if (memcmp(m_tx_buf, rx, WULPUS_BYTES_PER_XFER) != 0) {
                atomic_inc(&wulpus_loopback_errors);
            }
#endif
        }

        atomic_inc(&wulpus_frame_total);

        if (!wulpus_forward) {
            continue;
        }

        /* Frame complete – advance write pointer */
        buffer_content++;
        if (buffer_content > WULPUS_MAX_FRAMES - 1) {
            LOG_WRN("WULPUS ring buffer overflow");
            buffer_content = WULPUS_MAX_FRAMES - 1;
        }

        buffer_counter++;
        if (buffer_counter == WULPUS_MAX_FRAMES) {
            buffer_counter = 0;
        }
        //LOG_INF("WULPUS SPI done, give BLE ready sem");
        k_sem_give(&wulpus_ble_ready_sem);
    }
}

K_THREAD_DEFINE(wulpus_spi_tid, WULPUS_SPI_STACK_SIZE,
                wulpus_spi_thread, NULL, NULL, NULL,
                WULPUS_SPI_PRIORITY, 0, 0);

/*==============================================================================
 * BLE forwarding thread
 *
 * Drains the ring buffer and enqueues 4 BLE NUS notifications per frame.
 * Each BLE packet is framed with a 7-byte header/counter/timestamp prefix
 * (see WULPUS_META_* below), then one 201-byte SPI transfer chunk, then a
 * 3-byte reserved tail:
 *   packet 0 : 0x10 + 6 meta + 201 payload + 3 reserved = 211 bytes total
 *   packet 1 : 0x11 + 6 meta + 201 payload + 3 reserved = 211 bytes total
 *   packet 2 : 0x12 + 6 meta + 201 payload + 3 reserved = 211 bytes total
 *   packet 3 : 0x13 + 6 meta + 201 payload + 3 reserved = 211 bytes total
 *
 * The per-frame counter+timestamp prefix is mirrored into all four chunks so
 * the receiver can read it from any chunk, and matches the EEG/EMG/MIC/PPG
 * packet layout.
 *
 * 211 bytes matches EEG_PCKT_SIZE so the biogui can use a single
 * packetSize=211 for both WULPUS and EEG/EMG on the same stream.
 *============================================================================*/
#define WULPUS_BLE_STACK_SIZE  2048
#define WULPUS_BLE_PRIORITY    5
/** Standardized BLE packet size – must equal EEG_PCKT_SIZE (211). */
#define WULPUS_BLE_PKT_SIZE  (WULPUS_BYTES_PER_XFER + 1U + 9U)

/*
 * Per-frame metadata written at the FRONT of every chunk, mirrored into all
 * four chunks of a frame so the receiver can read it from any chunk. This
 * matches the EEG/EMG/MIC/PPG packet layout (header, counter, timestamp):
 *   [0]       chunk header (0x10..0x13)
 *   [1:3]     frame counter (uint16 LE)
 *   [3:7]     timestamp us  (uint32 LE)
 *   [7:208]   201-byte SPI payload chunk
 *   [208:211] reserved (zero)
 */
#define WULPUS_META_CNT_OFF  1U
#define WULPUS_META_TS_OFF   3U
#define WULPUS_SPI_OFF       7U

static void wulpus_ble_thread(void *a, void *b, void *c)
{
    ARG_UNUSED(a); ARG_UNUSED(b); ARG_UNUSED(c);

    //LOG_INF("WULPUS BLE thread started");

    uint8_t ble_packet[WULPUS_BLE_PKT_SIZE];
    /* Zero once. The header [0], metadata [1..6] and SPI payload [7..207] are
     * rewritten per chunk/frame below; the reserved bytes [208..210] stay zero. */
    memset(ble_packet, 0, WULPUS_BLE_PKT_SIZE);

    while (1) {
        k_sem_take(&wulpus_ble_ready_sem, K_FOREVER); 

        //LOG_INF("WULPUS BLE ready, processing frames");
        while (current_buffer != buffer_counter) {
            int base = current_buffer * WULPUS_NUMBER_OF_XFERS;

            /* Per-frame metadata at the front of every chunk (mirrored), matching
             * the ExG/MIC/PPG layout: counter (u16 LE) + microsecond timestamp
             * (u32 LE). These persist across the four sends because memcpy below
             * only rewrites the SPI payload region [7:208] and the header [0].
             * The timestamp was captured at frame acquisition (SPI thread). */
            uint32_t ts_us = m_frame_ts[current_buffer];
            ble_packet[WULPUS_META_CNT_OFF]     = (uint8_t)(wulpus_frame_counter);
            ble_packet[WULPUS_META_CNT_OFF + 1] = (uint8_t)(wulpus_frame_counter >> 8);
            //LOG_INF("WULPUS frame %d, timestamp %u us", wulpus_frame_counter, ts_us);
            ble_packet[WULPUS_META_TS_OFF]      = (uint8_t)(ts_us);
            ble_packet[WULPUS_META_TS_OFF + 1]  = (uint8_t)(ts_us >> 8);
            ble_packet[WULPUS_META_TS_OFF + 2]  = (uint8_t)(ts_us >> 16);
            ble_packet[WULPUS_META_TS_OFF + 3]  = (uint8_t)(ts_us >> 24);
            wulpus_frame_counter++;

            ble_packet[0] = WULPUS_BLE_HDR_XFER_0;
            memcpy(&ble_packet[WULPUS_SPI_OFF], m_rx_buf[base + 0].buffer, WULPUS_BYTES_PER_XFER);


                        
            add_data_to_send_buffer(ble_packet, WULPUS_BLE_PKT_SIZE);

            

            ble_packet[0] = WULPUS_BLE_HDR_XFER_1;
            memcpy(&ble_packet[WULPUS_SPI_OFF], m_rx_buf[base + 1].buffer, WULPUS_BYTES_PER_XFER);

            add_data_to_send_buffer(ble_packet, WULPUS_BLE_PKT_SIZE);

            ble_packet[0] = WULPUS_BLE_HDR_XFER_2;
            memcpy(&ble_packet[WULPUS_SPI_OFF], m_rx_buf[base + 2].buffer, WULPUS_BYTES_PER_XFER);

            add_data_to_send_buffer(ble_packet, WULPUS_BLE_PKT_SIZE);


            ble_packet[0] = WULPUS_BLE_HDR_XFER_3;
            memcpy(&ble_packet[WULPUS_SPI_OFF], m_rx_buf[base + 3].buffer, WULPUS_BYTES_PER_XFER);

            add_data_to_send_buffer(ble_packet, WULPUS_BLE_PKT_SIZE);


            buffer_content--;
            current_buffer++;
            if (current_buffer == WULPUS_MAX_FRAMES) {
                current_buffer = 0;
            }
        }
    }
}


K_THREAD_DEFINE(wulpus_ble_tid, WULPUS_BLE_STACK_SIZE,
                wulpus_ble_thread, NULL, NULL, NULL,
                WULPUS_BLE_PRIORITY, 0, 0);

/*==============================================================================
 * Public API
 *============================================================================*/

void wulpus_init(void)
{
    /* --- HOST_LINK_RDY output – initially LOW --- */
    if (!device_is_ready(host_link_rdy.port)) {
        LOG_ERR("HOST_LINK_RDY GPIO port not ready");
        return;
    }
    if (gpio_pin_configure_dt(&host_link_rdy, GPIO_OUTPUT_INACTIVE) < 0) {
        LOG_ERR("HOST_LINK_RDY configure failed");
        return;
    }

    /* --- HOST_DATA_RDY input + rising-edge interrupt --- */
    if (!device_is_ready(host_data_rdy.port)) {
        LOG_ERR("HOST_DATA_RDY GPIO port not ready");
        return;
    }
    if (gpio_pin_configure_dt(&host_data_rdy, GPIO_INPUT) < 0) {
        LOG_ERR("HOST_DATA_RDY configure failed");
        return;
    }
    if (gpio_pin_interrupt_configure_dt(&host_data_rdy,
                                        GPIO_INT_EDGE_TO_ACTIVE) < 0) {
        LOG_ERR("HOST_DATA_RDY interrupt configure failed");
        return;
    }
    gpio_init_callback(&data_rdy_cb_data, data_rdy_gpio_cb,
                       BIT(host_data_rdy.pin));
    if (gpio_add_callback(host_data_rdy.port, &data_rdy_cb_data) < 0) {
        LOG_ERR("HOST_DATA_RDY callback add failed");
        return;
    }

    if (!spi_is_ready_dt(&wulpus_spi)) {
        LOG_ERR("SPI device not ready");
        return;
    }

    LOG_INF("WULPUS initialized – HOST_LINK_RDY LOW, waiting for MSP430 config");

    /* Race-condition guard: if a BLE config packet arrived before wulpus_init()
     * ran (possible when the host connects quickly and sends the combined
     * EEG+WULPUS start sequence before the 5-second boot delay completes),
     * wulpus_set_msp_config() will have set wulpus_active=true and asserted
     * HOST_LINK_RDY HIGH – but gpio_pin_configure_dt() above just reset it LOW.
     * Re-assert here now that SPI and the GPIO ISR are fully set up. */
    if (wulpus_active) {
        gpio_pin_set_dt(&host_link_rdy, 1);
        LOG_INF("WULPUS: pre-init config detected – HOST_LINK_RDY re-asserted");
    }
}


void wulpus_set_msp_config(const uint8_t *config, uint16_t len)
{
    if (config != NULL && len > 0) {
        /* 0xFA (250) = new config, 0xFB (251) = restart — start a fresh assembly */
        LOG_INF("WULPUS wulpus_set_msp_config recv %u bytes - first byte is %d, last byte is %d", len, config[0], config[len - 1]);
        if (config[0] == 250 || config[0] == 251) {
            m_tx_write_pos = 0;
            wulpus_frame_counter = 0;   /* new streaming session: reset counter */
            wulpus_forward = (config[0] == 250);
        }
        uint16_t copy_len = MIN(len, (uint16_t)(WULPUS_BYTES_PER_XFER - m_tx_write_pos));
        memcpy(m_tx_buf + m_tx_write_pos, config, copy_len);
        m_tx_write_pos += copy_len;
    }

    /* A BLE-fragmented package (e.g. a 105-byte MSP_RESTART_PCK_LEN package
     * split across two ~64-byte NUS writes) reaches here once per fragment.
     * Signalling the MSP430 on a partial fragment would expose it to a
     * still-incomplete/stale m_tx_buf before the rest of the package
     * arrives, so wait for a complete package before doing anything the
     * MSP430 can observe. */
    if (m_tx_write_pos < MSP_RESTART_PCK_LEN) {
        LOG_INF("WULPUS config fragment (%u bytes, tx_buf filled %u/%u) – awaiting rest of package",
                (unsigned)len, (unsigned)m_tx_write_pos, (unsigned)MSP_RESTART_PCK_LEN);
        return;
    }

    /* Reset ring buffer */
    buffer_counter = 0;
    current_buffer = 0;
    buffer_content = 0;

    msp_conf_received = true;
    wulpus_active     = true;

    /* Assert HOST_LINK_RDY → MSP430 will start sending US frames */
    gpio_pin_set_dt(&host_link_rdy, 1);

    /* If DATA_READY rose while the link was down, that edge was ignored and
     * the MSP430 is still waiting for its SPI transfer. */
    if (gpio_pin_get_dt(&host_data_rdy) == 1) {
        k_sem_give(&wulpus_data_rdy_sem);
    }

    LOG_INF("WULPUS config package complete (tx_buf filled %u/%u) – HOST_LINK_RDY HIGH",
            (unsigned)m_tx_write_pos, (unsigned)WULPUS_BYTES_PER_XFER);
}

void wulpus_stop(void)
{
    wulpus_active     = false;
    wulpus_forward    = false;
    msp_conf_received = false;

    gpio_pin_set_dt(&host_link_rdy, 0);

    LOG_INF("WULPUS stopped – HOST_LINK_RDY LOW");
}

bool wulpus_is_streaming(void)
{
    return wulpus_active;
}

uint32_t wulpus_frames(void)
{
    return atomic_get(&wulpus_frame_total);
}

uint32_t wulpus_spi_errors(void)
{
    return atomic_get(&wulpus_loopback_errors);
}
