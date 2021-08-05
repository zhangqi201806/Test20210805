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
#include "pre.h" 
#include "powamp.h"  
#include "txandre.h"  
#include "mtx.h"
#include "mre.h"
#include "stimer.h"
#include "ui_key.h" 
#include "ui_poweramp.h" 

int main(void)
{
  init_InitSystem();
	
	txandre_AddSetAddrTask();
	sys485_Proc();
	
	ui_Registering();
	PowerAmp_SetStartRegFlag();
	while(PowerAmp_IsReging())
		sys485_Proc();
	ui_Register_Finshed();	

	stimer_Start(1000);//2sec
	while(!stimer_IsArrived())
	{
		if(SystemKeyValue == KEY_Stop)
			break;
	}

	SystemKeyValue=KEY_NULL;
	WndUpdate = 1;
	while(1)
	{	
		key_GetKey();
		
		ui_KeyProc();

		ui_WndProc();
		
		sys485_Proc();
		
		PowerAmp_CheckStateProc();
	}
}


