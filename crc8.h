#ifndef CRC8_H
#define CRC8_H

#include <inttypes.h>


/**
 *	@file
 *	@defgroup jyrith_crc8 Crc8
 *	@brief CRC 8 calculation
	
	@author J.Karlbom
 */

/**@{*/

/**
 *	
	@brief Calculate crc calue 
 
	@param bytes Data array
	@param size Size of data array.
	
	@return	crc8 value
 */

uint8_t crc8(uint8_t bytes[], uint8_t size);

/**@}*/

#endif // CRC8_H
