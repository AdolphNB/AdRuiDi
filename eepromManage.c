

#include "avr/io.h"
#include <avr/eeprom.h>
#include "eepromManage.h"
#include <avr/interrupt.h>
#include "user_hmi.h"

AmortizeDataBase_t EepromDB;
extern RepayDate_t CurDate;

uint8_t EepromRead_Block(uint16_t *addr, uint8_t *data, uint8_t num)
{
	cli();
	eeprom_busy_wait(); 
	eeprom_read_block(data,  addr, num);delay_ms(1);
	sei();
	return TRUE;
}




uint8_t EepromWrite_Block(uint16_t *addr, uint8_t *data, uint8_t num)
{
	cli();
	eeprom_busy_wait();
	eeprom_write_block(data,  addr, num);delay_ms(1);
	sei();
	return TRUE;
}

uint8_t EepromRead_PassWord(uint16_t *addr, uint8_t *data, uint8_t num)
{
	cli();
	eeprom_busy_wait(); 
	eeprom_read_block(data,  addr, num);delay_ms(1);
	sei();
	return TRUE;
}




uint8_t EepromWrite_PassWord(uint16_t *addr, uint8_t *data, uint8_t num)
{
	cli();
	eeprom_busy_wait();
	eeprom_write_block(data,  addr, num);delay_ms(1);
	sei();
	return TRUE;
}



uint8_t EepromWrite_Byte(uint16_t uiAddress, uint8_t ucData)
{
	cli();
	eeprom_busy_wait();
	eeprom_write_byte((uint8_t *)uiAddress, ucData);delay_ms(1);
	sei();
	return TRUE;
}


uint8_t EepromRead_Byte(uint16_t uiAddress, uint8_t *data)
{
	uint8_t ret = 0;
	
	cli();
	eeprom_busy_wait();
	ret = eeprom_read_byte((uint8_t*)uiAddress);delay_ms(1);
	sei();
	return ret;
}


void WriteEEprom_RepaymentDate(uint8_t eepromNum, uint8_t cnt)
{
    uint8_t year, month, day = 0;
    RepayDate_t data;

    year  = ((CurDate.month + cnt) > 12) ? (CurDate.year+ 1) : CurDate.year; 
    
	month = ((CurDate.month + cnt) % 12 == 0) ? 12 : ((CurDate.month + cnt) % 12);

    if (CurDate.day >= 1 && CurDate.day <= 28){
        day = CurDate.day;
    }else{

        if (month == 2 && day >= 29){
            day = 28;
        }else if (month == 4 || month == 6 ||month == 9 || month == 11){
            day = CurDate.day > 30 ? 30 : CurDate.day;
        }else{
            day = CurDate.day;
        }
        
    }

    data.flag = 0xff;
    data.year = year;
    data.month = month;
    data.day = day;
    
    switch(eepromNum){

        case 1:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_1ST, (uint8_t *)&data, 4);
            break;
        case 2:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_2ST, (uint8_t *)&data, 4);
            break;
        case 3:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_3ST, (uint8_t *)&data, 4);
            break;
        case 4:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_4ST, (uint8_t *)&data, 4);
            break;
        case 5:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_5ST, (uint8_t *)&data, 4);
            break;
        case 6:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_6ST, (uint8_t *)&data, 4);
            break;
        case 7:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_7ST, (uint8_t *)&data, 4);
            break;
        case 8:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_8ST, (uint8_t *)&data, 4);
            break;
        case 9:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_9ST, (uint8_t *)&data, 4);
            break;
        case 10:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_10ST, (uint8_t *)&data, 4);
            break;
        case 11:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_11ST, (uint8_t *)&data, 4);
            break;
        case 12:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_12ST, (uint8_t *)&data, 4);
            break;
        
    }
}

uint8_t SetAmortizeAndStore(uint8_t pic, uint8_t ch)
{
	uint8_t i, j = 0;
	uint8_t ret = FALSE;
	uint8_t val = 0;
	static uint8_t setAmortize = 0;
	
	switch(ch){

		case 0:
		case 3:
		case 6:
		case 12:
			
			setAmortize = ch;
			//change display color;
			

			break;


		case 0xfe:
			ret = TRUE;
			break;

			
		case 0xff: //confirm completed set
			
			if(setAmortize == 0){
				//all pay
				EepromWrite_Byte(EEPROM_ADDRESS_TOTAL_NUMBER, setAmortize);
				delay_ms(10);
				val = 0x00;
				EepromWrite_Byte(EEPROM_ADDRESS_TOTAL_SWITCH, val);
				delay_ms(10);
			}else{
				EepromWrite_Byte(EEPROM_ADDRESS_TOTAL_NUMBER, setAmortize);
				delay_ms(10);
				val = 0xBB;
				EepromWrite_Byte(EEPROM_ADDRESS_TOTAL_SWITCH, val);
				delay_ms(10);

				for(i = setAmortize, j = 1; i > 0; i--, j++){
					WriteEEprom_RepaymentDate(i, j);
					delay_ms(10);
				}
			}
			//puts1("Qnum>: ", setAmortize);delay_ms(10);
			//puts1("val>: ", val);delay_ms(10);
			setAmortize = 0;

			ret = TRUE;;
				
			break;

		case 0xa5:

			val = EepromRead_Byte(EEPROM_CHINESE_ENGLISH_FLAG, NULL);
			if(val == TRUE){
				EepromWrite_Byte(EEPROM_CHINESE_ENGLISH_FLAG, FALSE);
			}else{
				EepromWrite_Byte(EEPROM_CHINESE_ENGLISH_FLAG, TRUE);
			}
			delay_ms(1);
			break;

	}	

	return ret;
}








