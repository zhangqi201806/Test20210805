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
#include "mre.h" 
#include "ptx.h" 
#include "pre.h"
#include "ui_key.h" 
#include "ui_poweramp.h" 
#include "config.h" 

u8 PowerAmp_BuzzerOn_Flag;

u8 PowerAmp_Offline_Flag;

u8 PowerAmp_Fault_Flag[10]={0};
u8 PowerAmp_Fault_Num;

u8 PowerAmp_Start_Flag[10]={0};
u8 PowerAmp_Start_Num;

u8 PowerAmp_Play_Flag[10]={0};
u8 PowerAmp_Play_Num;

u8 PowerAmp_Mic_Flag[10]={0};
u8 PowerAmp_Mic_Num;

//巡检中功放的正常回复
static void Normal(int nAddr)
{
	if(PowerAmp_Fault_Flag[nAddr])
	{
		PowerAmp_Fault_Flag[nAddr] = 0;
		
		PowerAmp_Fault_Num--;
		
		if(!PowerAmp_Fault_Num && !PowerAmp_Offline_Flag)
		{
			PowerAmp_BuzzerOn_Flag = 0;
			FAULTLED_OFF;//功放故障LED
				
			if(!AreaFault_BuzzerOn_Flag)
				BUZZER_OFF;
			
		}
	}				
}

//巡检中功放的故障回复
static void Fault(int nAddr)
{
	if(!PowerAmp_Fault_Flag[nAddr])
	{
		PowerAmp_Fault_Flag[nAddr] = 1;
		PowerAmp_Fault_Num++;
		
		FAULTLED_ON;
		PowerAmp_BuzzerOn_Flag = 1;
		if(!Record_Flag)
			BUZZER_ON;
	}	
}

void pre_Recv_Inspection_Ack(int nAddr,int nStatus)
{
	if(nAddr<1 || nAddr>10)
		return;
	
	nAddr--;
	if(!config_IsPowAmpCfged(nAddr))
		return;
	
	if(nStatus)
		Fault(nAddr);
	else
		Normal(nAddr);
}
