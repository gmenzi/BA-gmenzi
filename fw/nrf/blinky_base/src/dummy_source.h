#ifndef DUMMY_SOURCE_H
#define DUMMY_SOURCE_H

#include <stdint.h>

void dummy_us_start(void);
void dummy_us_stop(void);
void dummy_ppg_start(void);
void dummy_ppg_stop(void);

uint32_t dummy_us_frames(void);
uint32_t dummy_ppg_packets(void);

#endif
