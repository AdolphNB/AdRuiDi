
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


uint8_t DevInit_TickTimer(void);
uint8_t DevInit_PWMOut(void);
uint8_t DevInit_OutputIO(void);
uint8_t DevInit_InputIO(void);
uint8_t USART_Init(void);
void USART_Transmit( unsigned char data);


#endif //__PERIPHERALS_H__
