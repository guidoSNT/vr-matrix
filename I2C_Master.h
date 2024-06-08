#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#define SCL_CLK 100000L

typedef enum{failed,ready,busy,not_found,nack} i2c_state;
	
void I2C_Init();
i2c_state  I2C_Start(char);
i2c_state  I2C_Repeated_Start(char);
void I2C_Stop();
void I2C_Start_Wait(char);
i2c_state  I2C_Write(char);
char I2C_Read_Ack();
char I2C_Read_Nack();


#endif