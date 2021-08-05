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

void ptx_Send_MicOn_Command(void)
{
	txandre_SendPackage(0xF0,MicOn_Command,0,0);
}

void ptx_Send_Start_Command(void)
{
	txandre_SendPackage(0xF0,StartupPowerAmp_Command,0,0);
}

void ptx_Send_Play_Command(int nAddr)
{
	txandre_SendPackage(nAddr,Play_Command,0,0);
}

void ptx_Send_Stop_Command(int nAddr)
{
	txandre_SendPackage(nAddr,StopPowerAmp_Command,0,0);
}

void ptx_Send_AllStop_Command(void)
{
	txandre_SendPackage(0xF0,StopPowerAmp_Command,0,0);
}


