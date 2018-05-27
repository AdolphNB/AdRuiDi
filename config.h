
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "avr/io.h"


#define VER_CH 0
#define VER_EN 1


#define VERSION_DEFINE (VER_EN)

#define CFG_TIMER1_INTERRUPT_INTERVAL		(1100)   //62500 = 1s

#define CFG_ENERGY_PWM_OUTPUT_SWTICH		(0X01)



#define CFG_LOGO_PAGE_TIMEOUT_ENVET			(0xFF)

/*  the case of pass word manage mode */
#define CFG_PICTURE_LOGO_ID 				(0x00)
#define CFG_PICTURE_PASSWORD_ID				(0x22)
#define CFG_PICTURE_PUR_SETTING_ID			(0x23)


/*  the case of amortize mode  */
#define CFG_AMORTIZE_PW_ENTER_ID			(0x00)



/*  the case is normal work mode picture id */
#define CFG_PICTURE_MAIN_ID					(0X00)
#define CFG_PICTURE_ENERGY_ID				(0X02)
#define CFG_PICTURE_FREQUENCY_ID			(0X03)
#define CFG_PICTURE_TIMES_ID				(0X04)







#define OPEN_KV_ENERGY()					do{PORTF &= ~(1 << PF0);DDRF |= (1 << PF0);}while(0)
#define CLOSE_KV_ENERGY()					do{PORTF |=  (1 << PF0);DDRF |= (1 << PF0);}while(0)

#define TOGGLE_ENERGY_START()				do{PORTE &= ~(1 << PE4);DDRE |= (1 << PE4);}while(0)
#define TOGGLE_ENERGY_STOP()				do{PORTE |=  (1 << PE4);DDRE |= (1 << PE4);}while(0)


#define TOUCH_ADD_WATER_OPEN()				do{PORTF &= ~(1 << PF1);DDRF |= (1 << PF1);}while(0)
#define TOUCH_ADD_WATER_STOP()				do{PORTF |=  (1 << PF1);DDRF |= (1 << PF1);}while(0)

#define TOUCH_DEC_WATER_OPEN()				do{PORTE &= ~(1 << PE2);DDRE |= (1 << PE2);}while(0)
#define TOUCH_DEC_WATER_STOP()				do{PORTE |=  (1 << PE2);DDRE |= (1 << PE2);}while(0)

#define TOUCH_WATERFLOODING_OPEN()			do{PORTE &= ~(1 << PE5);DDRE |= (1 << PE5);}while(0)
#define TOUCH_WATERFLOODING_STOP()			do{PORTE |=  (1 << PE5);DDRE |= (1 << PE5);}while(0)

#define TOUCH_DEWATER_OPEN()				do{PORTF &= ~(1 << PF3);DDRF |= (1 << PF3);}while(0)
#define TOUCH_DEWATER_STOP()				do{PORTF |=  (1 << PF3);DDRF |= (1 << PF3);}while(0)

#define OVER_TEMPERATURE_ALERT_OPEN()		do{PORTE &= ~(1 << PE5);DDRE |= (1 << PE5);}while(0)
#define OVER_TEMPERATURE_ALERT_STOP()		do{PORTE |=  (1 << PE5);DDRE |= (1 << PE5);}while(0)


#define ENERGY_PWM_CONFIG  					OCR3A




#define GET_PD_INPUT_VALUE()				PIND;
#define ECG_INPUT_IO						PD4
#define TEMPERATURE_INPUT_IO				PD6
#define MANUAL_INPUT_IO						PD5



#define TRUE (1)
#define FALSE (0)




#define EEPROM_ADDRESS_SYSTEM_PASSWORD  0X00000000
#define EEPROM_ADDRESS_USER_PASSWORD    0X00000010
#define EEPROM_ADDRESS_FLAG_STATUS1     0X00000020
#define EEPROM_ADDRESS_FLAG_STATUS2     0X00000030






typedef enum{
	OPEN,
	CLOSE = !OPEN
}KVFlag_Def;


typedef struct{
	uint16_t frq;
	uint16_t egy;
	uint16_t times;
	uint16_t cnt;
}ShowParam_Def;

typedef enum{
	SET_IDLE,
	SET_ENERGY,
	SET_FRQ,
	SET_TIMES
}ParamSet_flag_t;

typedef enum{
	TROGGLE_IDLE = 0,
	TROGGLE_TING = 1,
}TroggleStatus_t;

typedef struct{
	uint8_t pic_id;
	ParamSet_flag_t SetEnum;
	KVFlag_Def kv_flag;
	TroggleStatus_t trg;
	ShowParam_Def *p;
}StructParam_Def;

typedef struct{
	uint8_t temp;
	uint8_t ecg;
	uint8_t man;
}StructInput_flag_t;

typedef struct{
	uint16_t q_put_spd;
	
}Misc_Pram_t;




typedef struct{

	uint8_t flag;
	uint8_t data[31];
	
}Date_ReadBack_t;

extern Date_ReadBack_t CurDate;


#endif //__CONFIG_H__













