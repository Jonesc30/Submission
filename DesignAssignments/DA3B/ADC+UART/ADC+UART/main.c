/*
 * ADC+UART.c
 *
 * Created: 10/20/2019 9:26:30 PM
 * Author : Cody Jones
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void USART_init(void); //function to initialize USART
unsigned char USART_receive(void); //function to receive through USART
void USART_transmit(unsigned char data); //function to send through USART
void USART_putstring(char* StringPtr); //function to scan through the string
void ADC_init (void); //function to initialize ADC

volatile unsigned int ADC_temp;
char outs[20];

uint8_t OVF_COUNT = 0; //initialize the overflow count for interrupt
uint8_t OVF_LIMIT = 250; //set the limit the count can reach to set 1 sec delay

int main(void)
{
	USART_init(); //initialize USART
	ADC_init (); //initialize ADC
	USART_putstring("Connected\r\n");
	
	
	TCCR0A = 0x00; //normal operation
	TCCR0B |= (1 << CS02); //set prescalar to 256
	TCNT0 = 16; //TOP = 256-240 = 16
	TIMSK0 |= (1 << TOIE0);
	sei(); //enable interrupt
	
	while (1)
	{ //sit here waiting for interrupt
	}
}

void USART_init(void)
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}

unsigned char USART_receive(void)
{
	while(!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void USART_transmit(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void USART_putstring(char* StringPtr)
{
	while(*StringPtr != 0x00)
	{
		USART_transmit(*StringPtr);
		StringPtr++;
	}
}

ISR (TIMER0_OVF_vect)
{
	OVF_COUNT++; //increment the overflow counter
	if (OVF_COUNT == OVF_LIMIT) //check to see if the limit was reached
	{
		ADCSRA|=(1<<ADSC);	//start conversion
		while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
		
		ADCSRA |= (1<<ADIF);

		int a = ADCL;
		a = a | (ADCH<<8);
		a = (a/1024.0) * 5000/10;
		a = (a*2)+32; //equation to convert celsius to farenheit. can't use used 2 instead of 9/5
		a = a % 100;
		USART_transmit((a/10)+'0');
		a = a % 10;
		USART_transmit((a)+'0');
		USART_transmit('\n');
		OVF_COUNT = 0; //reset overflow counter
	}
	TCNT0 = 16; //reset TOP
}

void ADC_init (void)
{
	ADMUX = (0<<REFS1)|					// reference selection bits
	(1<<REFS0)|							// AVcc - external cap at AREF
	(0<<ADLAR)|							// ADC Left Adjust Result
	(1<<MUX2)|							// Analog Channel Selection Bits
	(0<<MUX1)|							// ADC5 (PC5)
	(0<<MUX0);
	ADCSRA = (1<<ADEN)|					// ADC Enable
	(0<<ADSC)|							// ADC Start Conversion
	(0<<ADATE)|							// ADC Auto Trigger Enable
	(0<<ADIF)|							// ADC Interrupt Flag
	(0<<ADIE)|							// ADC Interrupt Enable
	(1<<ADPS2)|							// ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);
	
}