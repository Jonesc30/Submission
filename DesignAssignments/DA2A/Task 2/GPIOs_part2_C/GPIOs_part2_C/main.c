/*
 * GPIOs_part2_C.c
 *
 * Created: 9/29/2019 1:49:00 PM
 * Author : Cody Jones
 */ 

 #define F_CPU 16000000UL

 #include <avr/io.h>
 #include <util/delay.h>

 int main(void)
 {
	 DDRC &= (0<<2);		// connect PORTC.2 to switch as input
	 PORTC |= (1<<2);	// enable pull-up
	 
	 DDRB |= (1<<2);		// set PORTB.2 for output (LED)
	 
	 while(1){
		 if(!(PINC & (1<<PINC2))){	// check if pin is low
			 PORTB &= ~(1<<2);		// set LED on
			 _delay_ms(1333);		// set delay of 1.333 sec
		 }
		 else{						// otherwise (pin is high)
			 PORTB |= (1<<2);		// set LED off
		 }
	 }
	 return 0;
 }
