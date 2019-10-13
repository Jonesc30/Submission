/*
 * Timers_Task2.c
 *
 * Created: 10/10/2019 10:56:29 PM
 * Author : Cody Jones
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t OVFCOUNT = 0;		// overflow count multiplier
uint8_t OVFLIMIT = 16;		// overflow count limit, set for 40% of DC

int main(void)
{
	DDRB = 0xFF;					//all registers in DDRB are outputs
	DDRC = 0x00;					//all registers in DDRC are inputs
	PORTB = 0xFF;					//turn all LEDs on
	TCCR0A = 0x00;					//normal operation
	TCNT0 = 12;					//start the timer for .625 s, 40 clock cycles
	TCCR0B |= (1 << CS02) | (1 << CS00); //set prescaler to 1024
	TIMSK0 = (1<<TOIE0);				//enable timer0 overflow interrupt
	sei();								//enable interrupts
	
	
	while (1)
	{
		
	}
}
	ISR (TIMER0_OVF_vect)
	{
		OVFCOUNT++;			// increment counter
		if (OVFCOUNT == OVFLIMIT) 
		{
			PORTB ^= (1<<3);		// toggle LED
			PORTB |=(1<<2);			// LED off from switch
			
			if(OVFLIMIT == 16) 
			{
				OVFLIMIT = 24;		// set ovf limit for 60% of DC
				if(!(PINC & (1 << 3))) //check if SW3 is pushed
				{
				PORTB &= ~(1<<2);	//LED on
				OVFLIMIT = 85;		//when switch is pressed it goes through 85 cycles for 1.3333 sec
				PORTB |= (1<<3);	//LED off
				}
			} 
			else 
			{
				OVFLIMIT = 16;		// set ovf limit for 40% of DC
				if(!(PINC & (1 << 3))) //check if SW3 is pushed
				{
					PORTB &= ~(1<<2);	//LED on
					OVFLIMIT = 85;		//when switch is pressed it goes through 85 cycles for 1.3333 sec
					PORTB |= (1<<3);	//LED off
				}
			}
			OVFCOUNT = 0;			// reset counter
		}
		
		
		TCNT0 = 12;			// timer value for .625 sec
	}
