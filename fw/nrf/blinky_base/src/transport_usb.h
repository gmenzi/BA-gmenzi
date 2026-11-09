#ifndef TRANSPORT_USB_H
#define TRANSPORT_USB_H

#include <stdint.h>

/* Same name as in BioGAP ble_appl.h: all sensor data leaves through here. */
void add_data_to_send_buffer(uint8_t *data, uint16_t size);

/* Called from the USB RX thread with bytes from the PC. */
void usb_rx_bytes(const uint8_t *data, uint32_t size);

int usb_transport_init(void);
uint32_t usb_dropped_packets(void);

#endif
