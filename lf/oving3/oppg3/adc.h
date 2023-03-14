/*
 * adc.h
 *
 * Created: 07.03.2023
 *  Author: trondfc
 */ 
#ifndef USART_H_
#define USART_H_

void ADC0_init(void);
void ADC0_start(void);
bool ADC0_conersionDone(void);
uint16_t ADC0_read(void);

#endif