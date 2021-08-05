#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stm32f10x.h"

extern uint8_t Edit_Cfg_PowAmp_Area_Buffer[40];
extern uint8_t Edit_Password_Buffer[4];
extern uint8_t Main_Adress_ReadFlash_Buffer[];

//读写 主板地址  信息
void Read_Main_Adress(void);
void Write_Main_Adress(void);

void Read_Password(void);
int Password_IsUpdated(int nType);
void Write_Password(int nType);
void Load_PasswordBuf(int nType);
int Check_Password(int nType);

void Set_SysPassword(int nPos);
int Check_SysPassword(void);

//读功放配置信息
void Init_AllPowAmpCfg(void);
void Edit_PowAmpCfg(int nAddr);
void config_ToggleAreaFlag(int nArea);
void Write_PowAmpCfg(int nAddr);
int config_IsPowAmpCfged(int nPa);
int config_GetPowAmpCfgedNum(void);

int config_IsAreaCfged(int nPa,int nArea);
int config_IsAreaCfgedAny(int nArea);

void config_ReadAllConfig(void);
void config_RemoveAllCfg(void);
#endif
