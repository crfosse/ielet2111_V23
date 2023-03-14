/*
 * ielet2111_timer_led_blink.c
 *
 * Created: 24.02.2023 11:38:55
 * Author : crfosse
 */ 

#define F_CPU 40000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

void TCA0_init(void);
void PORT_init(void);

void TCA0_init(void) {

	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV1024_gc; /*Sysclk/1024*/
	
	TCA0.SINGLE.PERBUF = 0x0F42;
}

void TCA0_set_period_ms(int new_period_ms) {
	TCA0.SINGLE.PERBUF = F_CPU*(1/new_period_ms)/1024); /*F_CPU*F_IRQ/TCA_prescaler*/
}

void PORT_init(void) {
	PORTB.DIR |= PIN3_bm;
	
}

ISR(TCA0_OVF_vect) {
	PORTB.OUT ^= PIN3_bm;
	
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}


int main(void)
{
	PORT_init();
	TCA0_init();
	
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		;
    }
}

