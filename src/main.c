
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
#include "uart.h"
#include "timer.h"
#include "adc.h"
#include "descriptors.h"
#include "dsp.h"

/*---------------------------------------------------------------------------*/
/* usbFunctionRead                                                           */
/*---------------------------------------------------------------------------*/

uchar usbFunctionRead(uchar * data, uchar len)
{
	// DEBUG LED
	//PORTB = 0x20;

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	data[5] = 0;
	data[6] = 0;

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
/* usbFunctionWriteOut                                                       */
/*                                                                           */
/* this Function is called if a MIDI Out message (from PC) arrives.          */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void usbFunctionWriteOut(uchar * data, uchar len)
{
	//PORTB ^= 0x20;
	//send_byte(0xff);
	//uint8_t i;
	//send_byte(len);
	//for(i = 0; i < len ; i++)
	//	send_byte(data[i]);

	if(len == 4){
		save_settings();
	}else if(data[2] == 0x32){
		debug_ch = data[3];
		//PORTB = data[3];
	}else{
		settings[ data[2] ][ data[3] ] = data[5];
		show_settings();
	}

	//send_byte(0xff);
	//_delay_ms(10);
	//PORTB &= ~0x20;
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

unsigned char counter_bit = 1;
ISR(TIMER1_OVF_vect)
{
	tot_overflow++;
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
	init_uart();
//	init_timer1();
	init_adc();
	load_settings();
	send_string("start");
	sei();
	//load_settings();
	//_delay_ms(5);
	//show_settings();
	while (1) {		/* main event loop */
		wdt_reset();
		usbPoll();
		//send_byte(0xff);
//		if(counter_bit == 0)
//			TCCR1B = 0x05 ;

		current_channel = 0;
		while(current_channel < 8){
			get_sample();

			if(debug_ch == current_channel) //watch signal ?
				send_byte(current_sample);
			if(debug_ch == 0x10)
				process_sample2();
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
		if(debug_ch == current_channel)
			send_byte(current_sample);
		process_hp_sample();
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
