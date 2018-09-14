#ifndef __EEPROM_INPUT_OUTPUT__
#define __EEPROM_INPUT_OUTPUT__

#include "config.h"

#include "avr/io.h"
#include <avr/eeprom.h>


extern AmortizeDataBase_t EepromDB;

uint8_t EepromRead_Block(uint16_t *addr, uint8_t *data, uint8_t num);
uint8_t EepromWrite_Block(uint16_t *addr, uint8_t *data, uint8_t num);
uint8_t EepromRead_PassWord(uint16_t *addr, uint8_t *dst, uint8_t num);
uint8_t EepromWrite_PassWord(uint16_t *addr, uint8_t *src, uint8_t num);
uint8_t EepromWrite_Byte(uint16_t addr, uint8_t data);
uint8_t EepromRead_Byte(uint16_t addr, uint8_t *data);
uint8_t SetAmortizeAndStore(uint8_t pic, uint8_t ch);
void ChEnSwitch();


#endif

