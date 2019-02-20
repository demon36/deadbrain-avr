#ifndef DSP_H_
#define DSP_H_

#include <avr/eeprom.h>
#include <usbdrv/usbdrv.h>
#include "adc.h"
#include "uart.h"
#include "sys_ex_defs.h"

extern uint8_t debug_ch;
extern deadbrain_config settings;

void save_settings();
void load_settings();
void dsp_send_hit();
void dsp_process_sample_2();
void dsp_send_pedal_status();
void dsp_process_hihat_pedal_sample();

#endif /* DSP_H_ */
