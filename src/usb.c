#include <util/delay.h>
#include <usbdrv/usbdrv.h>
#include "usb.h"

uint8_t buffer[8];

uint16_t nNotReady = 0;

uint8_t usb_send_dummy(){
	//will be recieved as F0 00 01 02 F7
	if (usbInterruptIsReady()){
		buffer[0] = 0x04;
		buffer[1] = 0xF0;
		buffer[2] = 0x00;
		buffer[3] = 0x00;
		buffer[4] = 0x07;
		buffer[5] = 0x00;
		buffer[6] = 0x00;
		buffer[7] = 0xF7;
		usbSetInterrupt(buffer, 8);
	}
//	_delay_ms(1);
//
//	if(usbInterruptIsReady()){
//		buffer[0] = 0x04;
//		buffer[1] = 0x00;
//		buffer[2] = 0x00;
//		buffer[3] = 0x00;
//		buffer[4] = 0x07;
//		buffer[5] = 0x00;
//		buffer[6] = 0x00;
//		buffer[7] = 0xF7;
//		usbSetInterrupt(buffer, 8);
//	}
	return 0;
}
