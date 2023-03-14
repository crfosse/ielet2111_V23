#define F_CPU 4000000UL
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)

#define MAX_COMMAND_LEN 9
#define NUMBER_OF_COMMANDS 4

#define CR 0x0D	// '\r'
#define LF 0x0A // '\n'

#define LED1_PIN_bm PIN0_bm
#define LED2_PIN_bm PIN1_bm

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

//Prototypes
void USART3_init(void);
void USART3_sendChar(char c);
static int USART3_printChar(char c, FILE *stream);
void USART3_sendString(char *str);
uint8_t USART3_read();
uint8_t compareCommands(char* src);


volatile char command_buf[MAX_COMMAND_LEN];
volatile uint8_t command_index = 0;
uint8_t command_received = 0;

// Setting up FDEV_SETUP_STREAM to use USART3_printChar
static FILE USART_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);

// Array of commands to compare to
static char commands[NUMBER_OF_COMMANDS][MAX_COMMAND_LEN] = {
	"ledon 1",
	"ledon 2",
	"ledoff 1",
	"ledoff 2"
};

// Init function
void USART3_init(void)
{
	PORTB.DIRSET = PIN0_bm; // Set TX pin to output
	PORTB.DIRCLR = PIN1_bm; // Set RX pin to input
	
	PORTD.DIRSET = LED1_PIN_bm | LED2_PIN_bm;

	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);	// Set Baud rate 9600
	USART3.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;
	USART3.CTRLA |= USART_RXCIE_bm;
	
	stdout = &USART_stream;	//Replacing output stream
}

// Function to send char
void USART3_sendChar(char c)
{
	while (!(USART3.STATUS & USART_DREIF_bm)) //while data register not empty
	{
		;
	}
	USART3.TXDATAL = c;
}
// Wrapping USART3_sendChar in a function computable with FDEV_SETUP_STREAM
static int USART3_printChar(char c, FILE *stream)
{
	USART3_sendChar(c);
	return 0;
}

// Function to send string
void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}

uint8_t compareCommands(char* src)
{
	// Return index of command received or 255 if none
	for(uint8_t i = 0; i < NUMBER_OF_COMMANDS; i++){
		if(strcmp(src, commands[i]) == 0){
			return i;
		}
	}
	return 255;
}

// Function to exicute command using switc case
void executeCommand(uint8_t command_number)
{
	switch(command_number)
	{
		case 0:
		PORTE.OUTSET = PIN0_bm;
		printf("ledon 1 \r\n");
		break;
		
		case 1:
		PORTE.OUTSET = PIN1_bm;
		printf("ledon 2 \r\n");
		break;
		
		case 2:
		PORTE.OUTCLR = PIN0_bm;
		printf("ledoff 1 \r\n");
		break;
		
		case 3:
		PORTE.OUTCLR = PIN1_bm;
		printf("ledoff 2 \r\n");
		break;
		
		default:
		printf("unknown command\r\n");
	}
}

ISR(USART3_RXC_vect) {
	uint8_t c = USART3.RXDATAL;
	if(c != LF && c != CR){	// If received \r or \n
		command_buf[command_index++] = c;
		if(command_index > MAX_COMMAND_LEN)
		{
			printf("\r\n");
			command_index = 0;
		}
	}
	if(c == CR){	// if received \r
		command_received = 1;
		
		command_buf[command_index] = 0;
		command_index = 0;
	}
}

int main(void)
{
	USART3_init();
	
	
	PORTE.DIR |= LED1_PIN_bm | LED2_PIN_bm;
	
	/* This delay invalidates the initial noise on the TX line, after device reset. */
	_delay_ms(10);
	
	char current_command[MAX_COMMAND_LEN];
	
	sei();
	
	while (1)
	{
		if (command_received) {
			
			command_received=0;
			cli(); //Disable interrupts while handling the command_buf, in order to avoid race conditions.
			strcpy(current_command, (const char *)command_buf);
			sei(); //Enabling interrupts again.
						
			printf("Received command: ");
			executeCommand(compareCommands(current_command));
		}
	}
}