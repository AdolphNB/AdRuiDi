
#include "peripherals.h"
#include "avr/io.h"
#include <avr/interrupt.h>
#include "MSG_Queue.h"
#include "user_hmi.h"
#include "eepromManage.h"
#include "system_setting_page.h"


extern ShowParam_Def cure;
extern StructParam_Def WorkStatus;
extern unsigned char gToggleValue;
//const unsigned int g_energy_PWM_array[11]= {1023,900,800,700,600,500,400,300,200,100,20};
//												0      1    2     3     4     5     6     7     8     9    10
//const unsigned int g_energy_PWM_array[11]  = {1023,974,954,926,887,832,755,648,497,287,20};
//const unsigned int g_energy_PWM_array[11]  = {1023,900,865,823,771,705,623,519,388,225,10}; //exp()
const unsigned int g_energy_PWM_array[11]  = {1023,900,865,825,780,725,653,552,420,253,15}; //adj







void delay_ms(unsigned int ms)
{
	unsigned int i = 192;
	while(ms--)
	{
		for(i = 192; i > 0; i--);
	}
}


void delay_us(unsigned int us)
{
	unsigned int i;
	while(us--)
	{
		for(i = 1; i > 0; i--);
	}
}


void Option_SelectChange(uint8_t opt, uint8_t color)
{
	uint8_t tx_buff[8] = {0x5A, 0xA5, 0x05, 0x82, 0x10, 0x03, 0xF8, 0x00};

	switch(opt){
		case 0:
			if(!color){
				tx_buff[4] = 0x10; tx_buff[5] = 0x03; tx_buff[6] = 0x00; tx_buff[7] = 0x00;
			}else{
				tx_buff[4] = 0x10; tx_buff[5] = 0x03;  tx_buff[6] = 0x05; tx_buff[7] = 0x1d;
			}
			break;

		case 1:
			if(!color){
				tx_buff[4] = 0x11; tx_buff[5] = 0x03; tx_buff[6] = 0x00; tx_buff[7] = 0x00;
			}else{
				tx_buff[4] = 0x11; tx_buff[5] = 0x03;  tx_buff[6] = 0x05; tx_buff[7] = 0x1d;
			}
			break;


		case 2:
			if(!color){
				tx_buff[4] = 0x12; tx_buff[5] = 0x03; tx_buff[6] = 0x00; tx_buff[7] = 0x00;
			}else{
				tx_buff[4] = 0x12; tx_buff[5] = 0x03;  tx_buff[6] = 0x05; tx_buff[7] = 0x1d;
			}
			break;
			
	}

	SendToMonitor(tx_buff, 8);
}





void SendToMonitor(uint8_t *buf, uint8_t len)
{
	uint8_t i;
	
	for(i = 0; i < len; i++)
		USART_Transmit(*buf++);
}




void Status_SendtoMonitor(StatusBar_Show_t sta)
{
	uint8_t StatusBar_StatusBuf[25] = {0x5A,0xA5,0x0b,0x82,0x02, \
										  0x00,0xCD,0xA3,0xCD,0xA3, \
										  0xCD,0xA3,0xCD,0xA3};
	switch(sta)
	{
		case OPT_STATUS_BAR_ENERGY_SET:

			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]  = 0x02;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]  = 0xC4;StatusBar_StatusBuf[7]  = 0xDC;//
				StatusBar_StatusBuf[8]  = 0xC1;StatusBar_StatusBuf[9]  = 0xBF;//
				StatusBar_StatusBuf[10] = 0xC9;StatusBar_StatusBuf[11] = 0xE8;
				StatusBar_StatusBuf[12] = 0xD6;StatusBar_StatusBuf[13] = 0xC3;
				SendToMonitor(StatusBar_StatusBuf,14);
			}else{
				StatusBar_StatusBuf[2]  = sizeof("Engrgy Set") + 3;
				StatusBar_StatusBuf[4]  = 0x02;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"Engrgy Set", sizeof("Engrgy Set"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("Engrgy Set") + 6);
			}
			
			break;
			
		case OPT_STATUS_BAR_FREQUENCY_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]  = 0x02;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]  = 0xC6;StatusBar_StatusBuf[7]  = 0xB5;
				StatusBar_StatusBuf[8]  = 0xC2;StatusBar_StatusBuf[9]  = 0xCA;//
				StatusBar_StatusBuf[10] = 0xC9;StatusBar_StatusBuf[11] = 0xE8;
				StatusBar_StatusBuf[12] = 0xD6;StatusBar_StatusBuf[13] = 0xC3;
				SendToMonitor(StatusBar_StatusBuf,14);
			}else{
				StatusBar_StatusBuf[2]  = sizeof("Frequency Set") + 3;
				StatusBar_StatusBuf[4]  = 0x02;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"Frequency Set", sizeof("Frequency Set"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("Frequency Set") + 6);
			}
			break;
		case OPT_STATUS_BAR_TIMES_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]  = 0x02;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]  = 0xB4;StatusBar_StatusBuf[7]  = 0xCE;
				StatusBar_StatusBuf[8]  = 0xCA;StatusBar_StatusBuf[9]  = 0xFD;//
				StatusBar_StatusBuf[10] = 0xC9;StatusBar_StatusBuf[11] = 0xE8;
				StatusBar_StatusBuf[12] = 0xD6;StatusBar_StatusBuf[13] = 0xC3;
				SendToMonitor(StatusBar_StatusBuf,14);
			}else{
				StatusBar_StatusBuf[2] = sizeof("Preset")+3;
				StatusBar_StatusBuf[4]  = 0x02;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"Preset", sizeof("Preset"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("Preset")+6);
			}
			
			break;
		case OPT_STATUS_BAR_CLEAR_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]  = 0x02;	StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]  = 0x00;StatusBar_StatusBuf[7]  = 0x00;
				StatusBar_StatusBuf[8]  = 0x00;StatusBar_StatusBuf[9]  = 0x00;//
				StatusBar_StatusBuf[10] = 0x00;StatusBar_StatusBuf[11] = 0x00;
				StatusBar_StatusBuf[12] = 0x00;StatusBar_StatusBuf[13] = 0x00;
				SendToMonitor(StatusBar_StatusBuf,14);
			}else{
				StatusBar_StatusBuf[2]  = sizeof("Frequency Set") + 3;
				StatusBar_StatusBuf[4]  = 0x02;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				SendToMonitor(StatusBar_StatusBuf,sizeof("Frequency Set") + 6);
			}

			break;
		case OPT_STATUS_BAR_KVOPEN_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]  = 0x01;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]  = 0xB8;StatusBar_StatusBuf[7]  = 0xDF;
				StatusBar_StatusBuf[8]  = 0xD1;StatusBar_StatusBuf[9]  = 0xB9;//
				StatusBar_StatusBuf[10] = 0xBF;StatusBar_StatusBuf[11] = 0xAA;
				StatusBar_StatusBuf[12] = 0xC6;StatusBar_StatusBuf[13] = 0xF4;
				SendToMonitor(StatusBar_StatusBuf,14);
			}else{
				StatusBar_StatusBuf[2]  = sizeof("KV start") + 3;
				StatusBar_StatusBuf[4]  = 0x01;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"KV start", sizeof("KV start"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("KV start") + 6);
			}

			
			break;
		case OPT_STATUS_BAR_KVCLOSE_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]  = 0x01;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]  = 0xB8;StatusBar_StatusBuf[7]  = 0xDF;
				StatusBar_StatusBuf[8]  = 0xD1;StatusBar_StatusBuf[9]  = 0xB9;//
				StatusBar_StatusBuf[10] = 0xB9;StatusBar_StatusBuf[11] = 0xD8;
				StatusBar_StatusBuf[12] = 0xB1;StatusBar_StatusBuf[13] = 0xD5;
				SendToMonitor(StatusBar_StatusBuf,14);
			}else{
				StatusBar_StatusBuf[2]  = sizeof("KV stop") + 3;
				StatusBar_StatusBuf[4]  = 0x01;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"KV stop", sizeof("KV stop"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("KV stop")+6);
			}
			
			break;
		case OPT_STATUS_BAR_ALERTING_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]  = 0x03;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]  = 0xBE;StatusBar_StatusBuf[7]  = 0xAF;
				StatusBar_StatusBuf[8]  = 0xB8;StatusBar_StatusBuf[9]  = 0xE6;//
				SendToMonitor(StatusBar_StatusBuf,10);
			}else{
				StatusBar_StatusBuf[2]  = sizeof("Warn!") + 3;
				StatusBar_StatusBuf[4]  = 0x03;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"Warn!", sizeof("Warn!"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("Warn!")+6);
			}
			
			break;
		case OPT_STATUS_BAR_ALERTCLEAR_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]  = 0x03;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]  = 0x00;StatusBar_StatusBuf[7]  = 0x00;
				StatusBar_StatusBuf[8]  = 0x00;StatusBar_StatusBuf[9]  = 0x00;//
				SendToMonitor(StatusBar_StatusBuf,10);
			}else{
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]  = 0x03;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				SendToMonitor(StatusBar_StatusBuf,25);
			}
			
			break;
		case OPT_STATUS_BAR_ADD_WATER_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]  = 0x04;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]  = 0xD5;StatusBar_StatusBuf[7]  = 0xFD;
				StatusBar_StatusBuf[8]  = 0xD4;StatusBar_StatusBuf[9]  = 0xDA;//
				StatusBar_StatusBuf[10] = 0xC9;StatusBar_StatusBuf[11] = 0xCF;
				StatusBar_StatusBuf[12] = 0xCB;StatusBar_StatusBuf[13] = 0xAE;
				SendToMonitor(StatusBar_StatusBuf,14);
			}else{
				StatusBar_StatusBuf[2]  = sizeof("Water+") + 3;
				StatusBar_StatusBuf[4]	= 0x04;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"Water+", sizeof("Water+"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("Water+")+6);
				
			}
			
			break;
		case OPT_STATUS_BAR_DEC_WATER_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]	= 0x04;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]	= 0xD5;StatusBar_StatusBuf[7]  = 0xFD;
				StatusBar_StatusBuf[8]	= 0xD4;StatusBar_StatusBuf[9]  = 0xDA;//
				StatusBar_StatusBuf[10] = 0xCF;StatusBar_StatusBuf[11] = 0xC2;
				StatusBar_StatusBuf[12] = 0xCB;StatusBar_StatusBuf[13] = 0xAE;
				SendToMonitor(StatusBar_StatusBuf,14);
			
			}else{
				StatusBar_StatusBuf[2]  = sizeof("Water-") + 3;
				StatusBar_StatusBuf[4]	= 0x04;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"Water-", sizeof("Water-"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("Water-")+6);
				
			}
			
			break; 
		case OPT_STATUS_BAR_WATER_INJECTION_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]	= 0x04;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]	= 0xD5;StatusBar_StatusBuf[7]  = 0xFD;
				StatusBar_StatusBuf[8]	= 0xD4;StatusBar_StatusBuf[9]  = 0xDA;//
				StatusBar_StatusBuf[10] = 0xD7;StatusBar_StatusBuf[11] = 0xA2;
				StatusBar_StatusBuf[12] = 0xCB;StatusBar_StatusBuf[13] = 0xAE;
				SendToMonitor(StatusBar_StatusBuf,14);
			
			}else{
				StatusBar_StatusBuf[2]  = sizeof("InFlow") + 3;
				StatusBar_StatusBuf[4]	= 0x04;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"InFlow", sizeof("InFlow"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("InFlow")+6);
				
			}
			
			break;
		case OPT_STATUS_BAR_DEWATERING_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]	= 0x04;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]	= 0xD5;StatusBar_StatusBuf[7]  = 0xFD;
				StatusBar_StatusBuf[8]	= 0xD4;StatusBar_StatusBuf[9]  = 0xDA;//
				StatusBar_StatusBuf[10] = 0xC5;StatusBar_StatusBuf[11] = 0xC5;
				StatusBar_StatusBuf[12] = 0xCB;StatusBar_StatusBuf[13] = 0xAE;
				SendToMonitor(StatusBar_StatusBuf,14);
			
			}else{
				StatusBar_StatusBuf[2]  = sizeof("Drainage") + 3;
				StatusBar_StatusBuf[4]	= 0x04;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"Drainage", sizeof("Drainage"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("Drainage") + 6);
				
			}

			break;
		case OPT_STATUS_BAR_WATERING_CLEAR_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]	= 0x04;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]	= 0x00;StatusBar_StatusBuf[7]  = 0x00;
				StatusBar_StatusBuf[8]	= 0x00;StatusBar_StatusBuf[9]  = 0x00;//
				StatusBar_StatusBuf[10] = 0x00;StatusBar_StatusBuf[11] = 0x00;
				StatusBar_StatusBuf[12] = 0x00;StatusBar_StatusBuf[13] = 0x00;
				SendToMonitor(StatusBar_StatusBuf,14);
			
			}else{
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]	= 0x04;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				SendToMonitor(StatusBar_StatusBuf,25);
				
			}
			

			break;

		case OPT_STATUS_BAR_TOGGLING_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]	= 0x05;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]	= 0xD5;StatusBar_StatusBuf[7]  = 0xFD;
				StatusBar_StatusBuf[8]	= 0xD4;StatusBar_StatusBuf[9]  = 0xDA;//
				StatusBar_StatusBuf[10] = 0xB4;StatusBar_StatusBuf[11] = 0xA5;
				StatusBar_StatusBuf[12] = 0xB7;StatusBar_StatusBuf[13] = 0xA2;
				SendToMonitor(StatusBar_StatusBuf,14);
			
			}else{
				StatusBar_StatusBuf[2]  = sizeof("Trigger on") + 3;
				StatusBar_StatusBuf[4]	= 0x05;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"Trigger on", sizeof("Trigger on"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("Trigger on") + 6);
				
			}

			break;
		case OPT_STATUS_BAR_NO_TOGGLE_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]	= 0x05;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]	= 0xCD;StatusBar_StatusBuf[7]  = 0xA3;
				StatusBar_StatusBuf[8]	= 0xD6;StatusBar_StatusBuf[9]  = 0xB9;//
				StatusBar_StatusBuf[10] = 0xB4;StatusBar_StatusBuf[11] = 0xA5;
				StatusBar_StatusBuf[12] = 0xB7;StatusBar_StatusBuf[13] = 0xA2;
				SendToMonitor(StatusBar_StatusBuf,14);
			
			}else{
				StatusBar_StatusBuf[2]  = sizeof("Trigger off") + 3;
				StatusBar_StatusBuf[4]	= 0x05;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"Trigger off", sizeof("Trigger off"));
				SendToMonitor(StatusBar_StatusBuf,sizeof("Trigger off") + 6);
				
			}

			break;
		case OPT_STATUS_BAR_TEMP_TOGGLE_SET:
			if(WorkStatus.EnChFlag == TRUE){
				StatusBar_StatusBuf[2]  = 0x0b;
				StatusBar_StatusBuf[4]	= 0x05;StatusBar_StatusBuf[5]  = 0x00;
				StatusBar_StatusBuf[6]	= 0xD4;StatusBar_StatusBuf[7]  = 0xDD;
				StatusBar_StatusBuf[8]	= 0xCD;StatusBar_StatusBuf[9]  = 0xA3;//
				StatusBar_StatusBuf[10] = 0xB4;StatusBar_StatusBuf[11] = 0xA5;
				StatusBar_StatusBuf[12] = 0xB7;StatusBar_StatusBuf[13] = 0xA2;
				SendToMonitor(StatusBar_StatusBuf,14);
			
			}else{
				StatusBar_StatusBuf[2]  = sizeof("Trigger off") + 3;
				StatusBar_StatusBuf[4]	= 0x05;StatusBar_StatusBuf[5]  = 0x00;
				memset(&StatusBar_StatusBuf[6], 0, 19);
				memcpy(&StatusBar_StatusBuf[6],"Trigger off", sizeof("Trigger off"));
				SendToMonitor(StatusBar_StatusBuf, sizeof("Trigger off") + 6);
				
			}

			break;
	}

}




void TankDrainConfirm_SendToMonitor()
{
	uint8_t StatusBar_StatusBuf[25] = {0x5A,0xA5,0x0b,0x82,0x02, \
										  0x00,0xCD,0xA3,0xCD,0xA3, \
										  0xCD,0xA3,0xCD,0xA3};
	
	
	if(WorkStatus.EnChFlag == TRUE){
		StatusBar_StatusBuf[2]	= 0x0C;
		StatusBar_StatusBuf[4]	= 0x07;StatusBar_StatusBuf[5]  = 0x00;
		StatusBar_StatusBuf[6]	= 0xCB;StatusBar_StatusBuf[7]  = 0xAE;
		StatusBar_StatusBuf[8]	= 0xCF;StatusBar_StatusBuf[9]  = 0xE4;//
		StatusBar_StatusBuf[10] = 0xC5;StatusBar_StatusBuf[11] = 0xC5;
		StatusBar_StatusBuf[12] = 0xCB;StatusBar_StatusBuf[13] = 0xAE;
		StatusBar_StatusBuf[14] = 0x3F;
		SendToMonitor(StatusBar_StatusBuf,15);
	
	}else{
		StatusBar_StatusBuf[2]	= sizeof("Water tank drain?") + 3;
		StatusBar_StatusBuf[4]	= 0x06;StatusBar_StatusBuf[5]  = 0x00;
		memset(&StatusBar_StatusBuf[6], 0, 19);
		memcpy(&StatusBar_StatusBuf[6],"Water tank drain?", sizeof("Water tank drain?"));
		SendToMonitor(StatusBar_StatusBuf, sizeof("Water tank drain?") + 6);
		
	}	

}




void TankInflowConfirm_SendToMonitor()
{
	uint8_t StatusBar_StatusBuf[25] = {0x5A,0xA5,0x0b,0x82,0x02, \
										  0x00,0xCD,0xA3,0xCD,0xA3, \
										  0xCD,0xA3,0xCD,0xA3};
	
	
	if(WorkStatus.EnChFlag == TRUE){
		StatusBar_StatusBuf[2]	= 0x0C;
		StatusBar_StatusBuf[4]	= 0x07;StatusBar_StatusBuf[5]  = 0x00;
		StatusBar_StatusBuf[6]	= 0xCB;StatusBar_StatusBuf[7]  = 0xAE;
		StatusBar_StatusBuf[8]	= 0xCF;StatusBar_StatusBuf[9]  = 0xE4;//
		StatusBar_StatusBuf[10] = 0xD7;StatusBar_StatusBuf[11] = 0xA2;
		StatusBar_StatusBuf[12] = 0xCB;StatusBar_StatusBuf[13] = 0xAE;
		StatusBar_StatusBuf[14] = 0x3F;
		SendToMonitor(StatusBar_StatusBuf,15);
	
	}else{
		StatusBar_StatusBuf[2]	= sizeof("Water tank inflow?") + 3;
		StatusBar_StatusBuf[4]	= 0x06;StatusBar_StatusBuf[5]  = 0x00;
		memset(&StatusBar_StatusBuf[6], 0, 19);
		memcpy(&StatusBar_StatusBuf[6],"Water tank inflow?", sizeof("Water tank inflow?"));
		SendToMonitor(StatusBar_StatusBuf, sizeof("Water tank inflow?") + 6);
		
	}	

}





void Pic_SwitchTo(uint8_t pic)
{
	uint8_t tx_buff[7] = {0x5A,0xA5,0x04,0x80,0x03,0x00,0x00};

	tx_buff[6] = pic;
	SendToMonitor(tx_buff, 7);
}




static void Egy_SendToMonitor(uint8_t val)
{
	uint8_t tx_buff[8] = {0x5A,0xA5,0x05,0x82,0x00,0x10,0x00,0x00};
	tx_buff[7] = val;
	SendToMonitor(tx_buff, 8);
}




static void Frq_SendToMonitor(uint8_t val)
{
	uint8_t tx_buff[8] = {0x5A,0xA5,0x05,0x82,0x00,0x11,0x00,0x00};
	tx_buff[7] = val;
	SendToMonitor(tx_buff, 8);
}




static void Times_SendToMonitor(uint16_t val)
{
	uint8_t tx_buff[8] = {0x5A,0xA5,0x05,0x82,0x00,0x12,0x00,0x00};
	tx_buff[6] = (uint8_t)(val >> 8);
	tx_buff[7] = (uint8_t)(val & 0x00ff);
	SendToMonitor(tx_buff, 8);
}




static void Cnt_SendToMonitor(uint16_t val)
{
	uint8_t tx_buff[8] = {0x5A,0xA5,0x05,0x82,0x00,0x13,0x00,0x00};
	tx_buff[6] = (uint8_t)(val >> 8);
	tx_buff[7] = (uint8_t)(val & 0x00ff);
	SendToMonitor(tx_buff, 8);
}




void CounterValue_SendToMonitor()
{
	MSG_BufferTypeDef m;
	cure.cnt++;
	if(cure.cnt >= cure.times){
		m.c = MSG_CTRL_KV_CLOSE;
		MSG_QueuePut(&m);
	}
	Cnt_SendToMonitor(cure.cnt);
}




void Key_ClearCounter()
{
	cure.cnt = 0;
	Cnt_SendToMonitor(cure.cnt);
}




void Poweron_InitConsig(void)
{
	/*Set energy to lowest when power on*/
	ENERGY_PWM_CONFIG = g_energy_PWM_array[0];

}





/*****´¥ÃþÆÁ¡°¼Ó¡±{¿ØÖÆ****/
void ParamSet_Add()
{
	switch(WorkStatus.SetEnum)
	{
		case SET_ENERGY:
			if(cure.frq <= 20 && cure.egy < 10){
				cure.egy++;
			}else if(cure.frq > 20 && cure.egy < 5){
				cure.egy++;
			}
			 
			ENERGY_PWM_CONFIG = g_energy_PWM_array[cure.egy];
			Egy_SendToMonitor(cure.egy);
			break;
		case SET_FRQ:
			if(cure.egy > 5){
				if(cure.frq < 20)
					cure.frq++;
				else cure.frq = 20;
			}else{
				if(cure.frq < 30)
					cure.frq++;
				else cure.frq = 30;
			}
			
			gToggleValue = (1000 / (cure.frq));
			Frq_SendToMonitor(cure.frq);
			break;
		case SET_TIMES:
			if(cure.times < 5000)
				cure.times += 100;
			else cure.times = 5000;
			Times_SendToMonitor(cure.times);
			break;
		case SET_IDLE:
			break;
	}
}






/*****´¥ÃþÆÁ¡°¼õ¡±{¿ØÖÆ****/
void ParamSet_Dec()
{
	switch(WorkStatus.SetEnum)
	{
		case SET_ENERGY:
			if(cure.egy > 0){
				cure.egy--;
			}else{ 
				cure.egy = 0;
			}
			ENERGY_PWM_CONFIG = g_energy_PWM_array[cure.egy];
			Egy_SendToMonitor(cure.egy);
			break;
		case SET_FRQ:
			if(cure.frq > 10)
				cure.frq--;
			else cure.frq = 10;
			
			gToggleValue = (1000 / (cure.frq));
			Frq_SendToMonitor(cure.frq);
			break;
		case SET_TIMES:
			if(cure.times > 100)
				cure.times -= 100;
			else cure.times = 100;
			Times_SendToMonitor(cure.times);
			break;
		case SET_IDLE:
			break;
	}
}






void InitStatus_Show()
{
	gToggleValue = (1000 / (cure.frq));
	Frq_SendToMonitor(cure.frq);
	Egy_SendToMonitor(cure.egy);
	Times_SendToMonitor(cure.times);
	Cnt_SendToMonitor(0);
	Status_SendtoMonitor(OPT_STATUS_BAR_KVCLOSE_SET);
	Status_SendtoMonitor(OPT_STATUS_BAR_NO_TOGGLE_SET);

	TOUCH_ADD_WATER_STOP();
	TOUCH_DEC_WATER_STOP();
	TOUCH_DEWATER_STOP();
	TOUCH_WATERFLOODING_STOP();
}









void ChEn_PictureSwitchToMain(uint8_t ChEn)
{
	if(ChEn == TRUE){
		WorkStatus.pic_id = CFG_PICTURE_MAIN_CHINESE_ID;
		Pic_SwitchTo(CFG_PICTURE_MAIN_CHINESE_ID);
	}else{
		WorkStatus.pic_id = CFG_PICTURE_MAIN_ENGLISH_ID;
		Pic_SwitchTo(CFG_PICTURE_MAIN_ENGLISH_ID);
	}
}




void ChEn_PictureSwitch(uint8_t pic, uint8_t ChEn)
{

	switch(pic)
	{
		case MSG_SHOW_ENERGY_SET:
			if(ChEn == TRUE){
				WorkStatus.pic_id = CFG_PICTURE_CHINESE_ENERGY_ID;
				Pic_SwitchTo(CFG_PICTURE_CHINESE_ENERGY_ID);
			}else{
				WorkStatus.pic_id = CFG_PICTURE_ENGLISH_ENERGY_ID;
				Pic_SwitchTo(CFG_PICTURE_ENGLISH_ENERGY_ID);
			}
			break;


		case MSG_SHOW_FREQUENCY_SET:
			if(ChEn == TRUE){
				WorkStatus.pic_id = CFG_PICTURE_CHINESE_FREQ_ID;
				Pic_SwitchTo(CFG_PICTURE_CHINESE_FREQ_ID);
			}else{
				WorkStatus.pic_id = CFG_PICTURE_ENGLISH_FREQ_ID;
				Pic_SwitchTo(CFG_PICTURE_ENGLISH_FREQ_ID);
			}
			break;

		
		case MSG_SHOW_TIMES_SET:
			if(ChEn == TRUE){
				WorkStatus.pic_id = CFG_PICTURE_CHINESE_PRESET_ID;
				Pic_SwitchTo(CFG_PICTURE_CHINESE_PRESET_ID);
			}else{
				WorkStatus.pic_id = CFG_PICTURE_ENGLISH_PRESET_ID;
				Pic_SwitchTo(CFG_PICTURE_ENGLISH_PRESET_ID);
			}
			break;
	}
	
}




void updateStatistics(void)
{
	
	MSG_BufferTypeDef q;

	q.pic = WorkStatus.pic_id;
	q.c = MSG_SHOW_UPDATE_STATISTICS;
	MSG_QueuePut(&q);
}



static uint32_t energySumCounter = 0;
void Operate_EnergySumConter(uint8_t opt)
{
	if(opt){
		energySumCounter += energyLevel_Value[cure.egy]; //add a energy of setting
	}else{
		energySumCounter = 0;
	}

}





const unsigned int energyLevel_Value[11]  = {0,1,2,3,4,5,6,7,8,9,10}; //adj
void sendEnergyCounter_Show(void)
{
	uint8_t StatusBar_StatusBuf[25] = {0x5A,0xA5,0x0d,0x82,0x02, 0x00,0xCD,0xA3,0xCD,0xA3, 0xCD,0xA3,0xCD,0xA3};
	
	StatusBar_StatusBuf[4]	= 0x03;StatusBar_StatusBuf[5]  = 0x00;
	memset(&StatusBar_StatusBuf[6], 0, 19);
	sprintf((char*)&StatusBar_StatusBuf[6], "%d.%dkv", (int)energySumCounter/100, (int)energySumCounter%100);
	SendToMonitor(StatusBar_StatusBuf,25);

}





static uint32_t timeLiftCounter = 0;
void Operate_TimeLiftConter()
{
	/*timelift = (total times - current counter)  /  current frequency*/
	timeLiftCounter = (cure.times - cure.cnt) / cure.egy; 

}




void sendCountDown_Show(void)
{
	
	uint8_t StatusBar_StatusBuf[25] = {0x5A,0xA5,0x0d,0x82,0x02, 0x00,0xCD,0xA3,0xCD,0xA3, 0xCD,0xA3,0xCD,0xA3};

	Operate_TimeLiftConter(1);
	StatusBar_StatusBuf[4]	= 0x03;StatusBar_StatusBuf[5]  = 0x00;
	memset(&StatusBar_StatusBuf[6], 0, 19);
	sprintf((char*)&StatusBar_StatusBuf[6], "%d:%d", (int)timeLiftCounter/60, (int)timeLiftCounter%100);
	SendToMonitor(StatusBar_StatusBuf,25);
}


















