

#include "avr/io.h"
#include <avr/eeprom.h>
#include "eepromManage.h"
#include <avr/interrupt.h>



uint8_t EepromRead_PassWord(uint32_t *addr, uint8_t *data)
{

	eeprom_busy_wait(); 
	eeprom_read_block(data,  addr, 8);

	return TRUE;
}




uint8_t EepromWrite_PassWord(uint32_t *addr, uint8_t *data)
{

	eeprom_busy_wait();
	cli();
	eeprom_write_block(data,  addr, 8);
	sei();
	return TRUE;
}



uint8_t EepromWrite_Byte(uint32_t *addr, uint8_t *data)
{

	eeprom_busy_wait();
	eeprom_write_byte(data,  addr);

	return TRUE;
}



uint8_t EepromRead_Byte(uint32_t *addr, uint8_t *data)
{

	eeprom_busy_wait();
	eeprom_read_byte(data);

	return TRUE;
}



