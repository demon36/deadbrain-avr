#include <avr/eeprom.h>
#include "adc.h"
#include "uart.h"
#include "midi.h"
#include "dsp.h"

#define true 1
#define false 0

#define PEDAL_CLOSED 1
#define PEDAL_OPEN	0
#define PEDAL_CH 8

//extern uchar sendEmptyFrame;

uint8_t debug_ch = NUM_CHANNELS;
DeadBrainConfig settings = {{0}};

uint8_t last_sample[NUM_CHANNELS] = {0};
uint8_t hit_started[NUM_CHANNELS] = {0};
unsigned int retrigger_ctd[NUM_CHANNELS] = {0};

uint8_t hihat_pedal_threshold = 0xB0;
uint8_t hihat_pedal_state = PEDAL_OPEN;

uint8_t p2_max[NUM_CHANNELS] = {0};
uint8_t p2_i[NUM_CHANNELS] = {0};
//uint8_t p2_win_size = 12;

void dsp_save_settings(){
	eeprom_write_block(settings, (uint8_t*)0, sizeof(DeadBrainConfig));
}

void dsp_load_settings(){
	eeprom_read_block(settings, (uint8_t*)0, sizeof(DeadBrainConfig));
}


void dsp_process_sample_2(){
	if (settings[current_channel].enabled){
		if(retrigger_ctd[current_channel] == 0){
			if( (current_sample > settings[current_channel].threshold) || (hit_started[current_channel] == true)){
				hit_started[current_channel] = true;
				if( current_sample >  p2_max[current_channel]){
					p2_max[current_channel] = current_sample;

				}
				p2_i[current_channel] += 1;
				if( p2_i[current_channel] > settings[current_channel].scan_time ){
//					send_byte(settings[current_channel][SCAN_TIME]);
//					last_sample[current_channel] = p2_max[current_channel] * settings[current_channel][GAIN];
//					send_byte(p2_max[current_channel]);
					last_sample[current_channel] = p2_max[current_channel];
					p2_max[current_channel] = 0;
					retrigger_ctd[current_channel] = settings[current_channel].retrigger * 10;
					p2_i[current_channel] = 0;
					hit_started[current_channel] = false;
					midi_qpush_note_msg(settings[current_channel].note, last_sample[current_channel] >> 1);
				}
			}
		}else{
			retrigger_ctd[current_channel]--;
		}
	}
}

void dsp_send_pedal_status(){
/*
	//TODO:uncomment later
	uint8_t msize = 4;
	if (usbInterruptIsReady()){
		midiMsg[0] = 0x09;
		midiMsg[1] = 0xB0; //CC message
		midiMsg[2] = 0x04; //controller number
	if (hihat_pedal_state == PEDAL_CLOSED){
		midiMsg[3] = 0x00; //pedal closed cc msg
		midiMsg[4] = 0x09;
		midiMsg[5] = 0x90;
		midiMsg[6] = 0x07;
		midiMsg[7] = 15; //velocity
		msize = 8;

		retrigger_ctd[PEDAL_CH] = settings[PEDAL_CH].retrigger * 10;
	}
	else{
		midiMsg[3] = 0x7F; //pedal open cc msg
		retrigger_ctd[PEDAL_CH] = settings[PEDAL_CH].retrigger * 20;
	}

	sendEmptyFrame = 0;
	usbSetInterrupt(midiMsg, msize);
	}
*/
}

void dsp_process_hihat_pedal_sample(){
	if ((current_sample == 0x00) && (last_sample[current_channel] == 0x00)){

		if(hihat_pedal_state == PEDAL_OPEN){
			hihat_pedal_state = PEDAL_CLOSED;
			dsp_send_pedal_status();
		}
	}
	else if((current_sample == 0xFF) && (last_sample[current_channel] == 0xFF)){
		if(hihat_pedal_state == PEDAL_CLOSED){
			hihat_pedal_state = PEDAL_OPEN;
			dsp_send_pedal_status();
		}
	}

	last_sample[current_channel] = current_sample;
}
