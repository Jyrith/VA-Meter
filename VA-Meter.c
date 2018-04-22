
/**
 *
 *	@brief	Current, voltage and power monitor tool 
 *		
 *	Measure voltage, current and power values with INA219 
 *	
 *	atmega328p
 *
 *	@author J.Karlbom
 */

/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "VA-Meter.h"
#include "communication.h"
#include "uart.h"
#include "i2cmaster.h"
#include "crc8.h"

/**	
 *	Bit-mask variable
 */

//volatile uint8_t globalBitStatus = 0;

uint8_t inputData[INPUT_ARRAY_SIZE];
uint8_t registerData[REG_ARRAY_SIZE];


/**
 *	Main
 */
int main (void)
	{    

    /*	Initialize UART library, 
		pass baudrate and AVR cpu clock 
	*/
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); 

    /*	Initialize I2C
	*/	
	i2c_init();

	// Set LED pin as output.
	LED_DDR |= (1<<LED_PIN);


	uint16_t charFromSerial;
    
    /*	Enable interrupts
	*/
    sei();

	uart_puts_P("AD-Converter\n");

	uint8_t index = 0;
	
	// Main loop
	for(;;)
		{
		charFromSerial = uart_getc();
		// Check if any data is available from UART
		if (! (charFromSerial & UART_NO_DATA ))	
			{
			BitSet(LED_PORT,LED_PIN);	// receiver visual indication
			inputData[index++] = (uint8_t) charFromSerial & 0x00FF;
			if (index >= INPUT_ARRAY_SIZE)
				{
				if ((inputData[LOC_HEADER1] == HEADER) && (inputData[LOC_HEADER2] == HEADER))
					{
					handleCommand(inputData[LOC_CMD]);
					}
				else	// Correct Header bytes are missing,reply with error note
					{
					handleError(ERROR_HEADER);
					}
				index = 0;	// Clear index for new input stream in both cases
				}
			}
			BitClear(LED_PORT,LED_PIN);	// receiver visual indication
		}	// For loop end
	}		// Main end


/**
 *	Handle commands 
 */
void handleCommand(uint8_t command)
	{
	uint8_t status;

	status = checkCrc();
	if(!status)
		{
		handleError(ERROR_CRC);
		return;
		}

	switch (command) 
		{
		case CMD_CONF:
		status = writeRegisterINA(CMD_CONF, inputData + LOC_PARA1);
		sendHeader(CMD_CONF);
		sendRegSetReply(CMD_CONF, status);
		break;

		case CMD_SHUNT_V:
		readRegisterINA(CMD_SHUNT_V, registerData);
		sendRegReadReply(CMD_SHUNT_V,REG_SHUNT_V);
		break;

		case CMD_BUS_V:
		readRegisterINA(CMD_BUS_V, registerData + REG_BUS_V);
		sendRegReadReply(CMD_BUS_V, REG_BUS_V);
		break;

		case CMD_POWER:
		readRegisterINA(CMD_POWER, registerData + REG_POWER);
		sendRegReadReply(CMD_POWER, REG_POWER);
		break;

		case CMD_CURRENT:
		readRegisterINA(CMD_CURRENT, registerData + REG_CURRENT);
		sendRegReadReply(CMD_CURRENT, REG_CURRENT);
		break;

		case CMD_CALIB:
		status = writeRegisterINA(CMD_CALIB, inputData + LOC_PARA1);
		sendRegSetReply(CMD_CALIB, status);
		break;

		case CMD_ALL:
		readAllData(registerData);
		sendHeader(CMD_ALL);
		sendAllData();
		break;
		}
	}


/**
 *	Send header and command
 */
void sendHeader(uint8_t command)
	{
	uart_putc(HEADER);
	uart_putc(HEADER);
	uart_putc(command);
	}

/**
 *	Send reply of register setting command status
 */
void sendRegSetReply(uint8_t command, uint8_t reply)
	{
	sendHeader(command);
	uart_putc (reply);
	}


/**
 *	Send reply with register data
 */
void sendRegReadReply(uint8_t command, uint8_t registerIndex)
	{
	sendHeader(command);
	uart_putc(registerData[registerIndex]);
	uart_putc(registerData[registerIndex + 1]);
	}


/**
 *	Send all data 
 */
void sendAllData(void)
	{
	uint8_t i;
	for( i=0; i<REG_ARRAY_SIZE; i++ )
		{
		uart_putc(registerData[i]);	
		}
	}


/**
 *	Calculate crc and compare it with received one
 */
uint8_t checkCrc(void)
	{
	uint8_t crc = 0;
	crc = crc8(registerData, INPUT_ARRAY_SIZE - 1);
	if(crc == registerData[LOC_CRC])
		return 1;
	else
		return 0;
	}



/**
 *	Error in communication
 */
void handleError(uint8_t errorCode)
	{

	uint8_t i = 20;

	uart_puts_P("???");
	uart_putc(errorCode);
	while ( uart_getc() & UART_NO_DATA)
		;	// Clear UART ringbuffer 

	while(i--)
		{
		BitSet(LED_PORT,LED_PIN);
		_delay_ms (100);
		BitClear(LED_PORT,LED_PIN);
		_delay_ms (100);
		}
	}
