/*
 * oppg1.c
 *
 * Created: 07/02/2023 11:38:34
 * Author : troch
 */ 

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


#define USART3_BAUD_RATE(BAUD_RATE)     ((float)(64 * 4000000 / (16 * (float)BAUD_RATE)) + 0.5)

void ADC0_init(void);
void ADC0_start(void);
bool ADC0_conersionDone(void);
uint16_t ADC0_read(void);

void USART3_init(void);
void USART3_sendChar(char c);
static int USART3_printChar(char c, FILE *stream);

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

float calculateTemp(uint16_t ADC_value){
	/* 
	Calculating temperature based on code from 
	circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/ 
	*/
	float R1 = 10000;
	float logR2, R2, Temp, TempC;
	float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
	
	R2 = R1 * (1023.0 / (float)ADC_value - 1.0);
	logR2 = log(R2);
	Temp = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
	TempC = Temp - 273.15;
	
	return TempC;
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

int main(void)
{
    // Run initialization functions
	ADC0_init();
	ADC0_start();

	USART3_init();

    uint8_t i = 0;

    while (1) 
    {
		if (ADC0_conersionDone())
		{
			uint16_t adcVal = ADC0_read(); // Read the ADC value
			//printf("value read is: %d \r\n", adcVal); 
			float temperature = calculateTemp(adcVal); // Calculate temperature

            // Set the LED's based on the temperature
            if(temperature > 30){
                PORTD.OUTSET = PIN7_bm;
                PORTD.OUTSET = PIN6_bm;
                PORTD.OUTSET = PIN5_bm;
            } else if (temperature > 20){
                PORTD.OUTSET = PIN7_bm;
                PORTD.OUTSET = PIN6_bm;
                PORTD.OUTCLR = PIN5_bm;
            } else if (temperature > 10){
                PORTD.OUTSET = PIN7_bm;
                PORTD.OUTCLR = PIN6_bm;
                PORTD.OUTCLR = PIN5_bm;
            } else {
                PORTD.OUTCLR = PIN7_bm;
                PORTD.OUTCLR = PIN6_bm;
                PORTD.OUTCLR = PIN5_bm;
            }

            i++; //loop counter
            // Print the temperature every 100th loop
            if(i == 100){
                printf("Temperature is: %f \r\n", temperature);
                i = 0;
            }
		}
    }
}

