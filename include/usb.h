#ifndef USB_H_
#define USB_H_

uint8_t usb_send_dummy();
uchar usbFunctionRead(uchar * data, uchar len);
uchar usbFunctionWrite(uchar * data, uchar len);
void hardwareInit();

#endif /* USB_H_ */
