#include "avr/io.h"
#include "MSG_Queue.h"
#include "config.h"
#include "peripherals.h"
#include "user_hmi.h"
#include "eepromManage.h"


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


void delay_ms(unsigned int ms)
{
	unsigned int i = 192;
	while(ms--)
	{
		for(i = 192; i > 0; i--);
	}
}


typedef enum{
	
	PASSWORD_MANAGE_MODE,
	AMORTIZE_MANAGE_MODE,
	SYSTEM_WORK_MODE
	
}System_WorkMode_t;



//set to pass word manage mode, when start machine.
System_WorkMode_t WorkMode = PASSWORD_MANAGE_MODE; 




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
					Pic_SwitchTo(CFG_PICTURE_ENERGY_ID);
					WorkStatus.pic_id = CFG_PICTURE_ENERGY_ID;
					Status_SendtoMonitor(OPT_STATUS_BAR_ENERGY_SET);
				}else{
					WorkStatus.SetEnum = SET_IDLE;
					Pic_SwitchTo(CFG_PICTURE_MAIN_ID);
					WorkStatus.pic_id = CFG_PICTURE_MAIN_ID;
					Status_SendtoMonitor(OPT_STATUS_BAR_CLEAR_SET);
				}
				break;
				
			case MSG_SHOW_FREQUENCY_SET:
				if(WorkStatus.SetEnum != SET_FRQ){
					WorkStatus.SetEnum = SET_FRQ;
					Pic_SwitchTo(CFG_PICTURE_FREQUENCY_ID);
					WorkStatus.pic_id = CFG_PICTURE_FREQUENCY_ID;
					Status_SendtoMonitor(OPT_STATUS_BAR_FREQUENCY_SET);
				}else{
					WorkStatus.SetEnum = SET_IDLE;
					Pic_SwitchTo(CFG_PICTURE_MAIN_ID);
					WorkStatus.pic_id = CFG_PICTURE_MAIN_ID;
					Status_SendtoMonitor(OPT_STATUS_BAR_CLEAR_SET);
				}
				break;
				
			case MSG_SHOW_TIMES_SET:
				if(WorkStatus.SetEnum != SET_TIMES){
					WorkStatus.SetEnum = SET_TIMES;
					Pic_SwitchTo(CFG_PICTURE_TIMES_ID);
					WorkStatus.pic_id = CFG_PICTURE_TIMES_ID;
					Status_SendtoMonitor(OPT_STATUS_BAR_TIMES_SET);
				}else{
					WorkStatus.SetEnum = SET_IDLE;
					Pic_SwitchTo(CFG_PICTURE_MAIN_ID);
					WorkStatus.pic_id = CFG_PICTURE_MAIN_ID;
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



int main()
{
	MSG_BufferTypeDef msg;
	
	DevInit_PWMOut();
	DevInit_TickTimer();
	DevInit_OutputIO();
	DevInit_InputIO();
	USART_Init();
	Poweron_InitConsig();
	//delay_ms(5000);
	InitStatus_Show();
	//delay_ms(5000);
	InitStatus_Show();
	MSG_QueueInit();
	
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


				case PASSWORD_MANAGE_MODE:
				/**************************************************
					*in this case complete pass word manage, include:
					# 1.how to enter manage page from logo page;
					# 2. enter pass word be enter to system, compare, retry, and enter;
					# 3.set amortize, and store to eeprom
				*/

					switch(msg.pic){
						

						case CFG_PICTURE_LOGO_ID:
							if (TRUE == EnterSettingPage_Login(msg.c)){

								// if the pass word is true, Enter system login page
							}
							break;


						case CFG_PICTURE_PASSWORD_ID:
							break;


						case CFG_PICTURE_PUR_SETTING_ID:
							break;

					}
					
					break;


				case AMORTIZE_MANAGE_MODE:
				/*************************************************
					# enter password to system and start next work period;
					# 1. receive char, compare pass word and decide to start next period;
					# 2. change eeprom  flag about setting, and store to eeprom
				*/
					
					break;


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
	}

	return 0;
}












#if STORE
int main(void) 
{ 
//	MSG_BufferTypeDef msg;
	uint8_t buff[8] = {0,1,2,3,4,5,6,7};
	uint8_t buff2[8];
	uint8_t i;

	USART_Init();

	while(1)
	{
		delay_ms(5000);
		memset(buff2, 0 , 8);
		SendToMonitor(buff2,8);

		for(i = 0; i < 8; i++) buff[i] += 1;
		
		EepromWrite_PassWord(EEPROM_ADDRESS_SYSTEM_PASSWORD, buff);
		delay_ms(5000);
		
		EepromRead_PassWord(EEPROM_ADDRESS_SYSTEM_PASSWORD, buff2);
		SendToMonitor(buff2,8);

		EepromWrite_PassWord(EEPROM_ADDRESS_USER_PASSWORD, buff);
		delay_ms(5000);
		EepromRead_PassWord(EEPROM_ADDRESS_USER_PASSWORD, buff2);
		SendToMonitor(buff2,8);
		
		EepromRead_PassWord(EEPROM_ADDRESS_SYSTEM_PASSWORD, buff2);
		SendToMonitor(buff2,8);
	};

}
#endif

#if 0



	DevInit_PWMOut();
	DevInit_TickTimer();
	DevInit_OutputIO();
	DevInit_InputIO();
	USART_Init();
	Poweron_InitConsig();
	delay_ms(5000);
	InitStatus_Show();
	delay_ms(5000);
	InitStatus_Show();
	MSG_QueueInit();
	while(1)
	{

           
		if(!MSG_QueueIsEmpty())
		{
			MSG_QueueGet(&msg);
			switch(msg.c)
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
						Pic_SwitchTo(CFG_PICTURE_ENERGY_ID);
						WorkStatus.pic_id = CFG_PICTURE_ENERGY_ID;
						Status_SendtoMonitor(OPT_STATUS_BAR_ENERGY_SET);
					}else{
						WorkStatus.SetEnum = SET_IDLE;
						Pic_SwitchTo(CFG_PICTURE_MAIN_ID);
						WorkStatus.pic_id = CFG_PICTURE_MAIN_ID;
						Status_SendtoMonitor(OPT_STATUS_BAR_CLEAR_SET);
					}
					break;
					
				case MSG_SHOW_FREQUENCY_SET:
					if(WorkStatus.SetEnum != SET_FRQ){
						WorkStatus.SetEnum = SET_FRQ;
						Pic_SwitchTo(CFG_PICTURE_FREQUENCY_ID);
						WorkStatus.pic_id = CFG_PICTURE_FREQUENCY_ID;
						Status_SendtoMonitor(OPT_STATUS_BAR_FREQUENCY_SET);
					}else{
						WorkStatus.SetEnum = SET_IDLE;
						Pic_SwitchTo(CFG_PICTURE_MAIN_ID);
						WorkStatus.pic_id = CFG_PICTURE_MAIN_ID;
						Status_SendtoMonitor(OPT_STATUS_BAR_CLEAR_SET);
					}
					break;
					
				case MSG_SHOW_TIMES_SET:
					if(WorkStatus.SetEnum != SET_TIMES){
						WorkStatus.SetEnum = SET_TIMES;
						Pic_SwitchTo(CFG_PICTURE_TIMES_ID);
						WorkStatus.pic_id = CFG_PICTURE_TIMES_ID;
						Status_SendtoMonitor(OPT_STATUS_BAR_TIMES_SET);
					}else{
						WorkStatus.SetEnum = SET_IDLE;
						Pic_SwitchTo(CFG_PICTURE_MAIN_ID);
						WorkStatus.pic_id = CFG_PICTURE_MAIN_ID;
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
	} 

}
#endif	





