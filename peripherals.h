
#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include "avr/io.h"
#include "config.h"

#define TIMER_TICK_OPEN()
#define TIMER_TICK_CLOSE()

#define OUT_PWM_OUT_OPEN()
#define OUT_PWM_OUT_CLOSE()

#define OUT_HIGH_VOLTAGE_OPEN()
#define OUT_HIGH_VOLTAGE_CLOSE()

#define OUT_TOGGLE_SWITCH_OPEN()
#define OUT_TOGGLE_SWITCH_CLOSE()

#define OUT_ADD_WATER_OPEN()
#define OUT_ADD_WATER_CLOSE()

#define OUT_DEC_WATER_OPEN()
#define OUT_DEC_WATER_CLOSE()

#define OUT_ALERT_SWITCH_OPEN()
#define OUT_ALERT_SWITCH_CLOSE()

#define IN_GET_EGG_SIG()
#define IN_GET_MANUAL_TOGGLE_SIG()
#define IN_GET_TEMPERATURE_SIG()




typedef struct{
	uint8_t flag;
	uint8_t pic;
	uint16_t period;
	uint32_t timeStamp;
}TimeoutTask_t;



uint8_t DevInit_TickTimer(void);
uint8_t DevInit_PWMOut(void);
uint8_t DevInit_OutputIO(void);
uint8_t DevInit_InputIO(void);
uint8_t USART_Init(void);
void USART_Transmit( unsigned char data);
uint32_t Get_SystemTick();

void StartTimeout_Task(uint8_t pic, uint16_t dly);
void DestroyTimeout_Task();
void TimeoutTask_PutToQueue();
void ReadCurrentDate();



#if DEBUG1_TEST
void uart1_init(void);
void puts1(char *s, uint8_t c);
#endif

#endif //__PERIPHERALS_H__
