#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/boot.h>
#include <avr/interrupt.h>

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
	wdt_enable(WDTO_1S);
	hardwareInit();
	usbInit();
	adc_init();
	timer_register_cb(timer_cb);
	dsp_load_settings();
	timer_init();
	sei();
	while(1){
		wdt_reset();

		if(should_poll){
			usbPoll();
			midi_qpush_sysex_single_packet(adc_q_get_num_avail_samples(), current_channel, 0, 0);
			should_poll = 0;
		}
		midi_qpop_packet();

		while(adc_q_get_num_avail_samples() > 0){
			get_sample();

			if(debug_ch == NUM_CHANNELS){//debugging off
				dsp_process_sample_2();
			}else if(debug_ch == current_channel){
//				uart_send_byte(current_sample);
			}

			current_channel++;
			if(current_channel == 8){
				current_channel = 0;
			}
		}

		continue;

//		//hihat
//		get_sample();
//		if(debug_ch == current_channel) //watch signal ?
//			send_byte(current_sample);
//		process_sample2();
//		current_channel++;

		//hihat pedal
		get_pedal_value();
		if(debug_ch == current_channel){
//			uart_send_byte(current_sample);
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
