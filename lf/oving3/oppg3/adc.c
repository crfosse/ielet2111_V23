/*
 * adc.c
 *
 * Created: 07.03.2023
 *  Author: trondfc
 */ 
#include <avr/io.h>
#include <stdbool.h>

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

	/* Set */
	ADC0.CTRLA |= ADC_FREERUN_bm;
}

void ADC0_start(void)
{
	/* Start conversion */
	ADC0.COMMAND = ADC_STCONV_bm;
}
bool ADC0_conersionDone(void)
{
	return (ADC0.INTFLAGS & ADC_RESRDY_bm);
}
uint16_t ADC0_read(void)
{
	/* Clear the interrupt flag by writing 1: */
	ADC0.INTFLAGS = ADC_RESRDY_bm;

	return ADC0.RES;
}