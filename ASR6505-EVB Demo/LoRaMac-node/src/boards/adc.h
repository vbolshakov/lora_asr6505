#ifndef __ADC_H
#define __ADC_H
#include "stm8l15x.h"

extern uint16_t adc1_value;
extern float VoltageValue;

void adc_init(void);
void vol_measure(void);

#endif
