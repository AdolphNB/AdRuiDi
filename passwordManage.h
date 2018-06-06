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




#endif

