
#include "avr/io.h"
#include <stdlib.h>
#include <string.h>
#include "MSG_Queue.h"
#include "config.h"
#include "peripherals.h"
#include "user_hmi.h"
#include "eepromManage.h"
#include "passwordManage.h"
#include <avr/wdt.h>
#include "system_setting_page.h"




static uint8_t systemSetting1_Item(uint8_t ch)
{
	uint8_t ret = FALSE;
	
	if (ch == 0x02){
		Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_2);
		ret = TRUE;
		
	}else if (ch == 0x03){
		Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_3);
		ret = TRUE;
	}
	
	return ret;
}




static uint8_t systemSetting2_Item(uint8_t ch)
{
	uint8_t ret = FALSE;
	
	if (ch == 0x01){
		Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_1);
		ret = TRUE;
		
	}else if (ch == 0x03){
		Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_3);
		ret = TRUE;
	}
	
	return ret;
}




static uint8_t systemSetting3_Item(uint8_t ch)
{
	uint8_t ret = FALSE;
	
	if (ch == 0x01){
		Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_1);
		ret = TRUE;
		
	}else if (ch == 0x02){
		Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_2);
		ret = TRUE;
	}
	
	return ret;
}







void Display_Date_Of_Production(uint8_t flg)
{
	//uint8_t val;
	uint8_t StatusBar_StatusBuf[25] = {0x5A,0xA5,0x0d,0x82,0x02, 0x00,0xCD,0xA3,0xCD,0xA3, 0xCD,0xA3,0xCD,0xA3};

	StatusBar_StatusBuf[4]	= 0x0F;StatusBar_StatusBuf[5]  = 0x10;
	memset(&StatusBar_StatusBuf[6], 0, 19);

	if(flg){
		EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_PRODT, &StatusBar_StatusBuf[6], 10);
		if(StatusBar_StatusBuf[6] != '2')
			memcpy((char*)&StatusBar_StatusBuf[6], "2018-01-01", sizeof("xxxx-xx-xx"));
	}else{
		memcpy((char*)&StatusBar_StatusBuf[6], "          ", sizeof("xxxx-xx-xx"));
	}
	
	SendToMonitor(StatusBar_StatusBuf,25);
}





void Display_CheseEnglish()
{
	uint8_t val;
	uint8_t StatusBar_StatusBuf[25] = {0x5A,0xA5,0x0b,0x82,0x02, 0x00,0xCD,0xA3,0xCD,0xA3, 0xCD,0xA3,0xCD,0xA3};

	
	StatusBar_StatusBuf[4]	= 0x0F;StatusBar_StatusBuf[5]  = 0x00;
	memset(&StatusBar_StatusBuf[6], 0, 19);
	
	val = EepromRead_Byte(EEPROM_CHINESE_ENGLISH_FLAG, NULL);
	if(val == TRUE){
		memcpy(&StatusBar_StatusBuf[6],"Chinese", sizeof("Chinese"));
	}else{
		memcpy(&StatusBar_StatusBuf[6],"English", sizeof("English"));
	}
	SendToMonitor(StatusBar_StatusBuf,25);
}





static DatePrdt_t datePrdt;
static void Modify_DateProduction(uint8_t ch)
{
	uint8_t StatusBar_StatusBuf[25] = {0x5A,0xA5,0x0d,0x82,0x02, 0x00,0xCD,0xA3,0xCD,0xA3, 0xCD,0xA3,0xCD,0xA3};
	uint8_t flag = 0;
	uint8_t i = 0;
	
	switch(ch){
		case 0:case 1:case 2:case 3:
		case 4:case 5:case 6:case 7:
		case 8:case 9:

			if(datePrdt.cnt == 10) break;
			
			datePrdt.data[datePrdt.cnt] = ch + '0';
			datePrdt.cnt++;
			if (datePrdt.cnt == 4) datePrdt.cnt = 5;
			if (datePrdt.cnt == 7) datePrdt.cnt = 8;
			
			//for (i =  datePrdt.cnt; i < 10; i++)
			//	datePrdt.data[i] = 'x';

			
			flag = 1;
			break;


		case 0x0b:
			memset(&datePrdt, 0, sizeof(DatePrdt_t));
			Display_Date_Of_Production(0);
			break;


		case 0x0a:
			Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_1);
			Display_Date_Of_Production(1);
			break;


		case 0x0c:
			//write value to eeprom
			EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_PRODT, (uint8_t *)&datePrdt.data, 10);
			
			Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_1);
			Display_Date_Of_Production(1);
			break;
			
	}


	//update display
	if(flag){
		
		StatusBar_StatusBuf[4]	= 0x0F;StatusBar_StatusBuf[5]  = 0x10;
		memset(&StatusBar_StatusBuf[6], 0, 19);
		datePrdt.data[4] = '-';
		datePrdt.data[7] = '-';
		memcpy((char*)&StatusBar_StatusBuf[6],(char*)&datePrdt.data, 10);
		
		SendToMonitor(StatusBar_StatusBuf,25);
	}
	
}












































void Run_SystemSetttingPage(uint8_t pic, uint8_t ch)
{


	switch(pic){


		case CFG_PICTURE_DATE_PRODUCTION:
			Modify_DateProduction(ch);
			break;

			

		case CFG_PICTURE_PUR_SETTING_ID_1:

			if(TRUE == systemSetting1_Item(ch))
				break;
			

			if (ch == 0xa5){ //system setting --> chinese / english
				ChEnSwitch();
			}else if (ch == 0xa6){ // system setting ---> date of production
				Pic_SwitchTo(CFG_PICTURE_DATE_PRODUCTION);
				memset(&datePrdt, 0, sizeof(DatePrdt_t));
				Display_Date_Of_Production(0);
			}

			break;
			
		case CFG_PICTURE_PUR_SETTING_ID_2:

			if(TRUE == systemSetting2_Item(ch))
				break;

			break;
			
		case CFG_PICTURE_PUR_SETTING_ID_3:

			if(TRUE == systemSetting3_Item(ch))
				break;
			
			
			//if set seccuss ---> reboot¡
			/*
			if(TRUE == SetAmortizeAndStore(msg.pic, msg.c)){

				Sys_RebootMCU();
				
			}
			*/
			break;

		}

}























