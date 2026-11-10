#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <stdint.h>

/*
 * Data link to the PC: USB (transport_usb.c) or BLE (transport_ble.c).
 * CONFIG_APP_TRANSPORT_BLE picks one of them in CMakeLists.txt.
 */

/* Same name as in BioGAP ble_appl.h: all sensor data leaves through here. */
void add_data_to_send_buffer(uint8_t *data, uint16_t size);

/* Called by the transport with bytes from the PC (implemented in commands.c). */
void transport_rx_bytes(const uint8_t *data, uint32_t size);

int transport_init(void);
uint32_t transport_dropped_packets(void);

#endif
