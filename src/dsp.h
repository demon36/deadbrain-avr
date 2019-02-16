/*
 * dsp.h
 *
 *  Created on: Mar 3, 2016
 *      Author: demon
 */

#ifndef DSP_H_
#define DSP_H_

#include <avr/eeprom.h>
#include "adc.h"
#include "usbdrv/usbdrv.h"
#include "uart.h"

#define true 1
#define false 0
#define NO_CHANNELS	16
#define NO_SETTINGS	5
#define ENABLED		0
#define SCAN_TIME	1
#define THRESHOLD	2
#define NOTE		3
#define RETRIGGER	4
#define PEDAL_CLOSED 1
#define PEDAL_OPEN	0
#define PEDAL_CH 8

uint8_t debug_info[6] = {0,0,0,0,0,0};

uint8_t debug_ch = 0x10;
uint8_t settings[NO_CHANNELS][NO_SETTINGS];

//uint8_t enabled[16] = {0, 1, 0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//uint8_t gain[16] = {1,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
//uint8_t slope_threshold[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//uint8_t note[16] = {36, 37, 0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//uint8_t retrigger_time[16] = {10, 5, 10,10,10,10,10,10,10,10,10,10,10,10,10,10};

uint8_t last_sample[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t hit_started[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int retrigger_ctd[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t hihat_pedal_threshold = 0xB0;
uint8_t hihat_pedal_state = PEDAL_OPEN;
uint8_t midiMsg[8];
uint8_t test = 120;

// delayed message
uint8_t delayed_msg_flag = false;
uint8_t delayed_msg_note = 0x00;
uint8_t delayed_msg_velocity = 0x00;

void save_settings(){
	eeprom_write_byte((uint8_t*)0, debug_ch);
	//eeprom_read_block((void*)&StringOfData, (const void*)12, 10);
	eeprom_write_block(settings, (uint8_t*)1, NO_CHANNELS * NO_SETTINGS);
}

void show_eeprom(){
	uint8_t temp[81];
	eeprom_read_block(temp, (uint8_t*)0, NO_CHANNELS * NO_SETTINGS + 1);
	uint8_t i2;
	for(i2 = 0; i2 < 81; i2++)
		send_byte(temp[i2]);
}

void show_settings(){
	uint8_t i,j;
	for(i = 0; i < NO_CHANNELS; i++){
		for(j = 0; j < NO_SETTINGS; j++){
			send_byte(settings[i][j]);
		}
	}
}

void load_settings(){
	eeprom_write_byte((uint8_t*)79, 0x99);
	debug_ch = eeprom_read_byte((uint8_t*)0);
	eeprom_read_block(settings, (uint8_t*)1, NO_CHANNELS * NO_SETTINGS);
}

void send_hit(){
	if (usbInterruptIsReady()){
		midiMsg[0] = 0x09;
		midiMsg[1] = 0x90;
		midiMsg[2] = settings[current_channel][NOTE];
		midiMsg[3] = last_sample[current_channel] >> 1; //velocity
		sendEmptyFrame = 0;
		usbSetInterrupt(midiMsg, 4);
//		debug_info[4] = midiMsg[3];
//
//		uint8_t k;
//		for(k = 0; k < 5; k++)
//			send_byte(debug_info[k]);
	}else{
		delayed_msg_flag = true;
		delayed_msg_note = settings[current_channel][NOTE];
		delayed_msg_velocity = last_sample[current_channel] >> 1; //velocity
//		send_byte(0x99);
	}
}

uint8_t p2_max[NO_CHANNELS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t p2_i[NO_CHANNELS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//uint8_t p2_win_size = 12;
void process_sample2(){
	if (settings[current_channel][ENABLED] == true){
		if(retrigger_ctd[current_channel] == 0){
			if( (current_sample > settings[current_channel][THRESHOLD]) || (hit_started[current_channel] == true)){
				hit_started[current_channel] = true;
				if( current_sample >  p2_max[current_channel]){
					p2_max[current_channel] = current_sample;

				}
				p2_i[current_channel] += 1;
				if( p2_i[current_channel] > settings[current_channel][SCAN_TIME] ){
//					send_byte(settings[current_channel][SCAN_TIME]);
//					last_sample[current_channel] = p2_max[current_channel] * settings[current_channel][GAIN];
//					send_byte(p2_max[current_channel]);
					last_sample[current_channel] = p2_max[current_channel];
					p2_max[current_channel] = 0;
					retrigger_ctd[current_channel] = settings[current_channel][RETRIGGER] * 10;
					p2_i[current_channel] = 0;
					hit_started[current_channel] = false;
					send_hit();
				}
			}
		}else{
			retrigger_ctd[current_channel]--;
		}
	}
	if (delayed_msg_flag){
		if (usbInterruptIsReady()){
			midiMsg[0] = 0x09;
			midiMsg[1] = 0x90;
			midiMsg[2] = delayed_msg_note;
			midiMsg[3] = delayed_msg_velocity;
			sendEmptyFrame = 0;
			usbSetInterrupt(midiMsg, 4);
			delayed_msg_flag = false;
		}
	}
}

void send_pedal_status(){
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
		retrigger_ctd[PEDAL_CH] = settings[PEDAL_CH][RETRIGGER] * 10;
	}
	else{
		midiMsg[3] = 0x7F; //pedal open cc msg
		retrigger_ctd[PEDAL_CH] = settings[PEDAL_CH][RETRIGGER] * 20;
	}

	sendEmptyFrame = 0;
	usbSetInterrupt(midiMsg, msize);
	}
}

void process_hp_sample(){
	if ((current_sample == 0x00) && (last_sample[current_channel] == 0x00)){

		if(hihat_pedal_state == PEDAL_OPEN){
			hihat_pedal_state = PEDAL_CLOSED;
			send_pedal_status();
		}
	}
	else if((current_sample == 0xFF) && (last_sample[current_channel] == 0xFF)){
		if(hihat_pedal_state == PEDAL_CLOSED){
			hihat_pedal_state = PEDAL_OPEN;
			send_pedal_status();
		}
	}

	last_sample[current_channel] = current_sample;
}



#endif /* DSP_H_ */
