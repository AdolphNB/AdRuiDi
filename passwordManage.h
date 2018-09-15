#ifndef __PASS_WORD_MANAGE_H__
#define __PASS_WORD_MANAGE_H__



/***************************************************************
	*define a structure to store chars, these chars will be store user password.
	* @the para cnt is a couter, store current sequence-number;
*/
#define PASSWORD_LEN	(8)
typedef struct{
	uint8_t cnt;
	uint8_t data[PASSWORD_LEN];
}PassWordManage_t;


typedef struct {
	uint8_t cnt;
	uint8_t data[10];
}DatePrdt_t;




typedef struct {
	uint8_t flg;
	uint8_t year;
	uint8_t mon;
	uint8_t day;
}Amortize_Date_t;







typedef enum{
	
	WRONG = 0,
	RIGHT = 1,
	IGNORE = 3,
	DELETE = 4
	
}StatusReturn_t;

uint8_t EnterSettingPage_Login_TimeoutClear(void);
uint8_t EnterSettingPage_Login(uint8_t ch);
StatusReturn_t PassWordPrase(uint8_t pic, PassWordManage_t *pw, uint8_t ch);
uint8_t IS_Popup_AmortizePassWordPage(void);
void AlreadyPaid_ClearCurrentStore(void);
void DisplayRandomCodeToScreen();
void DisplayPasswordCharToScreen(uint8_t pic, uint8_t num);
void DisplayPaying_AmortizeNumber(void);


#endif

