/*
 * FinalProject.c
 *
 * Created: 12/8/2019 12:32:56 AM
 * Author : cody jones
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// Global constants for uart
#define BAUD 115200
#define FOSC 16000000
#define UBRR FOSC/8/BAUD-1

#define APDS9960_WRITE 0x72
#define APDS9960_READ 0x73

//include standard libraries
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>


//include custom libraries
#include "APDS9960_def.h"
#include "i2c_master.h"


//Function declarations
void getValues(void);
// void TIMER1_init();
void init_APDS9960(void);
void usart_init();
void USART_putstring(volatile unsigned char *StringPtr);

//AT commands
volatile unsigned char AT[] = "AT\r\n"; // Test
volatile unsigned char CWMODE[] = "AT+CWMODE=3\r\n"; // Set Wi-Fi mode
volatile unsigned char CWJAP[] = "AT+CWJAP=\"ZTE7\",\"hello702\"\r\n"; // Get Wi-Fi info
volatile unsigned char CIPSTART[] = "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n"; // Establish connection with ThingSpeak
volatile unsigned char CIPSEND[] = "AT+CIPSEND=104\r\n"; // Set send function to 104
volatile unsigned char CIPMUX[] = "AT+CIPMUX=0\r\n"; // Enable connection
volatile unsigned char SEND_DATA[] = "GET /update?key=D4LE8GA6LD4PGK13&field1="; // Get Write Key
volatile unsigned char RESET[] = "AT+RST\r\n"; // Get AT Firmware info
volatile unsigned char LINEBREAK[] = "\r\n"; // end of temperature transmission
volatile unsigned char CLOSE[] = "AT+CIPCLOSE\r\n";

//string for colors
volatile unsigned char RedStr[10];
volatile unsigned char GreenStr[10];
volatile unsigned char BlueStr[10];

uint16_t redVal, greenVal, blueVal;


int main(void){
	i2c_init();
	usart_init(115200);
	init_APDS9960();

	//Start up Esp
	//Start AT communication
	_delay_ms(10);
	USART_putstring(AT);					//send AT to the USART

	//connect to network
	_delay_ms(10);
	USART_putstring(RESET);		//reset ESP
	_delay_ms(10);
	USART_putstring(AT);			//confirm communication
	_delay_ms(10);
	USART_putstring(CWMODE);		//WiFi mode = 3
	_delay_ms(10);
	USART_putstring(CWJAP);		//Send wifi login


	while(1){
		//getValues();

		_delay_ms(10);
		USART_putstring(CIPMUX);		//Single connection point
		_delay_ms(10);
		USART_putstring(CIPSTART);	// Connect to ThingSpeak
		_delay_ms(10);
		USART_putstring(CIPSEND);	// Declare send length 50
		_delay_ms(10);
		getValues();
		USART_putstring(SEND_DATA);	// Connect to proper key
		USART_putstring(RedStr);		// Send adc data
		USART_putstring("&field2=");
		USART_putstring(GreenStr);		// Send adc data
		USART_putstring("&field3=");
		USART_putstring(BlueStr);		// Send adc data
		_delay_ms(1000);

	}
	
	return 0;
}


void init_APDS9960(void){
	uint8_t setup;
	
	i2c_readReg(APDS9960_WRITE, APDS9960_ID, &setup,1);
	if(setup != APDS9960_ID_1) while(1);
	setup = 1 << 1 | 1<<0 | 1<<3 | 1<<4;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_ENABLE, &setup, 1);
	setup = DEFAULT_ATIME;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_ATIME, &setup, 1);
	setup = DEFAULT_WTIME;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_WTIME, &setup, 1);
	setup = DEFAULT_PROX_PPULSE;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_PPULSE, &setup, 1);
	setup = DEFAULT_POFFSET_UR;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_UR, &setup, 1);
	setup = DEFAULT_POFFSET_DL;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_DL, &setup, 1);
	setup = DEFAULT_CONFIG1;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG1, &setup, 1);
	setup = DEFAULT_PERS;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_PERS, &setup, 1);
	setup = DEFAULT_CONFIG2;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG2, &setup, 1);
	setup = DEFAULT_CONFIG3;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG3, &setup, 1);
	
}
 

void getValues(void){
uint8_t redVH, redVL;
uint8_t greenVH, greenVL;
uint8_t blueVH, blueVL;

unsigned char i;
char dummy[10];


// Read red value
i2c_readReg(APDS9960_WRITE, APDS9960_RDATAH, &redVH, 1);
i2c_readReg(APDS9960_WRITE, APDS9960_RDATAL, &redVL, 1);

// Read green value
i2c_readReg(APDS9960_WRITE, APDS9960_GDATAH, &greenVH, 1);
i2c_readReg(APDS9960_WRITE, APDS9960_GDATAL, &greenVL, 1);

// Read blue value 
i2c_readReg(APDS9960_WRITE, APDS9960_BDATAH, &blueVH, 1);
i2c_readReg(APDS9960_WRITE, APDS9960_BDATAL, &blueVL, 1);

redVal = (redVH << 8) | redVL;
greenVal = (greenVH << 8) | greenVL;
blueVal = (blueVH << 8) | blueVL;

// Set max threshold values
if (redVal > 255){
redVal = 255;
}
if (greenVal > 255){
greenVal = 255;
}
if (blueVal > 255){
blueVal = 255;
}
	
	itoa(redVal, dummy, 10); //convert char to ascii
	for(i = 0 ; i < 10 ; i++){
		RedStr[i] = dummy[i]; //move converted ascii
	}	
	itoa(greenVal, dummy, 10); //convert char to ascii
	for(i = 0 ; i < 10 ; i++){
		GreenStr[i] = dummy[i]; //move converted ascii
	}	
	itoa(blueVal, dummy, 10); //convert char to ascii
	for(i = 0 ; i < 10 ; i++){
		BlueStr[i] = dummy[i]; //move converted ascii
	}
	
	
}

void usart_init() {
	UBRR0H = ((UBRR) >> 8);
	UBRR0L = UBRR;
	UCSR0A |= (1<< U2X0); // divisor baud = 8
	UCSR0B |= (1 << TXEN0); // Enable transmission
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // 8 bits
}

void USART_putstring(volatile unsigned char *StringPtr)
{
	while ((*StringPtr != '\0')){ 
		while (!(UCSR0A & (1 << UDRE0))); 
		UDR0 = *StringPtr; 
		StringPtr++; 
	}
}
