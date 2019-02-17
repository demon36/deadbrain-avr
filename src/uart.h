#ifndef UART_H_
#define UART_H_

void uart_init();
void uart_send_str(char* data);
void uart_send_byte(char data);

#endif /* UART_H_ */
