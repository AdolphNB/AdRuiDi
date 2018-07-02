#include "avr/io.h"
#include "MSG_Queue.h"
#include "config.h"
#include "eepromManage.h"
#include "peripherals.h"
#include "user_hmi.h"
#include "string.h"
#include "passwordManage.h"
#include <stdio.h>
#include <stdlib.h>



#define ENTER_DELETE	0X0A
#define ENTER_ENTER		0X0B







void DisplayPasswordCharToScreen(uint8_t pic, uint8_t num)
{
//5a a5 0b 82 0a 20 30 31 34 35 36 39 37 34	
	uint8_t data[14] = {0x5a,0xa5,0x0b,0x82,0x0a,0x20,};
	uint8_t i = 0;

	if(pic == SYSTEM_PASSWORD){
		data[5] = 0x00;
	}else if(pic == PURCHASE_PASSWORD){
		data[5] = 0x20;
	}


	for(i = 0; i < num; i++){
		data[i+6] = data[i+6]+'*';
	}
	SendToMonitor(data,14);
}




extern uint32_t System_Tick;
static uint8_t ReadPassWord(uint8_t pic, uint8_t * data)
{
	static uint32_t num = 0;
	static uint32_t num1 = 0;


	if(num1 == 0 && num == 0){
		
		srand(ADC_read());
		num = rand();
		num1 = rand();
		data[0] = num % 10; data[1] = (num >> 1) % 10; data[2] = (num >> 2) % 10; data[3] = (num >> 3) % 10;
		data[4] = num1 % 10; data[5] = (num1 >> 1) % 10; data[6] = (num1 >> 2) % 10; data[7] = (num1 >> 3) % 10;
		//36898947
		//42423480
	}else{ 
		// +  * * /
		data[0] = ((num % 10) + 1) % 10; data[1] = (((num >> 1) % 10) * 2) % 10;
		data[2] = (((num >> 2) % 10) * 3) % 10; data[3] = (((num >> 3) % 10) / 4) % 10;
		// +  * * /
		data[4] = ((num1 % 10) + 5) % 10; data[5] = (((num1 >> 1) % 10) * 6) % 10; 
		data[6] = (((num1 >> 2) % 10)* 7) % 10; data[7] = (((num1 >> 3) % 10) / 8) % 10;
	}
	
	return TRUE;
}





static uint8_t GetSystePassWord(uint8_t pic, uint8_t * data)
{

	data[0] = 1;data[1] = 2;data[2] = 3;data[3] = 4;
	data[4] = 5;data[5] = 6;data[6] = 7;data[7] = 8;

	return TRUE;

}



static uint8_t ComparePassWord(uint8_t pic, PassWordManage_t *pw)
{
	uint8_t data[8];
	
	switch(pic){

		case SYSTEM_PASSWORD: 
			
			if(TRUE == GetSystePassWord(SYSTEM_PASSWORD, data)){
				if(0 == strncmp((char *)data, (char *)pw->data, 8))
					return TRUE;
			}
			break;

			
		case PURCHASE_PASSWORD:
			
			if(TRUE == ReadPassWord(PURCHASE_PASSWORD, data)){
				if(0 == strncmp((char *)data, (char *)pw->data, 8))
					return TRUE;
			}
			break;

		
		default:break;

	}
	return FALSE;
}



StatusReturn_t PassWordPrase(uint8_t pic, PassWordManage_t *pw, uint8_t ch)
{

	switch(ch){

		case 0:case 1:case 2:case 3:
		case 4:case 5:case 6:case 7:
		case 8:case 9:
			
			if(pw->cnt < 8){
				
				pw->data[pw->cnt] = ch;  // store this char.
				pw->cnt++;
				
			}
			DisplayPasswordCharToScreen(pic, pw->cnt);
			break;		


		case ENTER_DELETE:
			
			if(pw->cnt > 0 && pw->cnt <= 8){
				
				pw->cnt -= 1;
			
			}else pw->cnt = 0;
			DisplayPasswordCharToScreen(pic, pw->cnt);
			return DELETE;
			
			break;


		case ENTER_ENTER:
			
			if(pw->cnt == PASSWORD_LEN){
				
				if (TRUE == ComparePassWord(pic, pw)){
					
					memset(pw, 0, sizeof(PassWordManage_t)); //clear password
					return RIGHT;	//enter completed, and this password is correctly.
				}
			}
			
			memset(pw, 0, sizeof(PassWordManage_t)); //ERROR: clear this data structure
			DisplayPasswordCharToScreen(pic, pw->cnt);
			
			return WRONG;
			
			break;

	}
	
	
	return IGNORE;
}




typedef struct{

	uint8_t cnt;
	uint8_t data[4];
}Logo_EnterSystemLogin_t;

static Logo_EnterSystemLogin_t LogoLogin = {
	.cnt = 0,
	.data[0] = 0
};



uint8_t EnterSettingPage_Login(uint8_t ch)
{
	
	uint8_t ret =  FALSE;

	
	LogoLogin.data[LogoLogin.cnt] = ch;
	LogoLogin.cnt++;


	if (LogoLogin.cnt == 4){

		if (LogoLogin.data[0] == 0x01 && LogoLogin.data[1] == 0x02 && \
			LogoLogin.data[2] == 0x03 && LogoLogin.data[3] == 0x04){
			
			ret = TRUE;
		}
		
		memset(&LogoLogin, 0, sizeof(Logo_EnterSystemLogin_t));
	}


	return ret;
}


/*************************************************************
* if time out in logo page, you must clear the buffer of store keys;
*/
uint8_t EnterSettingPage_Login_TimeoutClear(void)
{

	memset(&LogoLogin, 0, sizeof(Logo_EnterSystemLogin_t));
	
	return 0;
}





/***************************************************************************
*	Blow code is about read flag, and judge whether enter AMORTIZE MODE.
*	these codes include:
*	#	1.read the SWITCH about whether amortize-pay is OVER;
*	#	2.judge current data whether is the time-node  that enter password unlock next period;
***************************************************************************/


uint8_t ReadEEprom_DateData(uint8_t times, RepayDate_t *data)
{
    switch(times){

        case 1:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_1ST, (uint8_t *)data, 4);
            break;
        case 2:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_2ST, (uint8_t *)data, 4);
            break;
        case 3:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_3ST, (uint8_t *)data, 4);
            break;
        case 4:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_4ST, (uint8_t *)data, 4);
            break;
        case 5:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_5ST, (uint8_t *)data, 4);
            break;
        case 6:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_6ST, (uint8_t *)data, 4);
            break;
        case 7:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_7ST, (uint8_t *)data, 4);
            break;
        case 8:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_8ST, (uint8_t *)data, 4);
            break;
        case 9:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_9ST, (uint8_t *)data, 4);
            break;
        case 10:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_10ST, (uint8_t *)data, 4);
            break;
        case 11:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_11ST, (uint8_t *)data, 4);
            break;
        case 12:
            EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_12ST, (uint8_t *)data, 4);
            break;
        
    }
    
    return FALSE;
}


//Date_ReadBack_t CurDate;
extern volatile RepayDate_t CurDate;
static uint8_t Judge_PasswordTimeNode(uint8_t times)
{
	
	//CurDate.flag = TRUE;
	RepayDate_t data;

	
    ReadEEprom_DateData(times, &data);

	/***********************************************************
		if the date is equal to UNLOCKDate    or    more than  UNLOCKDate,
		and the LOCK_FLAG is not be clear,    will return TRUE
	*/
	if (data.flag == 0)
        return FALSE;

    if (CurDate.day == data.day && CurDate.month == data.month && CurDate.year == data.year){
        return TRUE;
    }

	return FALSE;
}



uint8_t IS_Popup_AmortizePassWordPage(void)
{

	uint8_t MasterSwitch = 0;
	uint8_t RemainTimes = 0;

    MasterSwitch = EepromRead_Byte(EEPROM_ADDRESS_TOTAL_SWITCH, NULL);
	delay_ms(10);
    RemainTimes = EepromRead_Byte(EEPROM_ADDRESS_TOTAL_NUMBER, NULL);
	delay_ms(10);
	//return TRUE;

	puts1(">: ", MasterSwitch);delay_ms(10);
	puts1(">: ", RemainTimes);delay_ms(10);
	if (MasterSwitch ==  0 || RemainTimes == 0){

		//  the master switch is close, or the remain times is 0, that means USER don't need enter password
		return FALSE;
	}

	if (FALSE == Judge_PasswordTimeNode(RemainTimes)){

		// it isn't time to unlock the next proid 
		return FALSE;
	}

	
	return TRUE;
	
}



uint8_t ClearEEprom_DateData(uint8_t times, RepayDate_t *data)
{
    switch(times){

        case 1:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_1ST, (uint8_t *)data, 4);
            break;
        case 2:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_2ST, (uint8_t *)data, 4);
            break;
        case 3:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_3ST, (uint8_t *)data, 4);
            break;
        case 4:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_4ST, (uint8_t *)data, 4);
            break;
        case 5:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_5ST, (uint8_t *)data, 4);
            break;
        case 6:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_6ST, (uint8_t *)data, 4);
            break;
        case 7:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_7ST, (uint8_t *)data, 4);
            break;
        case 8:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_8ST, (uint8_t *)data, 4);
            break;
        case 9:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_9ST, (uint8_t *)data, 4);
            break;
        case 10:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_10ST, (uint8_t *)data, 4);
            break;
        case 11:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_11ST, (uint8_t *)data, 4);
            break;
        case 12:
            EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_12ST, (uint8_t *)data, 4);
            break;
        
    }
    
    return FALSE;
}




void AlreadyPaid_ClearCurrentStore(void)
{
    uint8_t MasterSwitch;
	uint8_t RemainTimes;
	uint8_t val = 0;
    RepayDate_t data;

    EepromRead_Byte(EEPROM_ADDRESS_TOTAL_SWITCH, &MasterSwitch);
    EepromRead_Byte(EEPROM_ADDRESS_TOTAL_NUMBER, &RemainTimes);

    memset(&data, 0, sizeof(RepayDate_t));
    ClearEEprom_DateData(RemainTimes, &data);
    
    RemainTimes--;
    if(RemainTimes == 0){
		val = 0;
        EepromWrite_Byte(EEPROM_ADDRESS_TOTAL_SWITCH, val);
    }
    EepromWrite_Byte(EEPROM_ADDRESS_TOTAL_NUMBER, RemainTimes);
    
}




void DisplayRandomCodeToScreen()
{
//5a a5 0b 82 0a 20 30 31 34 35 36 39 37 34	
	uint8_t data[14] = {0x5a,0xa5,0x0b,0x82,0x0a,0x10,0x00,};
	uint8_t i = 0;

	
	ReadPassWord(PURCHASE_PASSWORD, &data[6]);

	for(i = 0; i < 8; i++){
		data[i+6] = data[i+6]+'0';
	}
	
	SendToMonitor(data,14);
#if 0
	puts1("0:", data[0]);delay_ms(20);
	puts1("1:", data[1]);delay_ms(20);
	puts1("2:", data[2]);delay_ms(20);
	puts1("3:", data[3]);delay_ms(20);
	puts1("4:", data[4]);delay_ms(20);
	puts1("5:", data[5]);delay_ms(20);
	puts1("6:", data[6]);delay_ms(20);
	puts1("7:", data[7]);delay_ms(20);
#endif	
}






