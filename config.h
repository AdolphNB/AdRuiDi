
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "avr/io.h"


#define SYSTEM_VERSION "Ver1.5.1"




#define DEBUG_TEST (1)




#define VER_CH 0
#define VER_EN 1


#define VERSION_DEFINE (VER_EN)

#define CFG_TIMER1_INTERRUPT_INTERVAL		(1100)   //62500 = 1s

#define CFG_ENERGY_PWM_OUTPUT_SWTICH		(0X01)



#define CFG_LOGO_PAGE_TIMEOUT_ENVET			(0xFF)

/*  the case of pass word manage mode */
#define CFG_PICTURE_LOGO_ID 				(0x00)
#define CFG_PICTURE_PASSWORD_ID				(0x0a)
#define CFG_PICTURE_PUR_SETTING_ID_1		(0x0b)
#define CFG_PICTURE_PUR_SETTING_ID_2		(0x0c)
#define CFG_PICTURE_PUR_SETTING_ID_3		(0x0d)

#define CFG_PICTURE_DATE_PRODUCTION			(0x0e)
#define CFG_PICTURE_DATE_AMORTIZE			(0x0F)






#define SYSTEM_PASSWORD 	CFG_PICTURE_PASSWORD_ID
#define PURCHASE_PASSWORD 	CFG_PICTURE_PUR_SETTING_ID_1


/*  the case of amortize mode  */
#define CFG_AMORTIZE_PW_ENTER_ID			(20)



/*  the case is normal work mode picture id */
/*Chinese version picture id*/
//#define CFG_PICTURE_MAIN_CHINESE_ID			(51)
//#define CFG_PICTURE_CHINESE_CONFIRM_ID		(52)
#define CFG_PICTURE_MAIN_CHINESE_ID			(50)
#define CFG_PICTURE_CHINESE_BACK_ID			(51)
#define CFG_PICTURE_CHINESE_ENERGY_ID		(52)
#define CFG_PICTURE_CHINESE_FREQ_ID			(53)
#define CFG_PICTURE_CHINESE_PRESET_ID		(54)

#define CFG_PICTURE_CHINESE_CONFIRM_ID		(55)



/*English version picture id*/
//#define CFG_PICTURE_MAIN_ENGLISH_ID			(53)
//#define CFG_PICTURE_ENGLISH_CONFIRM_ID			(54)
#define CFG_PICTURE_MAIN_ENGLISH_ID			(30)
#define CFG_PICTURE_ENGLISH_BACK_ID			(31)
#define CFG_PICTURE_ENGLISH_ENERGY_ID		(32)
#define CFG_PICTURE_ENGLISH_FREQ_ID			(33)
#define CFG_PICTURE_ENGLISH_PRESET_ID		(34)

#define CFG_PICTURE_ENGLISH_CONFIRM_ID		(35)




#define CFG_OPTION_ENERGY_ID			(0X00)
#define CFG_OPTION_FREQUENCY_ID			(0X01)
#define CFG_OPTION_TIMES_ID				(0X02)





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
	uint8_t EnChFlag;
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


typedef struct{
	uint8_t dewaterOpenFlag;
	uint8_t dewaterCloseFlag;
	uint8_t judgeflag;
}Water_Confirm_t;




#define EEPROM_ADDRESS_TOTAL_SWITCH				(0x0010)
#define EEPROM_ADDRESS_TOTAL_NUMBER				(0x0020)
#define EEPROM_CHINESE_ENGLISH_FLAG				(0x0030)


#define EEPROM_ADDRESS_DATE_1ST					(0x0100)
#define EEPROM_ADDRESS_DATE_2ST					(0x0110)
#define EEPROM_ADDRESS_DATE_3ST					(0x0120)
#define EEPROM_ADDRESS_DATE_4ST					(0x0130)
#define EEPROM_ADDRESS_DATE_5ST					(0x0140)
#define EEPROM_ADDRESS_DATE_6ST					(0x0150)

#define EEPROM_ADDRESS_DATE_PRODT				(0x01A0)

#define EEPROM_ADDRESS_TOGGLE_COUNTER			(0x01E0)

typedef struct {

	uint8_t flag;
	uint8_t year;
	uint8_t month;
	uint8_t day;
	
}RepayDate_t;



#define MAX_AMORTIZE_NUMBER		12
typedef struct {

	uint8_t step;
	RepayDate_t datebase[MAX_AMORTIZE_NUMBER];
	
}AmortizeDataBase_t;





#endif //__CONFIG_H__













