
/**
 *	
	Communication definitions and functions
 */

#include <util/crc16.h>

#include "communication.h"
#include "VA-Meter.h"
#include "i2cmaster.h"
#include "uart.h"


/**
 *	Write a block of data to I2C device
 */
uint8_t writeToI2C(uint8_t devideAddr, uint8_t regAddr, uint8_t *data, uint8_t length)
{
	uint8_t status = 1;
	// wait until the device is no longer busy from the previous write operation
	i2c_start_wait(devideAddr+I2C_WRITE);	// set device address and write mode
	i2c_write(regAddr);						// Register address

	while(length--)
		status = i2c_write(*data++);

	i2c_stop();				// set stop condition = release bus

	return status;
}


/**
 *	Read a block of data from I2C device
 */
void readFromI2C(uint8_t devideAddr, uint8_t regAddr, uint8_t *data, uint8_t length)
{

	// wait until the device is no longer busy from the previous write operation
	i2c_start_wait(devideAddr+I2C_WRITE);	// set device address and write mode
	i2c_write(regAddr);						// Register address
	i2c_rep_start(devideAddr+I2C_READ);		// set device address and read mode

	while(--length)
		*data++=i2c_readAck();				// read with Acknowledge signal
	*data=i2c_readNak();					// read with Not Acknowledge signal

	i2c_stop();				// set stop condition = release bus
}



/**
 *	Send Configure/Calibration settings to INA219
 */
uint8_t writeRegisterINA(uint8_t registerAddress, uint8_t *data)
{
	return writeToI2C(DEV_INA219, CMD_MASK & registerAddress, data, 2);
}




/**
 *	Read register data from INA219
 */
void readRegisterINA(uint8_t registerAddress, uint8_t *data)
{
	readFromI2C(DEV_INA219, CMD_MASK & registerAddress, data, 2);
}


/**
 *	Read all registers data from INA219
 */
void readAllData(uint8_t *data)
{
	readRegisterINA( CMD_SHUNT_V, data );
	readRegisterINA( CMD_BUS_V, data + REG_BUS_V );
	readRegisterINA( CMD_POWER, data + REG_POWER );
	readRegisterINA( CMD_CURRENT, data + REG_CURRENT );
}


