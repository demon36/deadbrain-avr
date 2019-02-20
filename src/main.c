
/* Name: main.c
 * Project: V-USB MIDI device on Low-Speed USB
 * Author: Martin Homuth-Rosemann
 * Creation Date: 2008-03-11
 * Copyright: (c) 2008 by Martin Homuth-Rosemann.
 * License: GPL.
 *
 */

#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/boot.h>

#include <usbdrv/usbdrv.h>
#include <usbdrv/oddebug.h>
#include "descriptors.h"
#include "uart.h"
#include "timer.h"
#include "adc.h"
#include "dsp.h"
#include "midi.h"
#include "usb.h"

/*---------------------------------------------------------------------------*/
/* usbFunctionRead                                                           */
/*---------------------------------------------------------------------------*/

uchar usbFunctionRead(uchar * data, uchar len)
{
	// DEBUG LED
	//PORTB = 0x20;
	return 0;
}

/*---------------------------------------------------------------------------*/
/* usbFunctionWrite                                                          */
/*---------------------------------------------------------------------------*/

uchar usbFunctionWrite(uchar * data, uchar len)
{
	// DEBUG LED
	return 1;
}

/*---------------------------------------------------------------------------*/
/* hardwareInit                                                              */
/*---------------------------------------------------------------------------*/

static void hardwareInit(void)
{
	uchar i, j;

	/* activate pull-ups except on USB lines */
	USB_CFG_IOPORT =
	    (uchar) ~ ((1 << USB_CFG_DMINUS_BIT) |
		       (1 << USB_CFG_DPLUS_BIT));
	/* all pins input except USB (-> USB reset) */
#ifdef USB_CFG_PULLUP_IOPORT	/* use usbDeviceConnect()/usbDeviceDisconnect() if available */
	USBDDR = 0;		/* we do RESET by deactivating pullup */
	usbDeviceDisconnect();
#else
	USBDDR = (1 << USB_CFG_DMINUS_BIT) | (1 << USB_CFG_DPLUS_BIT);
#endif

	j = 0;
	while (--j) {		/* USB Reset by device only required on Watchdog Reset */
		i = 0;
		while (--i);	/* delay >10ms for USB reset */
	}
#ifdef USB_CFG_PULLUP_IOPORT
	usbDeviceConnect();
#else
	USBDDR = 0;		/*  remove USB reset condition */
#endif

	DDRB = 0b00011111;
}

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
	sendEmptyFrame = 0;
	uart_init();
	timer_init();
	timer_register_cb(timer_cb);
	init_adc();
	load_settings();
	uart_send_str("start");
	sei();
	//load_settings();
	//_delay_ms(5);
	//show_settings();
	while (1) {		/* main event loop */
		wdt_reset();

		if(should_poll){
			usbPoll();
//			midi_qpush_note_msg(4, 40);
//			midi_release_qpacket();
//			usb_send_dummy();
			should_poll = 0;
		}

		debug_ch = 0xFF;
		//send_byte(0xff);
//		if(counter_bit == 0)
//			TCCR1B = 0x05 ;

		current_channel = 0;
		while(current_channel < 8){
			get_sample();

			if(debug_ch == current_channel){ //watch signal ?
				uart_send_byte(current_sample);
//				midi_qpush_sysex_single_packet(current_sample, 0, 0, 0);
			}

			if(debug_ch == 0x10){
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
		midi_qpop_packet();
	}
	return 0;
}
