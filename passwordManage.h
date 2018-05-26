#ifndef __PASS_WORD_MANAGE_H__
#define __PASS_WORD_MANAGE_H__



uint8_t EnterSettingPage_Login_Timeout(void);

uint8_t EnterSettingPage_Login(uint8_t ch);

StatusReturn_t PassWordPrase(uint8_t pic, PassWordManage_t *pw, uint8_t ch);



#endif

