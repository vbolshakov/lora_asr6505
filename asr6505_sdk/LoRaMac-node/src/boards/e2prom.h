#ifndef __E2PROM_H
#define __E2PROM_H
#include "stm8l15x.h"

extern uint8_t e2prom_read_byte;

void e2prom_byte_rxtx(uint32_t addr, uint8_t c);

#endif
