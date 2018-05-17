#ifndef __MISC_H
#define __MISC_H

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BinToHex byte_to_ascii
#define HexToBin asc_to_byte
	

int16_t Round(int16_t val);
uint16_t MB_Crc16( uint8_t *pBuf, uint16_t len );
uint8_t Diff(uint8_t *ptr1, uint8_t *ptr2, uint16_t size);
	
uint8_t *byte_to_ascii (uint8_t value, uint8_t *pbuf);
uint8_t *word_to_ascii (uint16_t value, uint8_t *pbuf);
uint8_t *float_to_ascii(float value, uint8_t *pbuf);
uint8_t asc_to_byte (uint8_t *p_ascii);
uint16_t asc_to_word(uint8_t *p_ascii);
float asc_to_float(uint8_t *p_ascii);
	
#ifdef __cplusplus
}
#endif

#endif
