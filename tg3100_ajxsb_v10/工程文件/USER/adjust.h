#ifndef TIMEADJUST_H_
#define TIMEADJUST_H_

#include "stm32f10x.h"
//相关变量
//时间设置
extern uint8_t Time_Storage[14];
extern uint8_t Time_Adjust_Flag;
extern uint16_t year;
extern uint8_t yue,ri,hour,min,sec,year_H,year_L;

//功放配置

extern u8 CfgPowerAmpNum;
//相关函数
//时间设置
void LoadTimeStore(void);
void StoreTimeStore(void);
void DateTime_Display(void);
extern void Add_Time_Adjust_Function(void);
extern void Reduce_Time_Adjust_Function(void);	

//本机地址设置
void LoadNativeAddress(void);
int StoreNativeAddress(void);
void Add_CfgNativeAddress(void);
void Reduce_CfgNativeAddress(void);
void CfgNativeAddress_Display(void);

void Password_Display(void);
void Password_Inc_Display(void);
void Password_Dec_Display(void);

//功放配置
extern void Add_CfgPowerAmp_Function(void);
extern void Reduce_CfgPowerAmp_Function(void);
extern void CfgPowerAmp_Dispaly_Function(void);

#endif 
