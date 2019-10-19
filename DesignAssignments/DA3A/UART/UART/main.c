/*
 * UART.c
 *
 * Created: 10/18/2019 5:14:21 PM
 * Author : Cody Jones
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void USART_init(void); //function to initialize USART
unsigned char USART_receive(void); //function to receive through USART
void USART_transmit(unsigned char data); //function to send through USART
void USART_putstring(char* StringPtr); //function to scan through the string
float cj = 2.500; //float number chosen for assignment

char String[] = "DA3A"; //string for task
char newline[] = "\n"; //so that I can create a new line
char buffer[5]; //buffer created for the random int number that will be 5 digits
char cj_buffer[4]; //buffer created for float number chosen above
uint8_t OVF_COUNT = 0; //initialize the overflow count for interrupt
uint8_t OVF_LIMIT = 250; //set the limit the count can reach to set 1 sec delay

int main(void)
{
	USART_init(); //initialize USART
	
	TCCR0A = 0x00; //normal operation
	TCCR0B |= (1 << CS02); //set prescalar to 256
	TCNT0 = 16; //TOP = 256-250 = 16
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

// C program for implementation of ftoa()
// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
	int i=0, j=len-1, temp;
	while (i<j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++; j--;
	}
}

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x)
	{
		str[i++] = (x%10) + '0';
		x = x/10;
	}
	
	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)n;
	
	// Extract floating part
	float fpart = n - (float)ipart;
	
	// convert integer part to string
	int i = intToStr(ipart, res, 0);
	
	// check for display option after point
	if (afterpoint != 0)
	{
		res[i] = '.';  // add dot
		
		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter is needed
		// to handle cases like 233.007
		fpart = fpart * pow(10, afterpoint);
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}

ISR (TIMER0_OVF_vect)
{
	OVF_COUNT++; //increment the overflow counter
	if (OVF_COUNT == OVF_LIMIT) //check to see if the limit was reached
	{
		USART_putstring(String); //print string to serial monitor (terminal window)
		USART_putstring(newline); //go to next line
		itoa(rand(), buffer, 10); //convert random int number (base 10) to a string
		USART_putstring(buffer); //print out the converted string stored in buffer
		USART_putstring(newline); //go to next line
		ftoa(cj, cj_buffer, 2); //convert float number (2 digits after decimal precision) to a string
		USART_putstring(cj_buffer); //print out the converted string stored in buffer
		USART_putstring(newline); //go to next line
		USART_putstring(newline); //go to next line
		OVF_COUNT = 0; //reset overflow counter
	}
	TCNT0 = 16; //reset TOP
}