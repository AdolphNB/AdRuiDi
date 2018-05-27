

#include "avr/io.h"
#include <avr/eeprom.h>
#include "eepromManage.h"
#include <avr/interrupt.h>


AmortizeDataBase_t EepromDB;

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



uint8_t SetAmortizeAndStore(uint8_t pic, uint8_t ch)
{
	uint8_t i;
	static uint8_t setAmortize = 0;
	
	switch(ch){

		case 0:
		case 3:
		case 6:
		case 12:
			
			setAmortize = ch;
			//change display color;
			

			break;

		case 0xff: //confirm completed set
		
			EepromWrite_Byte(EEPROM_ADDRESS_TOTAL_NUMBER, setAmortize);

			for(i = 0; i < setAmortize; i++){
				
			}
			setAmortize = 0;

			return TRUE;
				
			break;

	}	

	return FALSE;
}








