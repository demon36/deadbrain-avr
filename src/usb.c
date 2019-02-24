#include <util/delay.h>
#include <usbdrv/usbdrv.h>
#include "descriptors.h"
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

void hardwareInit()
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
