#include "avr/io.h"
#include "MSG_Queue.h"
#include "config.h"
#include "peripherals.h"
#include "user_hmi.h"
#include "eepromManage.h"
#include "passwordManage.h"
#include <avr/wdt.h>
#include "system_setting_page.h"


volatile ShowParam_Def cure = {
	.frq = 10,
	.egy = 0,
	.times = 100,
	.cnt = 0
};

StructParam_Def WorkStatus = {
	.pic_id = 0,
	.SetEnum = SET_IDLE,
	.kv_flag = CLOSE,
	.trg = TROGGLE_IDLE,
	.p = NULL
};

StructInput_flag_t InFlag = {
	.ecg  = 0,
	.man  = 0,
	.temp = 0,
};

Water_Confirm_t waterConfirm = {
	.dewaterOpenFlag = FALSE,
	.dewaterCloseFlag = FALSE,
	.judgeflag = 0,
};




typedef enum{
	
	PASSWORD_MANAGE_MODE,
	SYSTEM_SETTING_MODE,
	AMORTIZE_MANAGE_MODE,
	SYSTEM_WORK_MODE
	
}System_WorkMode_t;


//system password
PassWordManage_t sysPassword;

//user password
PassWordManage_t userPassword;

//set to pass word manage mode, when start machine.
System_WorkMode_t WorkMode = PASSWORD_MANAGE_MODE; 

extern RepayDate_t CurDate;
extern uint32_t sysToggleCnt;



uint8_t GetEnterReturnPictureID()
{
	uint8_t ret;

	if(WorkStatus.EnChFlag == TRUE){

		switch(WorkStatus.pic_id){
			case 50: ret = 55; break;
			case 52: ret = 56; break;
			case 53: ret = 57; break;
			case 54: ret = 58; break;
			default: ret = WorkStatus.pic_id; break;
		}
		
	}else{
		
		switch(WorkStatus.pic_id){
			case 30: ret = 35; break;
			case 32: ret = 36; break;
			case 33: ret = 37; break;
			case 34: ret = 38; break;
			default: ret = WorkStatus.pic_id; break;
		}
	}

	return ret;
}



uint8_t GetOutReturnPictureID()
{
	uint8_t ret;

	if(WorkStatus.EnChFlag == TRUE){

		switch(WorkStatus.pic_id){
			case 55: ret = 50; break;
			case 56: ret = 52; break;
			case 57: ret = 53; break;
			case 58: ret = 54; break;
			default: ret = WorkStatus.pic_id; break;
		}
		
	}else{
		
		switch(WorkStatus.pic_id){
			case 35: ret = 30; break;
			case 36: ret = 32; break;
			case 37: ret = 33; break;
			case 38: ret = 34; break;
			default: ret = WorkStatus.pic_id; break;
		}
	}

	return ret;
}






uint8_t Confirm_Operate(uint8_t pic, uint8_t ch)
{
	
	uint8_t ret = TRUE;
	

	if(ch == MSG_LCD_COUNTER_SHOW) 
		return TRUE;

	
	switch(ch){
		
		case CFG_LOGO_PAGE_TIMEOUT_ENVET:
			waterConfirm.dewaterOpenFlag = FALSE;
			waterConfirm.dewaterCloseFlag = FALSE;
			ret = FALSE;
			break;


		//DestroyTimeout_Task();	
		case MSG_CTRL_DEWATER_OPEN:
			
			waterConfirm.dewaterCloseFlag = FALSE;

			
			if(waterConfirm.dewaterOpenFlag == FALSE){
				
				waterConfirm.judgeflag = 0;

				//chinese or english
				if(WorkStatus.EnChFlag == TRUE){
					WorkStatus.pic_id = GetEnterReturnPictureID();
					Pic_SwitchTo(WorkStatus.pic_id);
					TankDrainConfirm_SendToMonitor();
					
				}else{
					WorkStatus.pic_id = GetEnterReturnPictureID();
					Pic_SwitchTo(WorkStatus.pic_id);
					TankDrainConfirm_SendToMonitor();
				}				
				ret = FALSE;

			}else{
				DestroyTimeout_Task();
				ret = TRUE;
			}
			break;



		case MSG_CTRL_DEWATER_STOP:
			if(WorkStatus.pic_id == CFG_PICTURE_CHINESE_CONFIRM_ID || WorkStatus.pic_id == CFG_PICTURE_ENGLISH_CONFIRM_ID){
				ret = FALSE;
			}else{
				//waterConfirm.dewaterOpenFlag = FALSE;
				ret = TRUE;
			}
			break;


			
		case MSG_CTRL_WATERFLOODING_OPEN:
			
			waterConfirm.dewaterOpenFlag = FALSE;

			
			if(waterConfirm.dewaterCloseFlag == FALSE){
				
				waterConfirm.judgeflag = 1;

				//chinese or english
				if(WorkStatus.EnChFlag == TRUE){
					WorkStatus.pic_id = GetEnterReturnPictureID();
					Pic_SwitchTo(WorkStatus.pic_id);
					TankInflowConfirm_SendToMonitor();
				}else{
					WorkStatus.pic_id = GetEnterReturnPictureID();
					Pic_SwitchTo(WorkStatus.pic_id);
					TankInflowConfirm_SendToMonitor();
				}
				ret = FALSE;

				
			}else{
				DestroyTimeout_Task();
				ret = TRUE;
			}
			break;


			
		case MSG_CTRL_WATERFLOODING_STOP:
			
			if(WorkStatus.pic_id == CFG_PICTURE_CHINESE_CONFIRM_ID || WorkStatus.pic_id == CFG_PICTURE_ENGLISH_CONFIRM_ID){
				ret = FALSE;
			}else{
				//waterConfirm.dewaterCloseFlag = FALSE;
				ret = TRUE;
			}
			break;



		case 0x01:
			if(pic == 0x55){

				if(WorkStatus.EnChFlag == TRUE){
					WorkStatus.pic_id = GetOutReturnPictureID();
					Pic_SwitchTo(WorkStatus.pic_id);
				}else{
					WorkStatus.pic_id = GetOutReturnPictureID();
					Pic_SwitchTo(WorkStatus.pic_id);
				}
					ret = FALSE;			
				}
			break;



		case 0x02:
			if(pic == 0x55){
				
				if(waterConfirm.judgeflag == 0){
					waterConfirm.dewaterOpenFlag = TRUE;
				}else if (waterConfirm.judgeflag == 1){
					waterConfirm.dewaterCloseFlag = TRUE;
				}

					
				if(WorkStatus.EnChFlag == TRUE){
					WorkStatus.pic_id = GetOutReturnPictureID();
					Pic_SwitchTo(WorkStatus.pic_id);
				}else{
					WorkStatus.pic_id = GetOutReturnPictureID();
					Pic_SwitchTo(WorkStatus.pic_id);
				}
				StartTimeout_Task(WorkStatus.pic_id, 800); // after 8s, if not opterate ,cancel opterate 
				ret = FALSE;
			}
			break;

	}
	//puts1("CH: ", ch);delay_ms(5);
	return ret;
}




/******************************************************************
	* this function complete all of cure-setting and curing operate;
	*maybe next mode will be change
*/
void RunCureMode(uint8_t pic, uint8_t ch)
{

	if(FALSE == Confirm_Operate(pic, ch))
		return;

	switch(ch)
	{
	/*********************************************************/
			case MSG_LCD_COUNTER_SHOW:
				TOGGLE_ENERGY_START();
				delay_ms(19);
				TOGGLE_ENERGY_STOP();
				CounterValue_SendToMonitor();
				sysToggleCnt++;
				Restart_15minsCounter();
				break;
	/*********************************************************/
			case MSG_SHOW_ENERGY_SET:
				if(WorkStatus.SetEnum != SET_ENERGY){
					WorkStatus.SetEnum = SET_ENERGY;
					Option_SelectChange(CFG_OPTION_ENERGY_ID, TRUE);
					Option_SelectChange(CFG_OPTION_FREQUENCY_ID, FALSE);
					Option_SelectChange(CFG_OPTION_TIMES_ID, FALSE);
					Status_SendtoMonitor(OPT_STATUS_BAR_ENERGY_SET);
					ChEn_PictureSwitch(MSG_SHOW_ENERGY_SET, WorkStatus.EnChFlag);
				}else{
					WorkStatus.SetEnum = SET_IDLE;
					Option_SelectChange(CFG_OPTION_ENERGY_ID, FALSE);
					Status_SendtoMonitor(OPT_STATUS_BAR_CLEAR_SET);
					ChEn_PictureSwitchToMain(WorkStatus.EnChFlag);
				}
				break;
				
			case MSG_SHOW_FREQUENCY_SET:
				if(WorkStatus.SetEnum != SET_FRQ){
					WorkStatus.SetEnum = SET_FRQ;
					Option_SelectChange(CFG_OPTION_ENERGY_ID, FALSE);
					Option_SelectChange(CFG_OPTION_FREQUENCY_ID, TRUE);
					Option_SelectChange(CFG_OPTION_TIMES_ID, FALSE);
					Status_SendtoMonitor(OPT_STATUS_BAR_FREQUENCY_SET);
					ChEn_PictureSwitch(MSG_SHOW_FREQUENCY_SET, WorkStatus.EnChFlag);
				}else{
					WorkStatus.SetEnum = SET_IDLE;
					Option_SelectChange(CFG_OPTION_FREQUENCY_ID, FALSE);
					Status_SendtoMonitor(OPT_STATUS_BAR_CLEAR_SET);
					ChEn_PictureSwitchToMain(WorkStatus.EnChFlag);
				}
				break;
				
			case MSG_SHOW_TIMES_SET:
				if(WorkStatus.SetEnum != SET_TIMES){
					WorkStatus.SetEnum = SET_TIMES;
					Option_SelectChange(CFG_OPTION_ENERGY_ID, FALSE);
					Option_SelectChange(CFG_OPTION_FREQUENCY_ID, FALSE);
					Option_SelectChange(CFG_OPTION_TIMES_ID, TRUE);
					Status_SendtoMonitor(OPT_STATUS_BAR_TIMES_SET);
					ChEn_PictureSwitch(MSG_SHOW_TIMES_SET, WorkStatus.EnChFlag);
				}else{
					WorkStatus.SetEnum = SET_IDLE;
					Option_SelectChange(CFG_OPTION_TIMES_ID, FALSE);
					Status_SendtoMonitor(OPT_STATUS_BAR_CLEAR_SET);
					ChEn_PictureSwitchToMain(WorkStatus.EnChFlag);
				}
				break;
			case MSG_SHOW_CLEAR_SET:
				Key_ClearCounter();
				break;
	/*********************************************************/
			case MSG_PARAM_SET_ADD:
				ParamSet_Add();
				break;
			case MSG_PARAM_SET_DEC:
				ParamSet_Dec();
				break;
	/*********************************************************/

			case MSG_CTRL_TOGGLE_OPEN:
				if((OPEN == WorkStatus.kv_flag) && (TROGGLE_IDLE== WorkStatus.trg)){
					Status_SendtoMonitor(OPT_STATUS_BAR_TOGGLING_SET);
					WorkStatus.trg = TROGGLE_TING;
				}
			break;


			case MSG_CTRL_TOGGLE_CLOSE:
				Status_SendtoMonitor(OPT_STATUS_BAR_TEMP_TOGGLE_SET);
				WorkStatus.trg = TROGGLE_IDLE;
			break;

			case MSG_CTRL_KV_OPEN:
				Status_SendtoMonitor(OPT_STATUS_BAR_KVOPEN_SET);
				OPEN_KV_ENERGY();
				WorkStatus.kv_flag = OPEN;
				Restart_15minsCounter();
				break;

			case MSG_CTRL_KV_CLOSE:
				Status_SendtoMonitor(OPT_STATUS_BAR_KVCLOSE_SET);
				Status_SendtoMonitor(OPT_STATUS_BAR_TEMP_TOGGLE_SET);
				CLOSE_KV_ENERGY();
				WorkStatus.kv_flag = CLOSE;
				WorkStatus.trg = TROGGLE_IDLE;
				Start_OneTime_ToggleCounterStore();
				delay_ms(20);
				//toggle one time, avoid stored-energy don't be release.
				TOGGLE_ENERGY_START();
				delay_ms(20);
				TOGGLE_ENERGY_STOP();
				break;

	/***********************************************************/					
	/***********************************************************/
			case MSG_CTRL_ADD_WATER_OPEN:
				TOUCH_ADD_WATER_OPEN();
				Status_SendtoMonitor(OPT_STATUS_BAR_ADD_WATER_SET);
				break;

			case MSG_CTRL_ADD_WATER_STOP:
				TOUCH_ADD_WATER_STOP();
				Status_SendtoMonitor(OPT_STATUS_BAR_WATERING_CLEAR_SET);
				break;
	/***********************************************************/
	/***********************************************************/

			case MSG_CTRL_DEC_WATER_OPEN:
				TOUCH_DEC_WATER_OPEN();
				Status_SendtoMonitor(OPT_STATUS_BAR_DEC_WATER_SET);
				break;

			case MSG_CTRL_DEC_WATER_STOP:
				TOUCH_DEC_WATER_STOP();
				Status_SendtoMonitor(OPT_STATUS_BAR_WATERING_CLEAR_SET);
				break;
	/***********************************************************/
	/***********************************************************/

			case MSG_CTRL_DEWATER_OPEN:
				TOUCH_DEWATER_OPEN();
				Status_SendtoMonitor(OPT_STATUS_BAR_DEWATERING_SET);
				break;

			case MSG_CTRL_DEWATER_STOP:
				TOUCH_DEWATER_STOP();
				Status_SendtoMonitor(OPT_STATUS_BAR_WATERING_CLEAR_SET);
				StartTimeout_Task(WorkStatus.pic_id, 200); // after 2s, if not opterate ,cancel opterate 
				break;
	/***********************************************************/
	/***********************************************************/
				
			case MSG_CTRL_WATERFLOODING_OPEN:
				TOUCH_WATERFLOODING_OPEN();
				Status_SendtoMonitor(OPT_STATUS_BAR_WATER_INJECTION_SET);
				break;
				
			case MSG_CTRL_WATERFLOODING_STOP:
				TOUCH_WATERFLOODING_STOP();
				Status_SendtoMonitor(OPT_STATUS_BAR_WATERING_CLEAR_SET);
				StartTimeout_Task(WorkStatus.pic_id, 200); // after 2s, if not opterate ,cancel opterate 
				break;
	/***********************************************************/
	/***********************************************************/

			case MSG_CTRL_ALERT_OPEN:
				OVER_TEMPERATURE_ALERT_OPEN();
				Status_SendtoMonitor(OPT_STATUS_BAR_ALERTING_SET);
				break;

			case MSG_CTRL_ALERT_CLOSE:
				OVER_TEMPERATURE_ALERT_STOP();
				Status_SendtoMonitor(OPT_STATUS_BAR_ALERTCLEAR_SET);
				break;
	/***********************************************************/
	/***********************************************************/

			case MSG_CTRL_ENERGY_OPEN:
				break;

			case MSG_CTRL_ENERGY_CLOSE:
				break;
	/*********************************************************/

			case MSG_EEPROM_STORE_TOGGLE_COUNTER:
				update_EEprom_ToggleCounterValue();
				break;

				
			default:
				break;
		}
	//puts1("P: ", WorkStatus.pic_id);delay_ms(5);
}


/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
int main()
{
	//uint8_t i;
	MSG_BufferTypeDef msg;
	StatusReturn_t val = IGNORE;
	
	DevInit_PWMOut();
	DevInit_OutputIO();
	DevInit_InputIO();
	USART_Init();
	DevInit_TickTimer(); //start system tick timer 
#if DEBUG_TEST
	uart1_init();
#endif
	Poweron_InitConsig();
	ADC_Init();
	delay_ms(500);
	ReadCurrentDate();
	delay_ms(500);
	WorkStatus.EnChFlag = EepromRead_Byte(EEPROM_CHINESE_ENGLISH_FLAG, NULL);
	delay_ms(5);
	InitStatus_Show();delay_ms(5);
	Pic_SwitchTo(CFG_PICTURE_LOGO_ID);
	delay_ms(500);
	MSG_QueueInit();
	WorkMode = PASSWORD_MANAGE_MODE; 
	StartTimeout_Task(CFG_PICTURE_LOGO_ID, 500);
	wdt_enable(WDTO_2S);

	while(1)
	{

		if(!MSG_QueueIsEmpty())
		{

			MSG_QueueGet(&msg);
			//we must confirm this message come from current picture
			//some times, may be occur interrupe or send to montor time-delay, 
			//locate picture is change, and the screen not, because of sync need time
			switch(WorkMode){


/*******************************************************************************/
/*******************************************************************************/
				case PASSWORD_MANAGE_MODE:
				/**************************************************
					*in this case complete pass word manage, include:
					# 1.how to enter manage page from logo page;
					# 2. enter pass word be enter to system, compare, retry, and enter;
					# 3.set amortize, and store to eeprom
				*/

					switch(msg.pic){

						case CFG_PICTURE_LOGO_ID:

							if (msg.c == CFG_LOGO_PAGE_TIMEOUT_ENVET){
								
								//logo page timeout
								if (TRUE == IS_Popup_AmortizePassWordPage()){
									
									WorkMode = AMORTIZE_MANAGE_MODE;
									Pic_SwitchTo(CFG_AMORTIZE_PW_ENTER_ID);
									update_systemSetting3Data();
									DisplayPaying_AmortizeNumber();
									DisplayPasswordCharToScreen(PURCHASE_PASSWORD,0);
									DisplayRandomCodeToScreen();
									
									
								}else{
								
									WorkMode = SYSTEM_WORK_MODE;
									InitStatus_Show();delay_ms(5);
									if(WorkStatus.EnChFlag == TRUE){
										WorkStatus.pic_id = CFG_PICTURE_MAIN_CHINESE_ID;
										Pic_SwitchTo(CFG_PICTURE_MAIN_CHINESE_ID);
									}else{
										WorkStatus.pic_id = CFG_PICTURE_MAIN_ENGLISH_ID;
										Pic_SwitchTo(CFG_PICTURE_MAIN_ENGLISH_ID);
									}
								}
								EnterSettingPage_Login_TimeoutClear();



							/* ENTER PASSWORD, if correct, enter system setting page*/
							}else{
							
								if (TRUE == EnterSettingPage_Login(msg.c)){

									// if the pass word is true, Enter system login page
									DestroyTimeout_Task();
									DisplayPasswordCharToScreen(SYSTEM_PASSWORD, 0);
									Pic_SwitchTo(CFG_PICTURE_PASSWORD_ID);
								}
							}
							
							break;



						case CFG_PICTURE_PASSWORD_ID:

							val = PassWordPrase(msg.pic, &sysPassword, msg.c);

							if (val == RIGHT){
								//enter amortsize pay page
								WorkMode = SYSTEM_SETTING_MODE;
								Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID_1);
								Display_ChineseEnglish();
								Display_Date_Of_Production(1);
							
							}else if (val == WRONG){

								// hint error, and retry password
								//display "ENTER ERROR, PLEASE RETRY"
								
							}else if (val == DELETE){
							
								// can continue receive message and store it to buffer.
								//backspace and delete a "   *   " 
							
							}else{
							
								//do nothing, it is mean that pass word is not enough, must continue
							}
								
							break;
					}
					break;

					

/*******************************************************************************/
/*******************************************************************************/
				case AMORTIZE_MANAGE_MODE:
				/*************************************************
					# enter password to system and start next work period;
					# 1. receive char, compare pass word and decide to start next period;
					# 2. change eeprom  flag about setting, and store to eeprom
				*/
					val = PassWordPrase(msg.pic, &userPassword, msg.c);
				
					if (val == RIGHT){
						
						//clear this data in eeprom that about the flag, the date.etc
						AlreadyPaid_ClearCurrentStore();
						Sys_RebootMCU();
						//DevInit_TickTimer(); //start system tick timer 
					}else if (val == WRONG){
						// hint error, and retry password
						//display "ENTER ERROR, PLEASE RETRY"
						
					}else if (val == DELETE){

						// can continue receive message and store it to buffer.
						//backspace and delete a "   *   " 

					}else{

						//do nothing, it is mean that pass word is not enough, must continue
					}

					break;



					
/*******************************************************************************/
/*******************************************************************************/
				case SYSTEM_SETTING_MODE:

					Run_SystemSetttingPage(msg.pic, msg.c);
					
					break;


/*******************************************************************************/
/*******************************************************************************/
				case SYSTEM_WORK_MODE:
				/**************************************************
					*normal work, cure mode
				*/
					RunCureMode(msg.pic, msg.c);
						
					break;


				default:
					break;
				
			}
		}


		/* this function can generate some timeout event, and  put its to queue*/
		TimeoutTask_PutToQueue();
		Timeout_15minsHandle();
		wdt_reset();
	}

	return 0;
}






