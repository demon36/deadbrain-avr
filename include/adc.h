#ifndef ADC_H_
#define ADC_H_

extern uint8_t current_channel;
extern uint8_t current_sample;

void adc_init();
uint8_t adc_q_get_num_avail_samples();
uint8_t adc_qpop_sample();
void get_sample();
void get_pedal_value();
void dsp_enable_debugging(uint8_t channel_index);
void dsp_disable_debugging();

#endif /* ADC_H_ */
