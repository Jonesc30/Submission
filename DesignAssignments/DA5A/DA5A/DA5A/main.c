/*
 * DA5A.c
 *
 * Created: 11/27/2019 5:38:16 PM
 * Author : Cody Jones
 */ 

//	Set clock frequency
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//	Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif
#include "STDIO_UART.h"

//	Include nRF24L01+ library
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"
#include "spi.h"
void print_config(void);

//	Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;

// Global variables
volatile uint8_t ADCdata;
volatile unsigned char temp[10];



int main(void)
{
	// Set up ADC
	ADMUX |= (1 << REFS0); // use AVcc
	ADMUX |= (1 << ADLAR); // Right adjust
	ADCSRA = (1 << ADEN)	// Enable
	|(1 << ADPS1)
	|(1 << ADPS0)	// 128 prescaler for 16Mhz
	|(1 << ADATE)	// ADC Auto Trigger
	|(1 << ADSC);	// Start ADC
	
	//	Set cliche message to send (message cannot exceed 32 characters)
	char tx_message[32];				// Define string array
	unsigned char i;
	char dummy[10];
	
	//	Initialize UART
	uart_init();
	
	//	Initialize nRF24L01+ and print configuration info
	nrf24_init();
	print_config();
	
	//	Start listening to incoming messages
	nrf24_start_listening();

	strcpy(tx_message,"GOOD");	// Copy string into array
	nrf24_send_message(tx_message);
	while (1)
	{
		ADCdata = (ADCH << 1) * 2 + 32; // Convert Celsius to Fahrenheit
		itoa(ADCdata, dummy, 10); //convert char to ascii
		for(i = 0 ; i < 10 ; i++)
		{
			temp[i] = dummy[i]; //move converted ascii
		}
		
		//strcpy(tx_message,"Waiting");	// Copy string into array
		//nrf24_send_message(tx_message);
		//_delay_ms(50);
		
		if (message_received)
		{
			//	Message received, print it
			message_received = false;
			printf("Received message: %s\n",nrf24_read_message());
			//	Send message as response
			_delay_ms(500);
			status = nrf24_send_message(temp);
			if (status == true) printf("Message sent successfully\n");
		}
	}
}

//	Interrupt on IRQ pin
ISR(INT0_vect)
{
	message_received = true;
}


void print_config(void)
{
	uint8_t data;
	printf("Startup successful\n\n nRF24L01+ configured as:\n");
	printf("-------------------------------------------\n");
	nrf24_read(CONFIG,&data,1);
	printf("CONFIG		0x%x\n",data);
	nrf24_read(EN_AA,&data,1);
	printf("EN_AA			0x%x\n",data);
	nrf24_read(EN_RXADDR,&data,1);
	printf("EN_RXADDR		0x%x\n",data);
	nrf24_read(SETUP_RETR,&data,1);
	printf("SETUP_RETR		0x%x\n",data);
	nrf24_read(RF_CH,&data,1);
	printf("RF_CH			0x%x\n",data);
	nrf24_read(RF_SETUP,&data,1);
	printf("RF_SETUP		0x%x\n",data);
	nrf24_read(STATUS,&data,1);
	printf("STATUS		0x%x\n",data);
	nrf24_read(FEATURE,&data,1);
	printf("FEATURE		0x%x\n",data);
	printf("-------------------------------------------\n\n");
}