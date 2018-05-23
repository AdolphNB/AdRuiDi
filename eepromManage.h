#ifndef __EEPROM_INPUT_OUTPUT__
#define __EEPROM_INPUT_OUTPUT__

#include "config.h"

#include "avr/io.h"
#include <avr/eeprom.h>


uint8_t EepromRead_PassWord(uint32_t *addr, uint8_t *dst);
uint8_t EepromWrite_PassWord(uint32_t *addr, uint8_t *src);
uint8_t EepromWrite_Byte(uint32_t *addr, uint8_t *data);
uint8_t EepromRead_Byte(uint32_t *addr, uint8_t *data);


#endif

