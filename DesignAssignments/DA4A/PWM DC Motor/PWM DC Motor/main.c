/*
 * PWM DC Motor.c
 *
 * Created: 10/31/2019 4:57:41 PM
 * Author : Cody Jones
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned int ADC_Val;			// holds value of ADC
int toggle = 0;							// used to toggle

int main(void)
{
	DDRD |= 0x40; //PWM output
	
	DDRC = 0x02;
	// PC0 as input and PC1 is now an output
	PORTC |= (1<<PC1);
	
	DDRB |= (1<<2);			//PB1 as output to stop the motor on and off
	PORTB |= (1<<2);				//Stops and turns it on
	
	TCCR0A = 0x83;
	// set none-inverting mode
	// set fast PWM Mode for timer0 max height is 256
	TCCR0B = 0x05;
	// set prescaler to 1024 and starts PWM
	
	PCICR = (1<<PCIE1);			//Enable PCINT
	PCMSK1 = (1<<PCINT9);		// Set interupt for PC1
	
	//Set up ADC
	
	ADMUX = (1<<REFS0);												// PC0 is input for ADC
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);		// enable ADC, system clock used for ADC
		
	sei();		//enable interrupts
	
	while (1);
}


//ISR for PC1
ISR(PCINT1_vect)
{
	
	if(!(PINC & (1 << PINC1)))
	{
			if(toggle == 0)
			{
				OCR0A = 0;
				PORTB &= ~(1 << 2);			//turn off OCR0A
			}
			else if(toggle == 1)
			{
				PORTB |= (1 << 2);				//turn on OCR0A
				ADCSRA |= (1 << ADSC);				// start conversion
				while((ADCSRA&(1<<ADIF))==0)
				{	// wait for conversion to finish
					ADC_Val = ADC & 0x03FF;			// we only need first 10 bits
					OCR0A = 10*ADC_Val;				// Calculate duty cycle
				}
			}
	toggle ^= 1;		//toggle
	}
}