/*
 * I2C.c
 *
 * Created: 12/6/2019 6:27:54 PM
 * Author : Cody Jones
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "MPU6050_res_define.h"
#include "I2C_Master_H_file.h"
#include "USART_RS232_H_file.h"

float Acc_x,Acc_y,Acc_z,Temperature,Gyro_x,Gyro_y,Gyro_z;

void MPU6050_Init()
{
	_delay_ms(150);										/* Power up time >100ms 			*/
	I2C_Start_Wait(0xD0);								/* Start with device write address 	*/
	I2C_Write(SMPLRT_DIV);								/* Write to sample rate register 	*/
	I2C_Write(0x07);									/* 1KHz sample rate				 	*/
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);								/* Write to power management register 	*/
	I2C_Write(0x01);									/* X axis gyroscope reference frequency */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);									/* Write to Configuration register */
	I2C_Write(0x00);									/* Fs = 8KHz 					   */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);								/* Write to Gyro configuration register */
	I2C_Write(0x18);									/* Full scale range +/- 2000 degree/C 	*/
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);								/* Write to interrupt enable register */
	I2C_Write(0x01);
	I2C_Stop();
}

void MPU_Start_Loc()
{
	I2C_Start_Wait(0xD0);								/* I2C start with device write address 			   */
	I2C_Write(ACCEL_XOUT_H);							/* Write start location address from where to read */
	I2C_Repeated_Start(0xD1);							/* I2C start with device read address 			   */
}

void Read_RawValue()
{
	MPU_Start_Loc();									/* Read Gyro values */
	Acc_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();
}

int main()
{
	char buffer[20], float_[10];
	float Xa,Ya,Za,t;
	float Xg=0,Yg=0,Zg=0;
	I2C_Init();
	MPU6050_Init();
	USART_Init(9600);
	
	while(1)
	{
		Read_RawValue();

		Xa = Acc_x/16384.0;								/* Divide raw value by sensitivity scale factor to get real values */
		Ya = Acc_y/16384.0;
		Za = Acc_z/16384.0;
		
		Xg = Gyro_x/16.4;
		Yg = Gyro_y/16.4;
		Zg = Gyro_z/16.4;

		dtostrf( Xa, 3, 2, float_ );					/* Takes the first parameter and formts is with size 3 precission 2 and puts it in float_ */
		sprintf(buffer," Ax = %s g\t",float_);			/* This is where the value is being put as a string onto the buffer variable */
		USART_SendString(buffer);						/* This is where it is being displayed on the serial terminal  */

		dtostrf( Ya, 3, 2, float_ );					/* Takes the first parameter and formts is with size 3 precission 2 and puts it in float_v */
		sprintf(buffer," Ay = %s g\t",float_);			/* This is where the value is being put as a string onto the buffer variable */
		USART_SendString(buffer);						/* This is where it is being displayed on the serial terminal  */
		
		dtostrf( Za, 3, 2, float_ );					/* Takes the first parameter and formts is with size 3 precission 2 and puts it in float_ */
		sprintf(buffer," Az = %s g\t",float_);			/* This is where the value is being put as a string onto the buffer variable */
		USART_SendString(buffer);						/* This is where it is being displayed on the serial terminal  */

		dtostrf( Xg, 3, 2, float_ );					/* Takes the first parameter and formts is with size 3 precission 2 and puts it in float_ */
		sprintf(buffer," Gx = %s%c/s\t",float_,0xF8);	/* This is where the value is being put as a string onto the buffer variable */
		USART_SendString(buffer);						/* This is where it is being displayed on the serial terminal  */

		dtostrf( Yg, 3, 2, float_ );					/* Takes the first parameter and formts is with size 3 precission 2 and puts it in float_ */
		sprintf(buffer," Gy = %s%c/s\t",float_,0xF8);	/* This is where the value is being put as a string onto the buffer variable */
		USART_SendString(buffer);						/* This is where it is being displayed on the serial terminal  */
		
		dtostrf( Zg, 3, 2, float_ );					/* Takes the first parameter and formts is with size 3 precission 2 and puts it in float_ */
		sprintf(buffer," Gz = %s%c/s\r\n",float_,0xF8);	/* This is where the value is being put as a string onto the buffer variable */
		USART_SendString(buffer);						/* This is where it is being displayed on the serial terminal  */
		_delay_ms(1000);								/* This just be a delaylay for the thingys */
	}
}
