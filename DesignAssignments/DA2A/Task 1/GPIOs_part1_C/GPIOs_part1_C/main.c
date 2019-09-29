/*
 * GPIOs_part1_C.c
 *
 * Created: 9/29/2019 1:41:11 PM
 * Author : Cody Jones
 * Student ID: 5002863328
 * Email: jonesc30@unlv.nevada.edu
 */ 

 #define F_CPU 16000000UL

 #include <avr/io.h>
 #include <util/delay.h>


 int main(void)
 {
	 DDRB |= (1<<2);		// set PORTB.2 for output
	 
	 while (1)
	 {
		 _delay_ms(250);	// delay for 40% DC
		 PORTB &= ~(1<<2);	// set LED on
		 _delay_ms(375);	// delay for remaining 60%
		 PORTB |= (1<<2);	// set LED off
	 }
 }