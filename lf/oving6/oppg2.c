
#define F_CPU 4000000UL

#define PERIOD_EXAMPLE_VALUE (0x01A0)
#define DUTY_CYCLE_EXAMPLE_VALUE (0x00D0)
#include <avr/io.h>
#include <util/delay.h>
void TCA0_init(void);
void PORT_init(void);

void TCA0_init(void)
{
	/* set waveform output on PORT A */
	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTD_gc;
	
	TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm /* enable compare
	channel 0 */
	
	| TCA_SINGLE_WGMODE_DSBOTTOM_gc; /* set dual-slope PWM
	mode */
	
	
	/* set PWM frequency and duty cycle (50%) */
	TCA0.SINGLE.PERBUF = PERIOD_EXAMPLE_VALUE;
	TCA0.SINGLE.CMP0BUF = 0;
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc /* set clock source
	(sys_clk/4) */
	| TCA_SINGLE_ENABLE_bm; /* start timer */
}
void PORT_init(void)
{
	/* set pin 0 of PORT A as output */
	PORTD.DIR |= PIN0_bm;
}
int main(void)
{
	PORT_init();
	TCA0_init();
	
	int current_ds = 0;
	int ds_cnt_dir = 1;
	
	
	/* Replace with your application code */
	while (1)
	{
		if(current_ds == PERIOD_EXAMPLE_VALUE) {
			ds_cnt_dir = 0;
			} else if (current_ds == 0) {
			ds_cnt_dir = 1;
		}
		
		if(ds_cnt_dir) {
			current_ds++;
		} else {
			current_ds--;
		}
		
		TCA0.SINGLE.CMP0BUF = current_ds;
		_delay_ms(5);
	}
}