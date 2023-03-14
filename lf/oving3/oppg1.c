/*
 * ielet2111_adc_single_conversion.c
 *
 * Created: 05.02.2023 21:00:58
 * Author : crfosse
 */ 

/* RTC Period */
#define RTC_PERIOD (511)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>


#define F_CPU                           (4000000UL)         /* using default clock 4MHz*/
#define USART3_BAUD_RATE(BAUD_RATE)     ((float)(64 * 4000000 / (16 * (float)BAUD_RATE)) + 0.5)


uint16_t adcVal;

void ADC0_init(void);

uint16_t ADC0_read(void);

void USART3_init(void);
void USART3_sendChar(char c);
static int USART3_printChar(char c, FILE *stream);
void USART3_sendString(char *str);

void ADC0_init(void)
{
	/* Disable digital input buffer */
	PORTD.PIN6CTRL &= ~PORT_ISC_gm;
	PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	
	/* Disable pull-up resistor */
	PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;
	
	ADC0.CTRLC = ADC_PRESC_DIV4_gc;		/* CLK_PER divided by 4 */
	
	ADC0.CTRLA = ADC_ENABLE_bm			/* ADC Enable: enabled */
				| ADC_RESSEL_10BIT_gc;	/* 10-bit mode */
				
	VREF.ADC0REF = VREF_REFSEL_VDD_gc; /*VDD as reference */
	
	/* Select ADC channel */
	ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc;
}

uint16_t ADC0_read(void)
{
	/* Start ADC conversion */
	ADC0.COMMAND = ADC_STCONV_bm;
	
	/* Wait until ADC conversion done */
	while ( !(ADC0.INTFLAGS & ADC_RESRDY_bm) )
	{
		;
	}
	/* Clear the interrupt flag by writing 1: */
	ADC0.INTFLAGS = ADC_RESRDY_bm;
	return ADC0.RES;
}


// Defining USART_stream
static FILE USART_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);

void USART3_init(void)
{
	USART3.BAUD = (uint16_t)(USART3_BAUD_RATE(9600));   /* set the baud rate*/
	PORTB.DIRSET = PIN0_bm; //TX-pin
	PORTB.PIN0CTRL = PORT_PULLUPEN_bm; //Recommended
	
	//PORTB.DIRCLR |= PIN1_bm; //RX-pin
	
	//USART3.CTRLC = USART_CHSIZE_0_bm | USART_CHSIZE_1_bm; // Set the data format to 8-bit
	
	//USART3.CTRLB |= (1 << USART_TXEN_bp) | (1 << USART_RXEN_bp); // enable transmitter
	USART3.CTRLB |= USART_TXEN_bm;                      /* enable transmitter*/
	
	stdout = &USART_stream; // replace the standard output stream with the user-defined stream
}

void USART3_sendChar(char c)
{
	while(!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	
	USART3.TXDATAL = c;
}

static int USART3_printChar(char c, FILE *stream)	// Wrapping USART3_sendChar in a function computable with FDEV_SETUP_STREAM
{
	USART3_sendChar(c);
	return 0;
}

ISR(USART3_RXC_vect) {
	
	
}

int main(void)
{
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
	PORTB.DIRCLR = PIN2_bm;
	PORTB.DIRSET = PIN3_bm;
	
	ADC0_init();
	USART3_init();
	
	while (1)
	{
		
		if(!(PORTB.IN & PIN2_bm)) {
			adcVal = ADC0_read();
			PORTB.OUTCLR = PIN3_bm;
			printf("ADC value is: %d \n", adcVal);
			
			
		} else {
			PORTB.OUTSET = PIN3_bm;
		}
		
	}
}

