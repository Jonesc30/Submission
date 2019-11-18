/*
 * Servo.c
 *
 * Created: 11/17/2019 4:24:13 PM
 * Author : Cody Jones
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
int check = 0;
int main(void)

{
	

	DDRB = 0xFF; //DDRB as an output
	//DDRD = 0xFF;
	TCCR1B |= (1<<WGM13)  | (1<<WGM12)  | (1<<CS11)  | (1<<CS10);
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
	ICR1=4999;
	ADMUX = 0x60;
	ADCSRA = 0xA6;

	while (1)
	{
		ADCSRA |= ( 1 << ADSC); //start conversion
		while((ADCSRA & (1 << ADIF))== 0);
		check = ADCH; 			//temp value
		
		if(check == 0) // minimum value
		{
			OCR1A = 0; //turn 0 deg
			_delay_ms(500);
		}
		if(check == 255) // maximum pot value
		{
			//PORTB = ( 1 << PORTB2);
			OCR1A = 535;  //turn 180
			_delay_ms(500);
		}
	}
}