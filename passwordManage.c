#include "avr/io.h"
#include "MSG_Queue.h"
#include "config.h"
#include "peripherals.h"
#include "user_hmi.h"
#include "string.h"




#define ENTER_DELETE	0X0A
#define ENTER_ENTER		0X0B




/***************************************************************
	*define a structure to store chars, these chars will be store user password.
	* @the para cnt is a couter, store current sequence-number;
*/
#define PASSWORD_LEN	(8)
typedef struct{
	uint8_t cnt;
	uint8_t data[PASSWORD_LEN];
}PassWordManage_t;

//system password
PassWordManage_t sysPassword;

//user password
PassWordManage_t userPassword;



typedef enum{
	
	WRONG = 0,
	RIGHT = 1,
	IGNORE = 3
	
}StatusReturn_t;






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


		default:
			break;
	}
	

	return IGNORE;
}




#if DEBUG
void handle_function()
{
	StatusReturn_t val = PassWordPrase(pic, &sysPassword,  char);
	
	if (val == RIGHT){

		//ENTER NEXT PAGE
	}else if (val == WRONG){

		// hint error, and retry password
	}else {
		// can continue receive message and store it to buffer.
	}
	
	
}
#endif















