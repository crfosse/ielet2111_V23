/*
 * Oppg1.c
 *
 * Created: 01/03/2023 13:41:34
 * Author : troch
 */ 

#define F_CPU 4000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR (PORTB_PORT_vect){
	PORTE.OUTTGL = PIN0_bm | PIN1_bm | PIN2_bm;
	PORTB.INTFLAGS = PIN2_bm ; // Clear interrupt flag to end interrupt request
}
int main ( void )
{
	// initialization
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc ; // Internal pull-up on PB2 and rising edge interupts
	PORTE.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm;
	PORTB.DIRSET = PIN3_bm;
	
	sei(); // enable global interrupts
	while (1)
	{
		_delay_ms(1000);
		PORTB.OUTTGL = PIN3_bm;
	}
}

