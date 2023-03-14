#define PWM_PERIOD (10000)

#define DUTY_CYCLE_EXAMPLE_VALUE (750)

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "usart.h"

void TCA0_init(void);
void PORT_init(void);

uint16_t adcVal;

void ADC0_init(void);
uint16_t ADC0_read(void);
void ADC0_start(void);
bool ADC0_conversionDone(void);

void TCA0_init(void)
{
	/* set waveform output on PORT A */
	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTD_gc;
	
	TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm /* enable compare
	channel 0 */
	| TCA_SINGLE_WGMODE_DSBOTTOM_gc; /* set dual-slope PWM
	mode */
	
	/* disable event counting */
	TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTAEI_bm);
	
	/* set PWM frequency and duty cycle (50%) */
	TCA0.SINGLE.PERBUF = PWM_PERIOD;
	//TCA0.SINGLE.CMP0BUF = DUTY_CYCLE_EXAMPLE_VALUE;
	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc /* set clock source
	(sys_clk/4) */
	| TCA_SINGLE_ENABLE_bm; /* start timer */
}

void ADC0_init(void)
{
	VREF.ADC0REF = VREF_REFSEL_VDD_gc; /*VDD as reference */
	
	/* Disable digital input buffer */
	PORTD.PIN6CTRL &= ~PORT_ISC_gm;
	PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	
	/* Disable pull-up resistor */
	PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;
	
	ADC0.CTRLC = ADC_PRESC_DIV4_gc;		/* CLK_PER divided by 4 */
	
	ADC0.CTRLA = ADC_ENABLE_bm			/* ADC Enable: enabled */
	| ADC_RESSEL_10BIT_gc	/* 10-bit mode */
	| ADC_FREERUN_bm; /* FreeRun mode */
	
	
	
	/* Select ADC channel */
	ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc;
}

uint16_t ADC0_read(void)
{
	/* Clear the interrupt flag by writing 1: */
	ADC0.INTFLAGS = ADC_RESRDY_bm;
	return ADC0.RES;
}

void ADC0_start(void)
{
	/* Start conversion */
	ADC0.COMMAND = ADC_STCONV_bm;
}

bool ADC0_conversionDone(void)
{
	return (ADC0.INTFLAGS & ADC_RESRDY_bm);
}

void PORT_init(void)
{
	/* set pin 0 of PORT A as output */
	PORTD.DIR |= PIN0_bm;
}
int main(void)
{
	ADC0_init();
	ADC0_start();
	
	USART3_init();
	
	PORT_init();
	
	TCA0_init();
	
	//TCA0.SINGLE.CMP0BUF=250; //0 degree
	
	int PWMval = 750; // 1.5 ms
	int PWMmax = 1250; // 2.5 ms
	int PWMmin = 250; //0.5 ms
	
	int ADCmax = 1023;
	int ADCmin = 0;
	
	int cnt = 0;
	
	/* Replace with your application code */
	while (1)
	{
		if (ADC0_conversionDone()) {
			adcVal = ADC0_read();
			PWMval = adcVal + PWMmin;
			TCA0.SINGLE.CMP0BUF=PWMval; //0 degree
		}
		
		if (cnt==10000) {
			printf("Curr val: %d\n",adcVal);
			printf("Curr pwmVal: %d\n",PWMval);
			cnt=0;
		}
		
		cnt++;
	}	
}