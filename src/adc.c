#include <avr/io.h>
#include <stdint.h>

#include "adc.h"

#define ROT_BUF_SIZE 48
uint8_t current_channel;
uint8_t current_sample;

volatile uint8_t rotBuffer[ROT_BUF_SIZE];
volatile uint8_t bufferIndex = 0;
uint8_t toProcessSampleIndex = 0;
volatile uint8_t goodSample = 1;
volatile uint8_t prodRoundID = 0;
volatile uint8_t consRoundID = 0;

void init_adc(void) {
	for(int i = 0; i < ROT_BUF_SIZE; i++){
		rotBuffer[i++] = 0x00;
	}

	ADMUX  = 	0b11100000;
	//ADCSRA = 	0b11100110;
	ADCSRB =    0b00000000;
	ADCSRA =	(1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(0<<ADPS1)|(0<<ADPS0);
	ADCSRA |=	(1<<ADSC);
	//DIDR0 =		0b00111111;
	PORTB = 0x00;
}

ISR(ADC_vect){
	if(goodSample){
		rotBuffer[bufferIndex] = ADCH;

		bufferIndex++;
		if(bufferIndex == ROT_BUF_SIZE){
			bufferIndex = 0;
			prodRoundID = ~prodRoundID;
		}

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
	while(toProcessSampleIndex >= bufferIndex && consRoundID == prodRoundID);

	current_sample = rotBuffer[toProcessSampleIndex++];
//	send_byte(current_sample);
	if(toProcessSampleIndex == ROT_BUF_SIZE){
		toProcessSampleIndex = 0;
		consRoundID = ~consRoundID;
	}

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
