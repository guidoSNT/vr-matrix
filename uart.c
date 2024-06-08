#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>

/*
* @brief: initiates uart
*/
void USART_init(void){

	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
}

/*
* @brief: sends a byte of data
* @data: byte of data to be send
*/
void USART_send( unsigned char data){

	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;

}

/*
* @brief: receives a byte of data
* @return: the byte of data received stored as a char
*/
unsigned char USART_receive(void){

	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;

}

/*
* @brief: sends and string with uart
* @ stringPtr: string to be send
*/
void USART_putstring(char* StringPtr){

	while(*StringPtr != 0x00){ 
		USART_send(*StringPtr);
	StringPtr++;
	}
}
