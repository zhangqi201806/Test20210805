#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_usart.h"

#include "main.h"
#include "adjust.h" 
#include "bitmap.h"
#include "flash.h"
#include "init.h"
#include "ui_main.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "ledandkey.h"
#include "sys.h"  
#include "sys485.h"   
#include "txandre.h"  
#include "powamp.h"  
#include "mtx.h" 
#include "config.h" 

void mtx_CfgNativeAddress_Command(void)
{
	txandre_SendPackage(MB_ADDR,CfgNativeAddress_Command,1,&CfgNativeAddress);
}

//配置系统时间
void mtx_CfgSystemTime_Command(void)
{
	u8 Buf[6];
	
	Buf[0] = year;//年
	Buf[1] = yue;//月
	Buf[2] = ri;//日
	Buf[3] = hour;//时
	Buf[4] = min;//分
	Buf[5] = sec;//秒
	
	txandre_SendPackage(MB_ADDR,CfgSystemTime_Command,6,Buf);
}

void mtx_Send_Get30AreaInf_Command(int nInfoArea)
{
	int nCmd;
	switch(nInfoArea)
	{
		case 1:nCmd = Get1To30AreaInf_Command;break;
		case 2:nCmd = Get31To60AreaInf_Command;break;
		case 3:nCmd = Get61To90AreaInf_Command;break;
		case 4:nCmd = Get91To120AreaInf_Command;break;
		case 5:nCmd = Get121To150AreaInf_Command;break;
		case 6:nCmd = Get151To180AreaInf_Command;break;
		case 7:nCmd = Get181To210AreaInf_Command;break;
		case 8:nCmd = Get211To240AreaInf_Command;break;
		case 9:nCmd = Get241To270AreaInf_Command;break;
	}
		
	txandre_SendPackage(MB_ADDR,nCmd,0,0);
}

void mtx_Send_CfgPa60Area_Command(int nAddr,int nCfgArea)
{
	u8 com,ArrryNum;		  
	
	com=Cfg1PowerAmp1to60Area_Command+(nAddr-1)*5+(nCfgArea-1);
	ArrryNum=(nAddr-1)*40+(nCfgArea-1)*8;
	
	txandre_SendPackage(MB_ADDR,com,8,&CfgPowerAmp[ArrryNum]);
}

void mtx_Send_StartupArea_Command(int nArea,int bStart)
{
	u8 nCmd,Buf[2];
	
	nCmd = (bStart?StartupArea_Command:StopArea_Command);
	
	Buf[0] = nArea>>8;
	Buf[1] = nArea&0xFF;
	
	txandre_SendPackage(MB_ADDR,nCmd,2,Buf);
}
