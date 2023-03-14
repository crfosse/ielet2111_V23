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
#include "usart.h"
#include "adc.h"

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

