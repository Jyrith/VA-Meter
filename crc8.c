
/**
 *	
	CRC 8 
 */

#include "crc8.h"

uint8_t crc8(uint8_t bytes[], uint8_t size)
	{
	const uint8_t polynomial = 0x07;	// CRC-8-CCITT
	uint8_t crc = 0;
	uint8_t i,j;

	for (j = 0; j < size; j++)
		{
		crc ^= bytes[j];
		for (i = 0; i < 8; i++)
			{
			if ((crc & 0x80) != 0)
				{
				crc = (uint8_t)((crc << 1) ^ polynomial);
				}
			else
				{
				crc <<= 1;
				}
			}
		}
	return crc;
	}
