#include "stm32f10x_gpio.h"
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
#include "mre.h" 
#include "pre.h"
#include "ui_key.h"
#include "config.h"
#include "ui_poweramp.h"
#include "timer.h"

static uint8_t Play_Press_Flag;

void Play(void)
{
	if(!Play_Press_Flag)
		Play_Press_Flag=1;		
	else
		Play_Press_Flag=0;
		
	Task_Flag=1;
	TX_Task_Command_Flag[13]=1;//给主板 播放命令
}

void Stop_Play(void)
{
	Play_Press_Flag=0;

	Task_Flag=1;
	TX_Task_Command_Flag[14]=1;	//给主板 停止播放命令
}

void Fast_Forward(void)
{
	Task_Flag=1;
	TX_Task_Command_Flag[15]=1;
}
void Fast_Back(void)
{
	Task_Flag=1;
	TX_Task_Command_Flag[16]=1;
}
void Last_Song(void)
{
	Task_Flag=1;
	
	TX_Task_Command_Flag[17]=1;	
	TX_Task_Command_Flag[26]=1;//收到 第几首歌曲后 发获取歌曲名字命令
}
void Next_Song(void)
{
	Task_Flag=1;
	TX_Task_Command_Flag[19]=1;	
	TX_Task_Command_Flag[26]=1;//收到 第几首歌曲后 发获取歌曲名字命令
}

void Main_PlayFlag(int bSingle)
{
	Task_Flag=1;
	TX_Task_Command_Flag[20+(bSingle?0:1)]=1;
}

static int nReSendCount,nReSendTask;

void PowAmp_PeriodTaskProc(void)
{
	if(!nReSendTask)
		return;
	
	if(nReSendCount)
	{
		nReSendCount--;
		
		Task_Flag=1;
		TX_Task_Command_Flag[nReSendTask]=1;
	}
}

static void PowAmp_SetPeriodTask(int nTask)
{
	nReSendCount = 6;
	nReSendTask = nTask;
}

void PowAmp_All_Start(void)
{
	int nIndex;
	
	if(!config_GetPowAmpCfgedNum())
		return;
	
	if(PowerAmp_Start_Num == config_GetPowAmpCfgedNum())
		return;
	
	PowerAmp_Start_Num = config_GetPowAmpCfgedNum();
	PowerAmp_Mic_Num = 0;
	PowerAmp_Play_Num = 0;
	
	for(nIndex=0;nIndex<10;nIndex++)
	{
		if(!config_IsPowAmpCfged(nIndex))
			continue;
		
		PowerAmp_Start_Flag[nIndex] = 1;
		PowerAmp_Mic_Flag[nIndex] = 0;
		PowerAmp_Play_Flag[nIndex] = 0;
	}

	Task_Flag=1;
	TX_Task_Command_Flag[89]=1; //功放全局启动命令
	PowAmp_SetPeriodTask(89);
}

void PowAmp_All_Mic(void)
{
	int nIndex;
	
	if(!config_GetPowAmpCfgedNum())
		return;
	
	if(PowerAmp_Mic_Num == config_GetPowAmpCfgedNum())
		return;
	
	PowerAmp_Mic_Num = config_GetPowAmpCfgedNum();
	PowerAmp_Start_Num = 0;
	PowerAmp_Play_Num = 0;
	
	for(nIndex=0;nIndex<10;nIndex++)
	{
		if(!config_IsPowAmpCfged(nIndex))
			continue;
		
		PowerAmp_Mic_Flag[nIndex] = 1;
		PowerAmp_Start_Flag[nIndex] = 0;
		PowerAmp_Play_Flag[nIndex] = 0;
	}

	Task_Flag=1;
	TX_Task_Command_Flag[90]=1; //功放全局MicOn命令
	PowAmp_SetPeriodTask(90);
}


void PowAmp_All_PlayOrStop(void)
{
	int nArea,nIndex;
	
	if(!config_GetPowAmpCfgedNum())
		return;
	
	PowAmp_SetPeriodTask(0);
	
	PowerAmp_Start_Num = 0;
	PowerAmp_Mic_Num = 0;
	PowerAmp_Play_Num = 0;
	
	for(nIndex=0;nIndex<10;nIndex++)
	{
		PowerAmp_Start_Flag[nIndex] = 0;
		PowerAmp_Mic_Flag[nIndex] = 0;
		PowerAmp_Play_Flag[nIndex]=0;
	}

	for(nArea=0;nArea<270;nArea++)
	{
		if(!Area_Startup_Flag[nArea])
			continue;
		
		for(nIndex=0;nIndex<10;nIndex++)
		{
			if(PowerAmp_Play_Flag[nIndex]) 
				continue;
			
			if(config_IsAreaCfged(nIndex,nArea))
			{
				PowerAmp_Play_Flag[nIndex] = 1;
				PowerAmp_Play_Num++;
				if(PowerAmp_Play_Num == config_GetPowAmpCfgedNum())
					goto lbExit;
			}
		}
	}
	
	lbExit:
	
	Task_Flag=1;
	for(nIndex=0;nIndex<10;nIndex++)
	{
		if(!config_IsPowAmpCfged(nIndex))
			continue;
		
		if(PowerAmp_Play_Flag[nIndex])
		{
			TX_Task_Command_Flag[45+nIndex]=1; //功放播放命令
		}
		else
			TX_Task_Command_Flag[55+nIndex]=1; //功放停动命令
	}
}

void PowAmp_All_Stop(void)
{
	int nIndex;
	
	if(!config_GetPowAmpCfgedNum())
		return;
	
	if(!PowerAmp_Start_Num && !PowerAmp_Mic_Num && !PowerAmp_Play_Num)
		return;
	
	PowerAmp_Start_Num = 0;
	PowerAmp_Mic_Num = 0;
	PowerAmp_Play_Num = 0;

	for(nIndex=0;nIndex<10;nIndex++)
	{
		if(!config_IsPowAmpCfged(nIndex))
			continue;
		
			PowerAmp_Start_Flag[nIndex] = 0;
			PowerAmp_Mic_Flag[nIndex] = 0;
			PowerAmp_Play_Flag[nIndex] = 0;
	}
	
	Task_Flag=1;
	TX_Task_Command_Flag[91]=1; //功放全局停动命令
	PowAmp_SetPeriodTask(91);
}

void PowAmp_Play(int nArea)
{
	int nIndex;
	
	for(nIndex=0;nIndex<10;nIndex++)
	{
		if(config_IsAreaCfged(nIndex,nArea))				
		{
			if(!PowerAmp_Fault_Flag[nIndex] && !PowerAmp_Play_Flag[nIndex])
			{
				PowerAmp_Play_Flag[nIndex] = 1;
				PowerAmp_Play_Num++;
				
				if(PowerAmp_Start_Flag[nIndex])
				{
					PowerAmp_Start_Flag[nIndex] = 0;
					PowerAmp_Start_Num--;
				}
				
				if(PowerAmp_Mic_Flag[nIndex])
				{
					PowerAmp_Mic_Flag[nIndex] = 0;
					PowerAmp_Mic_Num--;
				}
				
				PowAmp_SetPeriodTask(0);
				
				Task_Flag=1;
				TX_Task_Command_Flag[45+nIndex]=1; //功放播放命令
			}
		}
	}
}

static int OtherAreaIsStoped(int nPaIndex,int nArea)
{
	int nIndex;
	
	for(nIndex=0;nIndex<270;nIndex++)
	{
		if(nIndex == nArea)
			continue;
		
		if(config_IsAreaCfged(nPaIndex,nIndex))
		{
			if(Area_Startup_Flag[nIndex])
					return 0;
		}
	}
	return 1;
}

void PowAmp_Stop(int nArea) 
{
	int nIndex;
	
	for(nIndex=0;nIndex<10;nIndex++)
	{
		if(config_IsAreaCfged(nIndex,nArea))				
		{
			if(!PowerAmp_Fault_Flag[nIndex] && (PowerAmp_Start_Flag[nIndex] || PowerAmp_Play_Flag[nIndex] || PowerAmp_Mic_Flag[nIndex]))
			{
				if(OtherAreaIsStoped(nIndex,nArea))
				{
					if(PowerAmp_Start_Flag[nIndex])
					{
						PowerAmp_Start_Flag[nIndex] = 0;
						PowerAmp_Start_Num--;
					}
					
					if(PowerAmp_Play_Flag[nIndex])
					{
						PowerAmp_Play_Flag[nIndex] = 0;
						PowerAmp_Play_Num--;
					}
					
					if(PowerAmp_Mic_Flag[nIndex])
					{
						PowerAmp_Mic_Flag[nIndex] = 0;
						PowerAmp_Mic_Num--;
					}
					
					PowAmp_SetPeriodTask(0);
					
					Task_Flag=1;
					TX_Task_Command_Flag[55+nIndex]=1; //功放停动命令
				}
			}
		}
	}
}
