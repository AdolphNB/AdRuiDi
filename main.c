#include "avr/io.h"
#include "MSG_Queue.h"
#include "config.h"
#include "peripherals.h"
#include "user_hmi.h"
#include "eepromManage.h"
#include "passwordManage.h"
#include <avr/wdt.h>

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





typedef enum{
	
	PASSWORD_MANAGE_MODE,
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

void Sys_RebootMCU()
{
	// infinite loop, cause watch dog timer ---> timeout, and reboot
	while(1);
}


/******************************************************************
	* this function complete all of cure-setting and curing operate;
	*maybe next mode will be change
*/
void RunCureMode(uint8_t pic, uint8_t ch)
{

	switch(ch)
	{
	/*********************************************************/
			case MSG_LCD_COUNTER_SHOW:
				TOGGLE_ENERGY_START();
				CounterValue_SendToMonitor();
				delay_ms(19);
				TOGGLE_ENERGY_STOP();
				break;
	/*********************************************************/
			case MSG_SHOW_ENERGY_SET:
				if(WorkStatus.SetEnum != SET_ENERGY){
					WorkStatus.SetEnum = SET_ENERGY;
					Option_SelectChange(CFG_OPTION_ENERGY_ID, TRUE);
					Option_SelectChange(CFG_OPTION_FREQUENCY_ID, FALSE);
					Option_SelectChange(CFG_OPTION_TIMES_ID, FALSE);
					Status_SendtoMonitor(OPT_STATUS_BAR_ENERGY_SET);
				}else{
					WorkStatus.SetEnum = SET_IDLE;
					Option_SelectChange(CFG_OPTION_ENERGY_ID, FALSE);
					Status_SendtoMonitor(OPT_STATUS_BAR_CLEAR_SET);
				}
				break;
				
			case MSG_SHOW_FREQUENCY_SET:
				if(WorkStatus.SetEnum != SET_FRQ){
					WorkStatus.SetEnum = SET_FRQ;
					Option_SelectChange(CFG_OPTION_ENERGY_ID, FALSE);
					Option_SelectChange(CFG_OPTION_FREQUENCY_ID, TRUE);
					Option_SelectChange(CFG_OPTION_TIMES_ID, FALSE);
					Status_SendtoMonitor(OPT_STATUS_BAR_FREQUENCY_SET);
				}else{
					WorkStatus.SetEnum = SET_IDLE;
					Option_SelectChange(CFG_OPTION_FREQUENCY_ID, FALSE);
					Status_SendtoMonitor(OPT_STATUS_BAR_CLEAR_SET);
				}
				break;
				
			case MSG_SHOW_TIMES_SET:
				if(WorkStatus.SetEnum != SET_TIMES){
					WorkStatus.SetEnum = SET_TIMES;
					Option_SelectChange(CFG_OPTION_ENERGY_ID, FALSE);
					Option_SelectChange(CFG_OPTION_FREQUENCY_ID, FALSE);
					Option_SelectChange(CFG_OPTION_TIMES_ID, TRUE);
					Status_SendtoMonitor(OPT_STATUS_BAR_TIMES_SET);
				}else{
					WorkStatus.SetEnum = SET_IDLE;
					Option_SelectChange(CFG_OPTION_TIMES_ID, FALSE);
					Status_SendtoMonitor(OPT_STATUS_BAR_CLEAR_SET);
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
			case MSG_CTRL_TOGGLE_OPEN_CLOSE:
				if((OPEN == WorkStatus.kv_flag) && (TROGGLE_IDLE== WorkStatus.trg)){
					Status_SendtoMonitor(OPT_STATUS_BAR_TOGGLING_SET);
					WorkStatus.trg = TROGGLE_TING;
				}else {
					Status_SendtoMonitor(OPT_STATUS_BAR_TEMP_TOGGLE_SET);
					WorkStatus.trg = TROGGLE_IDLE;
				}
				break;

			case MSG_CTRL_KV_OPEN:
				Status_SendtoMonitor(OPT_STATUS_BAR_KVOPEN_SET);
				OPEN_KV_ENERGY();
				WorkStatus.kv_flag = OPEN;
				break;

			case MSG_CTRL_KV_CLOSE:
				Status_SendtoMonitor(OPT_STATUS_BAR_KVCLOSE_SET);
				Status_SendtoMonitor(OPT_STATUS_BAR_TEMP_TOGGLE_SET);
				CLOSE_KV_ENERGY();
				WorkStatus.kv_flag = CLOSE;
				WorkStatus.trg = TROGGLE_IDLE;
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
			default:
				break;
		}
}

















#if 0

int main()
{

 	uint8_t qqq = 0, bb = 0;
	
	
	uart1_init(); 
	
//	EepromWrite_Byte(EEPROM_ADDRESS_TOTAL_SWITCH, 155);
	delay_ms(500);
	qqq = EepromRead_Byte(EEPROM_ADDRESS_TOTAL_SWITCH, &bb);
	delay_ms(5);
	puts1("DD: ",qqq++);
	EepromWrite_Byte(EEPROM_ADDRESS_TOTAL_SWITCH, qqq);
	
	while(1);

	return 0;
}	
#else

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
	InitStatus_Show();
	Pic_SwitchTo(CFG_PICTURE_LOGO_ID);
	delay_ms(500);
	//InitStatus_Show();

#if DEBUG_TEST
	puts1("YY: ", CurDate.year);delay_ms(5);
	puts1("MM: ", CurDate.month);delay_ms(5);
	puts1("DD: ", CurDate.day);delay_ms(5);
#endif
	
	MSG_QueueInit();
	WorkMode = PASSWORD_MANAGE_MODE; 
	StartTimeout_Task(CFG_PICTURE_LOGO_ID, 500);
	wdt_enable(WDTO_2S);

	while(1)
	{

		if(!MSG_QueueIsEmpty())
		{

			MSG_QueueGet(&msg);
			//we must confire this message come from current picture
			//some times, may be occur interrupe or send to montor time-delay, 
			//locate picture is change, and the screen not, because of sync need time
			//if (msg.picture != current_picture)
			//	continue;
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
									DisplayPasswordCharToScreen(PURCHASE_PASSWORD,0);
									DisplayRandomCodeToScreen();
									
								}else{
								
									WorkMode = SYSTEM_WORK_MODE;
									Pic_SwitchTo(CFG_PICTURE_MAIN_ID);
								}
								
								EnterSettingPage_Login_TimeoutClear();


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
								Pic_SwitchTo(CFG_PICTURE_PUR_SETTING_ID);
							
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



						case CFG_PICTURE_PUR_SETTING_ID:
							
							//if set seccuss ---> reboot
							if(TRUE == SetAmortizeAndStore(msg.pic, msg.c)){

								Sys_RebootMCU();
								
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
						WorkMode = SYSTEM_WORK_MODE;
						Pic_SwitchTo(CFG_PICTURE_MAIN_ID);
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
		wdt_reset();
	}

	return 0;
}

#endif





