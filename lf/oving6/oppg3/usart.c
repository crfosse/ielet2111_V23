#define F_CPU 4000000UL // changed from 3333333
#include "usart.h"
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)

#include <avr/io.h>
#include <string.h>
#include <stdio.h>

// Prototypes
void USART3_init(void);
void USART3_sendChar(char c);
static int USART3_printChar(char c, FILE *stream);
void USART3_sendString(char *str);

// Defining USART_stream
static FILE USART_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);

void USART3_init(void)
{
	PORTB.DIRSET = PIN0_bm; //TX-pin as  output, changed from PORTC.DIR |= PIN0_bm;
	PORTB.DIRCLR = PIN1_bm; //RX-pin as  input, changed from PORTC.DIR &= ~PIN1_bm;

	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	USART3.CTRLB |= USART_TXEN_bm;
	
	stdout = &USART_stream; // replace the standard output stream with the user-defined stream

}

void USART3_sendChar(char c)
{
	while (!(USART3.STATUS & USART_DREIF_bm))	//wait while USART3 Data Register not empty
	{
		;
	}
	USART3.TXDATAL = c;	// Send c
}

static int USART3_printChar(char c, FILE *stream)	// Wrapping USART3_sendChar in a function computable with FDEV_SETUP_STREAM
{
	USART3_sendChar(c);
	return 0;
}


void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)	//Send each cahr in str using USART3_sendString()
	{
		USART3_sendChar(str[i]);
	}
}