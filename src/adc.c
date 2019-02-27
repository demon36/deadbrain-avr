#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <string.h>

#include "adc.h"
#include "midi.h"
#include "dsp.h"

#define SAMPLE_QUEUE_SIZE	48
#define NULL_SAMPLE			0xFF

uint8_t current_channel = 0;
uint8_t current_sample = NULL_SAMPLE;

volatile uint8_t adc_rot_q[SAMPLE_QUEUE_SIZE];
volatile uint8_t* adc_q_head = adc_rot_q;
volatile uint8_t* adc_q_tail = adc_rot_q;

volatile uint8_t bufferIndex = 0;
uint8_t toProcessSampleIndex = 0;
volatile uint8_t goodSample = 1;
volatile uint8_t prodRoundID = 0;
volatile uint8_t consRoundID = 0;

uint8_t adc_q_is_end(volatile uint8_t* ptr){
	if(ptr == &adc_rot_q[SAMPLE_QUEUE_SIZE-1]){
		return 1;
	}else{
		return 0;
	}
}

uint8_t adc_qpush_sample(volatile uint8_t value){
	if(*adc_q_head != NULL_SAMPLE){
		return 0;
	}

	*adc_q_head = value;

	if(adc_q_is_end(adc_q_head)){
		adc_q_head = adc_rot_q;
	}else{
		++adc_q_head;
	}

	return 1;
}

uint8_t adc_qpop_sample(){
	if(*adc_q_tail == NULL_SAMPLE){
		return NULL_SAMPLE;
	}

	uint8_t temp_sample = *adc_q_tail;

	*adc_q_tail = NULL_SAMPLE;

	if(adc_q_is_end(adc_q_tail)){
		adc_q_tail = adc_rot_q;
	}else{
		++adc_q_tail;
	}

	return temp_sample;
}

uint8_t adc_q_get_num_avail_samples(){
	if(adc_q_head >= adc_q_tail){
		return adc_q_head - adc_q_tail;
	}else{
		return SAMPLE_QUEUE_SIZE - (adc_q_tail - adc_q_head);
	}
}

void adc_init() {
	for(uint8_t i = 0; i < SAMPLE_QUEUE_SIZE; i++){
		adc_rot_q[i] = NULL_SAMPLE;
	}

	ADMUX  = 	0b11100000;
	//ADCSRA = 	0b11100110;
	ADCSRB =    0b00000000;
	ADCSRA =	(1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(0<<ADPS1)|(0<<ADPS0);
	ADCSRA |=	(1<<ADSC);
	DIDR0 =		0b00111111;
	PORTB = 0x00;
}

ISR(ADC_vect){
	if(goodSample){
//		adc_rot_q[bufferIndex] = ADCH;

		adc_qpush_sample(ADCH);

		PORTB++;
		if(PORTB == 8)
			PORTB = 0;

		goodSample = 0;
	}else{
		goodSample = 1;
	}
}

void get_sample(){
	//wait for buffer to be filled
//	while(adc_q_get_num_avail_samples() == 0);

	current_sample = adc_qpop_sample();

//	while(!(ADCSRA & (1<<ADIF)));	//wait for conversion completion
//	current_sample =  ADCH;
//	ADCSRA |= (1<<ADIF);	//clear completion flag
//
//	PORTB = (PORTB + 1) % 8;	//rotate multiplexer
//
//	while(!(ADCSRA & (1<<ADIF)));
//	ADCSRA |= (1<<ADIF);	//clear completion flag

}

void get_pedal_value(){
	if(bit_is_set(PINB,5))
		current_sample = 0xFF;
	else
		current_sample = 0x00;
}

void dsp_enable_debugging(uint8_t channel_index){
	debug_ch = channel_index;
}

void dsp_disable_debugging(){
	debug_ch = NUM_CHANNELS;
}

void dsp_queue_debug_sample(uint8_t sample){
}

