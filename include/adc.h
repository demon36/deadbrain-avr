#ifndef ADC_H_
#define ADC_H_

#include <avr/interrupt.h>

extern uint8_t current_channel;
extern uint8_t current_sample;

void init_adc();
void get_sample();
void get_pedal_value();

#endif /* ADC_H_ */
