/*
 * Timers_Task1.c
 *
 * Created: 10/10/2019 7:11:14 PM
 * Author : Cody Jones
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>


int main(void)
{
	DDRB = 0xFF;					//all registers in DDRB are outputs
	DDRC = 0x00;					//all registers in DDRC are inputs
	PORTB = 0xFF;					//turn all LEDs on
	TCCR0A = 0x00;					//normal operation
	TCNT0 = 0x00;					//start the timer
	TCCR0B |= (1 << CS02) | (1 << CS00); //set prescaler to 1024
	int count = 0;						//use this to count overflow
	
	//The fout for this is about 60Hz
	
	while (1)
	{
		count = 0;					// reset counter
		TCNT0 = 0x00;				// reset timer
		while (count < 23)			// loop until 60% DC is met
		{			
			if(TCNT0 == 0xFF)		// if timer overflows
			{		
				count++;			// increment counter
				TCNT0 = 0x00;		// reset timer
			}
			 else if(!(PINC & (1 << 3))) //check if SW3 is pushed
			 {
				 PORTB |= (1 << 3); //turn off LED @ PB3
				 PORTB &= ~(1 << 2); //turn on LED @ PB2
				 TCNT0 = 0x00; //reset timer if switch is pushed
				 count = 0;
				 while(count < 82) //1.333 seconds
				 {
					 if(TCNT0 == 0xFF)
					 {
						 count++;
						 TCNT0 = 0x00;
					 }
				 }
				 PORTB |= (1 << 2); //turn off LED @ PB2
			 }
		}
		PORTB ^= (1<<3);			// toggle LED off
		count = 0;					// reset counter
		TCNT0 = 0x00;				// reset timer
		while (count < 15)			// loop until remaining 40% DC is met
		{			
			if(TCNT0 == 0xFF)		// if timer overflows
			{		
				count++;			// increment counter
				TCNT0 = 0x00;		// reset timer
			}
			 else if(!(PINC & (1 << 3))) //check if SW3 is pushed
			 {
				 PORTB |= (1 << 3); //turn off LED @ PB3
				 PORTB &= ~(1 << 2); //turn on LED @ PB2
				 TCNT0 = 0x00; //reset timer if switch is pushed
				 count = 0;
				 while(count < 82) //1.333 seconds
				 {
					 if(TCNT0 == 0xFF)
					 {
						 count++;
						 TCNT0 = 0x00;
					 }
				 }
				 PORTB |= (1 << 2); //turn off LED @ PB2
			}
		}
		PORTB ^= (1<<3);			// toggle LED on
	}
return 0;
}
