#ifndef DSP_H_
#define DSP_H_

#include "sys_ex_defs.h"

extern uint8_t debug_ch;
extern DeadBrainConfig settings;

void dsp_save_settings();
void dsp_load_settings();
void dsp_process_sample_2();
void dsp_send_pedal_status();
void dsp_process_hihat_pedal_sample();

#endif /* DSP_H_ */
