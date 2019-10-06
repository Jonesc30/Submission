/*
 * GPIO_IRQ_C.c
 *
 * Created: 10/5/2019 9:33:55 PM
 * Author : Cody Jones
 */ 

 #define F_CPU 16000000UL

 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>


 int main(void)
 {
	 DDRB = (1<<3);			// set PORTB.3 for output
	 PORTB = (1<<3);		// set LED low
	 PORTD = (1<<2);		//pull-up activated
	 EIMSK = (1<<INT0);		// enable external interrupt 0
	 EICRA = 0x2;			// make INT0 falling edge triggered
	 sei ();				// enable interrupts
	 
	 
	 while (1)
	 {
		 _delay_ms(250);	// delay for 40% DC
		 PORTB &= ~(1<<3);	// set LED on
		 _delay_ms(375);	// delay for remaining 60%
		 PORTB |= (1<<3);	// set LED off
	 }
 }
 
 ISR (INT0_vect)	{		// ISR for external interrupt INT0
	 PORTB &= ~(1<<3);		// turns on PORTB.3 (LED)
	 _delay_ms(1333);		// delay for 1.33 sec
 }