/*
 * uart.h
 *
 *  Created on: Feb 28, 2016
 *      Author: demon
 */

#ifndef UART_H_
#define UART_H_
#include <avr/io.h>

void init_uart(void) {
	UBRR0H = 0x00;
	UBRR0L = 0x0A;	/* targeting 115200 baud rate */

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void send_string(char* data){
	int i;
	//if (strlen(data) == )
	for (i = 0; i < strlen(data); i++){
		while (( UCSR0A & (1<<UDRE0))  == 0){};
		UDR0 = data[i];
	}
}

void send_byte(uint8_t data){
	while (( UCSR0A & (1<<UDRE0))  == 0){};
		UDR0 = data;
}

#endif /* UART_H_ */
