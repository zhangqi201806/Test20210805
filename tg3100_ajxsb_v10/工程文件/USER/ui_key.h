#ifndef __UIKEY_H__
#define __UIKEY_H__

#include "stm32f10x.h"

extern u8 Mute_Flag,bBackGround_Flag,bCheckSelf_Flag;

void ui_KeyProc(void);
void SelfCheck(int bGlobal);
void Fun_KEY_Emer_Function(void);
#endif
