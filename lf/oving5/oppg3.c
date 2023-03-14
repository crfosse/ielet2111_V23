/*
 * oppg1.c
 *
 * Created: 07/02/2023 11:38:34
 * Author : troch
 */ 

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void ADC0_init(void);
void ADC0_start(void);
long map(long x, long in_min, long in_max, long out_min, long out_max);

/**
 * @brief Maps a value from one range to another
 * 
 * @param x value to map
 * @param in_min  minimum value of input range
 * @param in_max  maximum value of input range
 * @param out_min minimum value of output range
 * @param out_max maximum value of output range
 * @return long mapped value
 */
long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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
	ADC0.MUXPOS = ADC_MUXPOS_AIN8_gc; //PE0

	ADC0.INTCTRL = ADC_RESRDY_bm;
	
	/* Set ADC to freerun */
	ADC0.CTRLA |= ADC_FREERUN_bm;
}

void ADC0_start(void)
{
	/* Start conversion */
	ADC0.COMMAND = ADC_STCONV_bm;
}

/**
 * @brief Interrupt service routine for ADC0
 * reads the ADC value and maps it to 0-255
 * and writes it to PORTD
 */
ISR(ADC0_RESRDY_vect){
	uint16_t adcVal = ADC0.RES;
	uint8_t mapedVal = map(adcVal,0,1024,0,255);
	PORTD.OUT = mapedVal;
	ADC0.INTFLAGS = ADC_RESRDY_bm;
}

int main(void)
{
	PORTD.DIRSET = 0xFF;
    // Run initialization functions
	ADC0_init();
	ADC0_start();
	sei();

    PORTB.DIRSET = PIN5_bm;
	
    while (1) 
    {
		PORTB.OUTTGL = PIN5_bm
		_delay_ms(500);
    }
}

