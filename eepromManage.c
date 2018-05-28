

#include "avr/io.h"
#include <avr/eeprom.h>
#include "eepromManage.h"
#include <avr/interrupt.h>


AmortizeDataBase_t EepromDB;

uint8_t EepromRead_PassWord(uint32_t *addr, uint8_t *data, uint8_t num)
{

	eeprom_busy_wait(); 
	eeprom_read_block(data,  addr, num);

	return TRUE;
}




uint8_t EepromWrite_PassWord(uint32_t *addr, uint8_t *data, uint8_t num)
{

	eeprom_busy_wait();
	cli();
	eeprom_write_block(data,  addr, num);
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


uint8_t AgreementDate_year = 0;
uint8_t AgreementDate_month = 0;
uint8_t AgreementDate_day = 0;

void WriteEEprom_RepaymentDate(uint8_t eepromNum, uint8_t cnt)
{
    uint8_t year, month, day;
    RepayDate_t data;

    year  = ((AgreementDate_month + cnt) > 12) ? (AgreementDate_year + 1) : AgreementDate_year; 
    
	month = ((AgreementDate_month + cnt) % 12 == 0) ? 12 : ((AgreementDate_month + cnt) % 12);

    if (AgreementDate_day >= 1 && AgreementDate_day <= 28){
        day = AgreementDate_day;
    }else{

        if (month == 2 && day >= 29){
            day = 28;
        }else if (month == 4 || month == 6 ||month == 9 || month == 11){
            day = AgreementDate_day > 30 ? 30 : AgreementDate_day;
        }else{
            day = AgreementDate_day;
        }
        
    }

    data.flag = 0xff;
    data.year = year;
    data.month = month;
    data.day = day;
    
    switch(eepromNum){

        case 1:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_1ST, &data, 4);
            break;
        case 2:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_2ST, &data, 4);
            break;
        case 3:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_3ST, &data, 4);
            break;
        case 4:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_4ST, &data, 4);
            break;
        case 5:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_5ST, &data, 4);
            break;
        case 6:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_6ST, &data, 4);
            break;
        case 7:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_7ST, &data, 4);
            break;
        case 8:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_8ST, &data, 4);
            break;
        case 9:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_9ST, &data, 4);
            break;
        case 10:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_10ST, &data, 4);
            break;
        case 11:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_11ST, &data, 4);
            break;
        case 12:
            EepromWrite_PassWord(EEPROM_ADDRESS_DATE_12ST, &data, 4);
            break;
        
    }
}
uint8_t SetAmortizeAndStore(uint8_t pic, uint8_t ch)
{
	uint8_t i, j;
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

			for(i = setAmortize, j = 1; i > 0; i--, j++){
				WriteEEprom_RepaymentDate(i, j);
			}
			setAmortize = 0;

			return TRUE;
				
			break;

	}	

	return FALSE;
}








