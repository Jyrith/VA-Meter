
#ifndef VA_METER_H
#define VA_METER_H


/**
 *	@file VA-Meter.h
 *	@defgroup jyrith_main Main code
 *
 *	@brief	INA219 Current/Power Monitor 
	- INA219 is able to monitor current, voltage and power.
	- Current, voltage and power values can be read by sending commands to serial port
 	- Reads voltage, current and power values from INA219 and send values to serial port

	- atmega328p

 *	@author J.Karlbom
 */

/**@{*/

/**
 *	
 *	I/O Control settings for LED
 */

#define LED_PORT	PORTB	///< LED Port
#define LED_DDR		DDRB	///< LED Port DDR
#define LED_PIN		PB5		///< LED pin


/**
 *	
	Serial communication protocol table
	Array name: inputData[]

	- [0] -> Header 1
	- [1] -> Header 2
	- [2] -> Command
	- [3] -> Parameter 1
	- [4] -> Parameter 2
	- [5] -> CRC
 */

#define INPUT_ARRAY_SIZE 6	///< Size value for serial input array, with crc
#define LOC_HEADER1	0		///< Header  byte 1
#define LOC_HEADER2	1		///< Header  byte 2
#define LOC_CMD		2		///< Command
#define LOC_PARA1 	3		///< Parameter 1
#define LOC_PARA2 	4		///< Parameter 2
#define LOC_CRC 	5		///< CRC

//#define HEADER 	0xA5		// Header byte
#define HEADER 	0x23		///< Header byte,#
#define ERROR_CHAR 	0x3F	///< Error reply, '?' char


#define ERROR_CRC 		0x30	///< Error code, crc mismatch
#define ERROR_HEADER 	0x31	///< Error code, Header bytes are missing

/**
 *	
	Register array locations for values to read from INA219
	Array name: registerData[]
 */
#define REG_ARRAY_SIZE 8	///< Register array size, 2 bytes for each register
#define REG_SHUNT_V 0x00	///< Shunt voltage register location in register data array
#define REG_BUS_V 	0x02	///< Bus voltage register location in register data array
#define REG_POWER 	0x04	///< Power register location in register data array
#define REG_CURRENT 0x06	///< Current register location in register data array


/**
 *	Handle commands 
 */
void handleCommand(uint8_t);

/**
 *	@brief Send header and command
 */
void sendHeader(uint8_t command);

/**
 *	@brief Send reply of register setting command status
 */
void sendRegSetReply(uint8_t command, uint8_t reply);

/**
 *	@brief Send reply with register data
 */
void sendRegReadReply(uint8_t command, uint8_t registerLocation);

/**
 *	@brief Send data of all register
 */
void sendAllData(void);

/**
 *	@brief Calculate crc and compare it with received crc byte
 *	@return	status info
 */
uint8_t checkCrc(void);

/**
 *	@brief Error in communication
 *		Clear buffer
 *		Blink LED for a while
 */
void handleError(uint8_t errorCode);



/**
 *	@brief	Bit-mask operation
 	- BitSet
	- BitClear
	- BitCheck
 */

#define BitSet(port,bit) ((port) |= (1<< (bit)))	///< Set specified bit on port
#define BitClear(port,bit) ((port) &= ~(1<< (bit)))	///< Clear specified bit on port
#define BitCheck(port,bit) ((port) & (1<< (bit)))	///< Toggle specified bit on port

/**@}*/

#endif // VA_METER_H
