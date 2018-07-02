

#ifndef __USER_HMI_H__
#define __USER_HMI_H__

#include "string.h"
#include "config.h"

typedef enum{
	OPT_STATUS_BAR_ENERGY_SET,
	OPT_STATUS_BAR_FREQUENCY_SET,
	OPT_STATUS_BAR_TIMES_SET,
	OPT_STATUS_BAR_CLEAR_SET,
	OPT_STATUS_BAR_KVOPEN_SET,
	OPT_STATUS_BAR_KVCLOSE_SET,
	OPT_STATUS_BAR_ALERTING_SET,
	OPT_STATUS_BAR_ALERTCLEAR_SET,
	OPT_STATUS_BAR_ADD_WATER_SET,
	OPT_STATUS_BAR_DEC_WATER_SET,
	OPT_STATUS_BAR_WATER_INJECTION_SET,
	OPT_STATUS_BAR_DEWATERING_SET,
	OPT_STATUS_BAR_WATERING_CLEAR_SET,
	OPT_STATUS_BAR_TOGGLING_SET,
	OPT_STATUS_BAR_NO_TOGGLE_SET,
	OPT_STATUS_BAR_TEMP_TOGGLE_SET
}StatusBar_Show_t;

void InitStatus_Show(void);
void ParamSet_Add();
void ParamSet_Dec();
void Key_ClearCounter();
void Pic_SwitchTo(uint8_t pic);
void Status_SendtoMonitor(uint8_t show_sta);
void CounterValue_SendToMonitor();
void Status_SendtoMonitor(StatusBar_Show_t sta);
void Poweron_InitConsig(void);
void SendToMonitor(uint8_t *buf, uint8_t len);
void delay_ms(unsigned int ms);


#endif //__MSG_QUEUE_H__





