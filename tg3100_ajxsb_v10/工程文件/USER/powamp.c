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
#include "mre.h" 
#include "ui_poweramp.h" 
#include "config.h" 
#include "pre.h" 

#define TIMER_ONLINE_COUNT   (0x10)

static u8 	RegedNum,OnlineNum;
static u16 	RegedFlag;
static u16	OnlineFlag;
static u8		OnlineTimer[PA_NUM];

static void OnTimer(void)
{
	int nIndex;
	
	if(!OnlineFlag) 
		return;
	
	for(nIndex=0;nIndex<PA_NUM;nIndex++)
	{
		if(OnlineTimer[nIndex])
		{
			OnlineTimer[nIndex]--;
			if(!OnlineTimer[nIndex])
			{
				OnlineFlag &= ~(1<<nIndex);
				OnlineNum--;
			}
		}
	}
}

static u8 Register_Flag;
static u8 Register_Finsh_Flag;
static u8 Register_TimeCount;

void PowerAmp_Online(int nAddr)
{
	u16 nMask = 1<<(nAddr-1);
	if(Register_Flag)
	{
		if(!(RegedFlag & nMask))
		{
			RegedFlag |= nMask;
			RegedNum++;		
			
			OnlineFlag |= nMask;
			OnlineNum++;
			OnlineTimer[nAddr-1] = TIMER_ONLINE_COUNT;
		}
	}	
	else
	{
		if(!(OnlineFlag & nMask))
		{
			OnlineFlag |= nMask;
			OnlineNum++;
		}
		OnlineTimer[nAddr-1] = TIMER_ONLINE_COUNT;
	}
}

int PowerAmp_IsReged(int nAddr)
{
	return ((RegedFlag & (1<<(nAddr-1)))?1:0);
}

int PowerAmp_GetRegedCount(void)
{
	return RegedNum;
}

static u8 nOfflinePa = PA_NUM;
void PowerAmp_CheckStateProc(void)
{
	int nPa;
	u16 nMask;
	
	if(Register_Flag)
		return;
	
	for(nPa=0;nPa<PA_NUM;nPa++)
	{
		if(!config_IsPowAmpCfged(nPa)) 
			continue;
		
		nMask = 1<<nPa;
		
		if(!(RegedFlag & nMask))
			continue;
		
		if(!(OnlineFlag & nMask))
			break;
	}
	
	if(nOfflinePa != nPa)
	{
		nOfflinePa = nPa;
		
		if(nPa == PA_NUM)
		{
			if(PowerAmp_Offline_Flag)
			{
				PowerAmp_Offline_Flag = 0;
				
				if(!PowerAmp_Fault_Num)
				{
					PowerAmp_BuzzerOn_Flag = 0;
					FAULTLED_OFF;//¹¦·Å¹ÊÕÏLED
					
					if(!AreaFault_BuzzerOn_Flag)
						BUZZER_OFF;
				}
			}
		}
		else
		{
			if(!PowerAmp_Offline_Flag)
			{
				PowerAmp_Offline_Flag = 1;
				
				FAULTLED_ON;
				PowerAmp_BuzzerOn_Flag = 1;
				if(!Record_Flag)
					BUZZER_ON;
			}
		}
	}
}

u8 PowerAmp_GetOfflinePa(void)
{
	return nOfflinePa;
}

void PowerAmp_SetStartRegFlag(void)
{
	int nIndex;
	
	Playback_Mode=0;
	Emergency_Mode=0;
	MP3_Mode=0;
	line1_Mode=0;
	line2_Mode=0;		
	
	Task_Flag=0;	
	
	Register_Flag=1;
	Register_Finsh_Flag=0;
	Register_TimeCount=0;
	SlaveAdress=0;
	
	RegedNum = 0;
	OnlineNum = 0;
	RegedFlag = 0;
	OnlineFlag = 0;
	for(nIndex=0;nIndex<PA_NUM;nIndex++)
		OnlineTimer[nIndex] = 0;
}

int PowerAmp_IsReging(void)
{
	return Register_Flag;
}

int PowerAmp_IsRegCompleted(void)
{
	return Register_Finsh_Flag;
}

void PowerAmp_OnRegCompleted(void)
{
	if(!Register_Finsh_Flag)
	{			
		Register_TimeCount++;	
		if(Register_TimeCount >= 3)
		{
			Register_TimeCount = 0;	
			Register_Flag = 0;
			Register_Finsh_Flag = 1;
			
			WndUpdate = 1;
		}	
	}
	else
			OnTimer();
}




