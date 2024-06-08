#include "I2C_Master.h"

/*
 * @brief: calculates bitrate for TWBR value
 * @TWSR: value of the bit rate prescaler
 * @return: TWBR (bitrate) setting value 
*/
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))

/*
 * @brief: starts the i2c comunication setting the bitrate to SCL_CLK
*/
void I2C_Init()
{
	TWBR = BITRATE(TWSR = 0x00);
}	

/*
 * @brief: links device with its addr and returns status
 * @addr: slave address
 * @return:
 *   failed, it couldnt start
 *   ready, ack received and ready
 *	 busy, ack received but busy
 *   not_found, ack never received
 * @documentation atmega328p p. 183
*/
i2c_state I2C_Start(char addr)
{
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x08) return failed;
	TWDR = addr;
	TWCR = (1<<TWEN)|(1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	switch(TWSR & 0xF8){
		case 0x18:
		  return ready;
		  break;
		case 0x20:
		  return busy;
		  break;
		default:
		  return not_found;
	}
}

/*
 * @brief: repeats i2c start comunication
  * @addr: slave address
  * @return:
  *   failed, it couldnt start
  *   ready, ack received and ready
  *	  busy, ack received but busy
  *   not_found, ack never received
  * @documentation atmega328p p. 183
*/
i2c_state I2C_Repeated_Start(char addr)
{
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x10) return failed;
	TWDR = addr;
	TWCR = (1<<TWEN)|(1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	switch(TWSR & 0xF8){
		case 0x40:
		  return ready;
		  break;
		case 0x20:
		  return busy;
		  break;
		default:
		return not_found;
	}
}

/*
 * @brief: stops i2c comunication
*/
void I2C_Stop()
{
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);
	while(TWCR & (1<<TWSTO));
}

/*
 * @brief: start i2c comunication waiting for it
 * @addr: slave address
 * @return:
 *   failed, it couldnt start
 *   ready, ack received and ready
 *	 busy, ack received but busy
 *   not_found, ack never received
*/
void I2C_Start_Wait(char addr)
{
	while (1) {
		TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));
		if ((TWSR & 0xF8) != 0x08) continue;
		TWDR = addr;
		TWCR = (1<<TWEN)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));
		if ((TWSR & 0xF8) != 0x18 ) {
			I2C_Stop();
			continue;
		}
		break;
	}
}

/*
 * @brief: writes a byte of data to the devicee 
 * @data: byte to be written
 * @return:
 *   failed, it couldnt start
 *   ready, ack received and ready
 *	 busy, ack received but busy
 *   not_found, ack never received
*/
i2c_state I2C_Write(char data)
{
	TWDR = data;
	TWCR = (1<<TWEN)|(1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	switch(TWSR & 0xF8){
		case 0x28:
		  return ready;
		  break;
		case 0x30:
		  return nack;
		  break;
		default:
		  return failed;
	}
}

/*
* @brief: reads for acknoledge message
* @return: received data from acknoledge send
*/
char I2C_Read_Ack()
{
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}	

/*
* @brief: reads for acknoledge message
* @return: received data from not acknoledge message
*/
char I2C_Read_Nack()
{
	TWCR=(1<<TWEN)|(1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}	
