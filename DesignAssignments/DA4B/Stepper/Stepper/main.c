/*
 * Stepper.c
 *
 * Created: 11/17/2019 2:40:58 PM
 * Author : Cody Jones
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void adc_int(void);
void timer_init(void);
volatile unsigned int speed; // Value of delay AKA speed
volatile int stop = 0; 		 // Variable used to stop operation

int main(void)
{
	DDRB = 0x0F;        // Set PD0 - PD3 as outputs for stepper motor
	adc_int();	      	// Initialize ADC
	TCCR1B = 0x0D;      // Set CTC mode and 1024 prescaler
	while(1){
		while((ADCSRA&(1<<ADIF))==0); // wait for ADC conversion
		
		if (ADC <= 4)    {stop = 0; speed = 1;}
		if (ADC <= 85)   {stop = 0; speed = 2;}
		if (ADC <= 170)  {stop = 0; speed = 3;}
		if (ADC <= 255)  {stop = 0; speed = 4;}
		if (ADC <= 340)  {stop = 0; speed = 5;}
		if (ADC <= 425)  {stop = 0; speed = 6;}
		if (ADC <= 510)  {stop = 0; speed = 7;}
		if (ADC <= 595)  {stop = 0; speed = 8;}
		if (ADC <= 680)  {stop = 0; speed = 9;}
		if (ADC <= 765)  {stop = 0; speed = 10;}
		if (ADC <= 850)  {stop = 0; speed = 11;}
		if (ADC <= 935)  {stop = 0; speed = 12;}
		if (ADC <= 1015) {stop = 0; speed = 13;}
		if (ADC >= 1016)  {stop = 1;}

		OCR1A = speed;	// set OCR1A to the determined speed
		TCNT1 = 0x00;	// reset the clock
		if(stop == 0){
			// If the ADC value is not at its MAX value then step with desired delay
			while((TIFR1 & 0x2) != 0x2);
			PORTB = 0x09;
			TIFR1 |= (1<<OCF1A);
			while((TIFR1 & 0x2) != 0x2);
			PORTB = 0x08;
			TIFR1 |= (1<<OCF1A);
			while((TIFR1 & 0x2) != 0x2);
			PORTB = 0X0C;
			TIFR1 |= (1<<OCF1A);
			while((TIFR1 & 0x2) != 0x2);
			PORTB = 0X04;
			TIFR1 |= (1<<OCF1A);
			while((TIFR1 & 0x2) != 0x2);
			PORTB = 0x06;
			TIFR1 |= (1<<OCF1A);
			while((TIFR1 & 0x2) != 0x2);
			PORTB = 0x02 ;
			TIFR1 |= (1<<OCF1A);
			while((TIFR1 & 0x2) != 0x2);
			PORTB = 0X03;
			TIFR1 |= (1<<OCF1A);
			while((TIFR1 & 0x2) != 0x2);
			PORTB = 0X01;
			TIFR1 |= (1<<OCF1A);
		}
	}
}

void adc_int(void){
	ADMUX = (0<<REFS1)|	//	Reference Selection Bits
	(1<<REFS0)|	//	AVcc-external cap at AREF
	(0<<ADLAR)|	//	ADC	Left Adjust	Result
	(0<<MUX3) |
	(0<<MUX2) |	//	ANalogChannel Selection	Bits
	(0<<MUX1) |	//	ADC0 (PC0)
	(0<<MUX0);

	ADCSRA = (1<<ADEN)|	//	ADC	Enable
	(1<<ADSC) |	//	ADC	Start Conversion
	(1<<ADATE)|	//	ADC	Auto Trigger Enable
	(0<<ADIF) |	//	ADC	Interrupt Flag
	(1<<ADIE) |	//	ADC	Interrupt Enable
	(1<<ADPS2)|	//	ADC	PrescalerSelect	Bits
	(1<<ADPS1)|
	(1<<ADPS0);
}
