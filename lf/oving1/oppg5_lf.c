#include <avr/io.h>
#define F_CPU 4000000UL
#include "util/delay.h"

const int LED_PIN = 0b00001000;
const int BTN_PIN = 0b00000100;

int main(void)
{    
	PORTB.DIRSET = LED_PIN; // Sett pinne 3 p� port B til utgang.
	PORTB.DIRCLR = BTN_PIN; // Sett pinne 2 p� port B til inngang.
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm;
	
	while(1) // Evig l�kke som g�r
	{
		PORTB.OUT = (PORTB.IN << 1);
	}
}