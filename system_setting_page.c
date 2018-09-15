
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




static void getDisplayAddr(uint8_t *addr, uint8_t num)
{

	switch(num){

		case 0:
			*addr++ = 0x0E;*addr = 0x00;
			break;

		case 1:
			*addr++ = 0x0E;*addr = 0x20;
			break;

		case 2:
			*addr++ = 0x0E;*addr = 0x40;
			break;

		case 3:
			*addr++ = 0x0E;*addr = 0x60;
			break;

		case 4:
			*addr++ = 0x0E;*addr = 0x80;
			break;

		case 5:
			*addr++ = 0x0E;*addr = 0xA0;
			break;
	}
}





static Amortize_Date_t amortizeDate[6];
static void update_systemSetting3Data(void)
{

	uint8_t i;
	uint8_t StatusBar_StatusBuf[25] = {0x5A,0xA5,0x0d,0x82,0x02, 0x00,0xCD,0xA3,0xCD,0xA3, 0xCD,0xA3,0xCD,0xA3};
	

	EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_1ST, (uint8_t*)&amortizeDate[0], 4);
	EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_2ST, (uint8_t*)&amortizeDate[1], 4);
	EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_3ST, (uint8_t*)&amortizeDate[2], 4);
	EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_4ST, (uint8_t*)&amortizeDate[3], 4);
	EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_5ST, (uint8_t*)&amortizeDate[4], 4);
	EepromRead_Block((uint16_t*)EEPROM_ADDRESS_DATE_6ST, (uint8_t*)&amortizeDate[5], 4);
	

	for (i = 0; i < 6; i++){

		if (amortizeDate[i].flg == 1 || amortizeDate[i].flg == 2){
			
			getDisplayAddr(&StatusBar_StatusBuf[4], i);
			memset(&StatusBar_StatusBuf[6], 0, 19);
			//sprintf((char*)&StatusBar_StatusBuf[6], "20%02d-%02d-%02d", amortizeDate[i].year, amortizeDate[i].mon, amortizeDate[i].day);
			SendToMonitor(StatusBar_StatusBuf,25);


			getDisplayAddr(&StatusBar_StatusBuf[4], i);
			StatusBar_StatusBuf[5] += 0x10;
			memset(&StatusBar_StatusBuf[6], 0, 19);
			
			if (amortizeDate[i].flg == 1)
				memcpy((char*)&StatusBar_StatusBuf[6], "YES", sizeof("YES"));
			else if (amortizeDate[i].flg == 2) 
				memcpy((char*)&StatusBar_StatusBuf[6], "NO", sizeof("NO"));

			SendToMonitor(StatusBar_StatusBuf,25);

			
		}else{

			getDisplayAddr(&StatusBar_StatusBuf[4], i);
			memset(&StatusBar_StatusBuf[6], 0, 19);
			memcpy((char*)&StatusBar_StatusBuf[6], "          ", sizeof("xxxx-xx-xx"));
			SendToMonitor(StatusBar_StatusBuf,25);

			getDisplayAddr(&StatusBar_StatusBuf[4], i);
			StatusBar_StatusBuf[5] += 0x10;
			memset(&StatusBar_StatusBuf[6], 0, 19);
			memcpy((char*)&StatusBar_StatusBuf[6], "    ", sizeof("    "));
			SendToMonitor(StatusBar_StatusBuf,25);
			
		}
		
	}

}





static uint8_t systemSetting1_Item(uint8_t ch)
{
	uint8_t ret = FALSE;
	
	if (ch == 0x02){
		Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_2);
		ret = TRUE;
		
	}else if (ch == 0x03){
		Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_3);
		update_systemSetting3Data();
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
		update_systemSetting3Data();
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






static uint8_t AmortizeModifyNum = 0;
static struct {
	uint8_t flag;
	uint8_t data[10];
}dateAmrtMdy;
static uint8_t updateAmortizeModify_Page()
{
	uint8_t StatusBar_StatusBuf[25] = {0x5A,0xA5,0x0d,0x82,0x02, 0x00,0xCD,0xA3,0xCD,0xA3, 0xCD,0xA3,0xCD,0xA3};

	StatusBar_StatusBuf[4]	= 0x0E;StatusBar_StatusBuf[5]  = 0xC0;
	memset(&StatusBar_StatusBuf[6], 0, 19);
	dateAmrtMdy.data[0] = '2';
	dateAmrtMdy.data[1] = '0';
	dateAmrtMdy.data[4] = '-';
	dateAmrtMdy.data[7] = '-';
	memcpy((char*)&StatusBar_StatusBuf[6],(char*)&dateAmrtMdy.data, 10);
	SendToMonitor(StatusBar_StatusBuf,25);



	StatusBar_StatusBuf[4]	= 0x0E;StatusBar_StatusBuf[5]  = 0xD0;
	memset(&StatusBar_StatusBuf[6], 0, 19);
	if (dateAmrtMdy.flag == 1)
		memcpy((char*)&StatusBar_StatusBuf[6],"YES", sizeof("YES"));
	else if (dateAmrtMdy.flag == 2)
		memcpy((char*)&StatusBar_StatusBuf[6],"NO", sizeof("NO"));
	else memcpy((char*)&StatusBar_StatusBuf[6],"NOP", sizeof("NOP"));
	SendToMonitor(StatusBar_StatusBuf,25);	
}




void update_ModifyDataToEEprom(uint8_t num)
{
	uint8_t data[4];


	data[0] = dateAmrtMdy.flag;
	data[1] = 10*(dateAmrtMdy.data[2] - '0') + (dateAmrtMdy.data[3] - '0');
	data[2] = 10*(dateAmrtMdy.data[5] - '0') + (dateAmrtMdy.data[6] - '0');;
	data[3] = 10*(dateAmrtMdy.data[8] - '0') + (dateAmrtMdy.data[9] - '0');;

	
	switch(num){
		case 0:EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_1ST, data, 4);break;
		case 1:EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_2ST, data, 4);break;
		case 2:EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_3ST, data, 4);break;
		case 3:EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_4ST, data, 4);break;
		case 4:EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_5ST, data, 4);break;
		case 5:EepromWrite_Block((uint16_t*)EEPROM_ADDRESS_DATE_6ST, data, 4);break;
	}
	
}




static void Modify_DateAmortize(uint8_t ch)
{
	uint8_t StatusBar_StatusBuf[25] = {0x5A,0xA5,0x0d,0x82,0x02, 0x00,0xCD,0xA3,0xCD,0xA3, 0xCD,0xA3,0xCD,0xA3};
	uint8_t flag = 0;
	static uint8_t counter = 2;
	
	switch(ch){
		case 0:case 1:case 2:case 3:
		case 4:case 5:case 6:case 7:
		case 8:case 9:

			if(counter == 10) break;
			
			dateAmrtMdy.data[counter] = ch + '0';
			counter++;
			if (counter == 4) counter = 5;
			if (counter == 7) counter = 8;	
			flag = 1;
			break;


		case 0x0D:
			if (dateAmrtMdy.flag++ >= 2)
				dateAmrtMdy.flag = 0;

			flag = 1;
			break;


		case 0x0B:
			memset(&dateAmrtMdy, 0, sizeof(dateAmrtMdy));
			counter = 2;
			updateAmortizeModify_Page();
			break;


		case 0x0A:
			Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_3);
			counter = 2;
			update_systemSetting3Data();
			break;


		case 0x0C:
			//write value to eeprom
			update_ModifyDataToEEprom(AmortizeModifyNum);
			counter = 2;
			Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_3);
			update_systemSetting3Data();
			break;
			
	}


	//update display
	if(flag){
		updateAmortizeModify_Page();
	}
	
}





void Run_SystemSetttingPage(uint8_t pic, uint8_t ch)
{


	switch(pic){


		case CFG_PICTURE_DATE_PRODUCTION:

			Modify_DateProduction(ch);
			break;



		case CFG_PICTURE_DATE_AMORTIZE:

			Modify_DateAmortize(ch);
			break;
			


		case CFG_PICTURE_PUR_SETTING_ID_1:

			if(TRUE == systemSetting1_Item(ch))
				break;
			

			if (ch == 0xFF){ // if exit be press, exit()

				Sys_RebootMCU();
				

			}else if (ch == 0xa5){ //system setting --> chinese / english

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


			if (ch == 0xFF){ // if exit be press, exit()

				Sys_RebootMCU();
				
			}
			
			break;



			
		case CFG_PICTURE_PUR_SETTING_ID_3:

			if(TRUE == systemSetting3_Item(ch))
				break;


			if (ch == 0xFF){ // if exit be press, exit()

				Sys_RebootMCU();
				
			}else{
			
				Pic_SwitchTo(CFG_PICTURE_DATE_AMORTIZE);
				AmortizeModifyNum = (ch - 0x50);
				memset(&dateAmrtMdy, 0, sizeof(dateAmrtMdy));
				updateAmortizeModify_Page();
				
			}
			
			break;

		}

}























