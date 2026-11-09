#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

int commands_init(void);

/* Same name as in BioGAP command_dispatcher.h. */
void handle_connectivity_command(const uint8_t *data, uint16_t size);

#endif
