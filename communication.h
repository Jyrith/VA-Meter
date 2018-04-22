
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

/**
 *	@file
 *	@defgroup jyrith_com Communication
	@brief Functions to read and write INA219 registers
	
	Communication definitions and functions for 
		I2C
	
	@author J.Karlbom
 */


#include <inttypes.h>

/**@{*/

/** 
 *	
	@name  Definitions for INA219
	Device address of the INA219. 
 */

//#define DEV_INA219  0x40		// device address of INA219
								// 0100 000 R/W
#define DEV_INA219  0x80		///< 1000 000 R/W

/** 
 *
	@name  Definitions for Uart commands
	Baud rate setting
	Commands for serial communication. 
 */

#define UART_BAUD_RATE      19200 

#define CMD_CONF 	0x50	///< P, Command, Configuration register
#define CMD_SHUNT_V 0x51	///< Q, Command, Shunt voltage register
#define CMD_BUS_V 	0x52	///< R, Command, Bus voltage register
#define CMD_POWER 	0x53	///< S, Command, Power register
#define CMD_CURRENT 0x54	///< T, Command, Current register
#define CMD_CALIB 	0x55	///< U, Command, Calibration register
#define CMD_ALL 	0x56	///< V, Command, Read all register values

#define CMD_MASK 	0x07	///< Mask bits to dig out a register address


/** 
 *
	@name Functions
 */


/**
 *	
	@brief Write a block of data to I2C device
 
	@param devideAddr Device address of the I2C device.
	@param regAddr Register address of the I2C device.
	@param *data Pointer to a data to be written to device.
	@param length Length of data.
	
	@return	status info
 */

uint8_t writeToI2C(uint8_t devideAddr, uint8_t regAddr, uint8_t *data, uint8_t length);



/**
 *	
	@brief Read a block of data from I2C device
 
	@param devideAddr Device address of the I2C device.
	@param regAddr Register address of the I2C device.
	@param *data Pointer to a data to be read to device.
	@param length Length of data.
	
	@return	none
 */

void readFromI2C(uint8_t devideAddr, uint8_t regAddr, uint8_t *data, uint8_t length);



/**
 *	
	@brief Configure/Calibration settings of INA219
	Configure:
		2 byte register
			Voltage range
			PGA gain
			ADC resolution/averaging
	Calibrate:
		2 byte register
			Calibration value
	@param registerAddress	Register address to write
	@param data	Register Bytes to write
	@retval   0 write successful
	@retval   1 write failed

 */
uint8_t writeRegisterINA(uint8_t registerAddress, uint8_t *data);



/**
	@brief Read register data from INA219
	@param registerAddress	Register address
	@param data	Register bytes to write
 */
void readRegisterINA(uint8_t registerAddress, uint8_t *data);



/**
	@brief Read all registers data from INA219
	@param data	Data array to write read data
 */
void readAllData(uint8_t *data);

/**@}*/

#endif // COMMUNICATION_H
