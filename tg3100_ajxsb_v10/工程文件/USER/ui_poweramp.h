#ifndef __UI_POWERAMP_H__
#define __UI_POWERAMP_H__

#include "stm32f10x.h"

extern const struct StructHead_CtrlType Struct_CtrlPoweiAmpRegisWnd[];
extern u8 CfgPowerAmpWnd_Flag;

//�������ó�ʼ�� ��LED
void Clear_PowAmpCfg_Var(void);
void Cfg_PowAmp_AreaLed(void);
void ShowAreaLed(void);

void Show_PowerAmp_Offline(int bReDraw);
void ui_Register_Finshed(void);
void ui_Registering(void);
#endif
