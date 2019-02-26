#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/boot.h>

#include "uart.h"
#include "timer.h"
#include "adc.h"
#include "dsp.h"
#include "midi.h"
#include "usb.h"

volatile uint8_t should_poll = 1;

void timer_cb(){
	should_poll = 1;
}

int main(void)
{
	//debug led 04
//	PORTB ^= 0x10;
//	_delay_ms(1000);
//	PORTB &= ~0x10;

	wdt_enable(WDTO_1S);
	hardwareInit();
	//odDebugInit();

	usbInit();
//	sendEmptyFrame = 0;
	uart_init();
	timer_init();
	timer_register_cb(timer_cb);
	init_adc();
	dsp_load_settings();
	sei();
	while (1) {
		wdt_reset();

		if(should_poll){
			usbPoll();
			should_poll = 0;
		}
		midi_qpop_packet();

		current_channel = 0;
		while(current_channel < 8){
			get_sample();

			if(debug_ch == current_channel){
				uart_send_byte(current_sample);
			}

			if(debug_ch == NUM_CHANNELS){//debugging off
				dsp_process_sample_2();
			}
			current_channel++;
		}

//		//hihat
//		get_sample();
//		if(debug_ch == current_channel) //watch signal ?
//			send_byte(current_sample);
//		process_sample2();
//		current_channel++;

		//hihat pedal
		get_pedal_value();
		if(debug_ch == current_channel){
			uart_send_byte(current_sample);
		}
		dsp_process_hihat_pedal_sample();
		current_channel++;


//		if(counter_bit == 0){
//			TCCR1B = 0x00 ;
//			send_string("time");
//			send_byte(tot_overflow);
//			send_byte(TCNT1H);
//			send_byte(TCNT1L);
//			counter_bit = 1;
//		}

		//PORTB &= ~0x20;
	}
	return 0;
}
