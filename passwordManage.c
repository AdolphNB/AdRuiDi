#include "avr/io.h"
#include "MSG_Queue.h"
#include "config.h"
#include "peripherals.h"
#include "user_hmi.h"
#include "string.h"
#include "passwordManage.h"



#define ENTER_DELETE	0X0A
#define ENTER_ENTER		0X0B





#define SYSTEM_PASSWORD 	0
#define PURCHASE_PASSWORD 	1

static uint8_t ReadPassWord(uint8_t pic, uint8_t * data)
{
	
	return FALSE;
}


static uint8_t ComparePassWord(uint8_t pic, PassWordManage_t *pw)
{
	uint8_t data[8];
	
	switch(pic){

		case SYSTEM_PASSWORD: 
			
			if(TRUE == ReadPassWord(SYSTEM_PASSWORD, data)){
				if(0 == strncmp((char *)data, (char *)pw->data, 8))
					return TRUE;
			}
			break;

			
		case PURCHASE_PASSWORD:
			
			if(TRUE == ReadPassWord(PURCHASE_PASSWORD, data)){
				if(0 == strncmp((char *)data, (char *)pw->data, 8))
					return TRUE;
			}
			break;

		
		default:break;

	}
	return FALSE;
}



StatusReturn_t PassWordPrase(uint8_t pic, PassWordManage_t *pw, uint8_t ch)
{

	switch(ch){

		case 0:case 1:case 2:case 3:
		case 4:case 5:case 6:case 7:
		case 8:case 9:
			
			if(pw->cnt < 8){
				
				pw->data[pw->cnt] = ch;  // store this char.
				pw->cnt++;
				
			}
			
			break;		


		case ENTER_DELETE:
			
			if(pw->cnt > 0 && pw->cnt <= 8){
				
				pw->cnt -= 1;
			
			}else pw->cnt = 0;
			
			return DELETE;
			
			break;


		case ENTER_ENTER:
			
			if(pw->cnt == PASSWORD_LEN){
				
				if (TRUE == ComparePassWord(pic, pw)){
					
					memset(pw, 0, sizeof(PassWordManage_t)); //clear password
					return RIGHT;	//enter completed, and this password is correctly.
				}
			}
			
			memset(pw, 0, sizeof(PassWordManage_t)); //ERROR: clear this data structure
			return WRONG;
			
			break;

	}
	

	return IGNORE;
}




typedef struct{

	uint8_t cnt;
	uint8_t data[4];
}Logo_EnterSystemLogin_t;

static Logo_EnterSystemLogin_t LogoLogin = {
	.cnt = 0,
	.data[0] = 0
};



uint8_t EnterSettingPage_Login(uint8_t ch)
{
	
	uint8_t ret =  FALSE;

	
	LogoLogin.data[LogoLogin.cnt] = ch;
	LogoLogin.cnt++;


	if (LogoLogin.cnt == 4){

		if (LogoLogin.data[0] == 0x01 && LogoLogin.data[1] == 0x02 && \
			LogoLogin.data[2] == 0x03 && LogoLogin.data[3] == 0x04){
			
			ret = TRUE;
		}
		
		memset(&LogoLogin, 0, sizeof(Logo_EnterSystemLogin_t));
	}


	return ret;
}


/*************************************************************
* if time out in logo page, you must clear the buffer of store keys;
*/
uint8_t EnterSettingPage_Login_Timeout(void)
{

	memset(&LogoLogin, 0, sizeof(Logo_EnterSystemLogin_t));
	
	return 0;
}





/***************************************************************************
*	Blow code is about read flag, and judge whether enter AMORTIZE MODE.
*	these codes include:
*	#	1.read the SWITCH about whether amortize-pay is OVER;
*	#	2.judge current data whether is the time-node  that enter password unlock next period;
***************************************************************************/

Date_ReadBack_t CurDate;
static uint8_t Judge_PasswordTimeNode()
{
	
	CurDate.flag = TRUE;

	//time1 = ReadDateFromScreen();
	//time2 = ReadEEpromData()

	
#if DEBUG

	/***********************************************************
		if the date is equal to UNLOCKDate    or    more than  UNLOCKDate,
		and the LOCK_FLAG is not be clear,    will return TRUE
	*/
	if (time2 <= tim1 && flag !=  0 ){

		return TRUE;

	}
#endif

	return FALSE;
}



uint8_t IS_Popup_AmortizePassWordPage(void)
{

	//uint8_t MasterSwitch = ;
	//uint8_t RemainTimes  = ;

	if (MasterSwitch ==  0 || RemainTimes == 0){

		//  the master switch is close, or the remain times is 0, that means USER don't need enter password
		return FALSE;
	}
		

	if (FALSE == Judge_PasswordTimeNode()){

		// it isn't time to unlock the next proid 
		return FALSE;
	}

	
	return TRUE;
	
}








