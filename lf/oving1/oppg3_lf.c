#include <avr/io.h>
#define F_CPU 4000000UL
#include "util/delay.h"

const int builtin_led_mask = PIN3_bm;

int main(void)
{    
	PORTB.DIRSET = builtin_led_mask; // Sett pinnene p� port B til utganger    
	
	while(1) // Evig l�kke som g�r    
	{       
		PORTB.OUTSET = builtin_led_mask;
		_delay_ms(1000);
		PORTB.OUTCLR = builtin_led_mask;
		_delay_ms(1000);

		//Alternativt:
		/*
		PORTB.OUT |= 1 << 0b0001000;
		_delay_ms(1000);
		PORTB.OUT &= ~(1 << 0b0001000);
		_delay_ms(1000);
		*/

	}
}
