#include <avr/io.h>
#include <string.h>
#include "uart.h"

void uart_init(void) {
	UBRR0H = 0x00;
	UBRR0L = 0x0A;	/* targeting 115200 baud rate */

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void uart_send_str(char* data){
	for (int i = 0; i < strlen(data); i++){
		while (( UCSR0A & (1<<UDRE0))  == 0){};
		UDR0 = data[i];
	}
}

void uart_send_byte(char data){
	while (( UCSR0A & (1<<UDRE0))  == 0){};
		UDR0 = data;
}
