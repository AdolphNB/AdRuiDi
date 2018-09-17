#ifndef __SYSTEM_SETTING_PAGE_H__
#define __SYSTEM_SETTING_PAGE_H__

#include "string.h"
#include "config.h"

void Run_SystemSetttingPage(uint8_t pic, uint8_t ch);
void Display_Date_Of_Production(uint8_t flg);
void Display_ChineseEnglish();
void update_systemSetting3Data(void);
void update_EEprom_ToggleCounterValue(void);
void Start_OneTime_ToggleCounterStore(void);







#endif
