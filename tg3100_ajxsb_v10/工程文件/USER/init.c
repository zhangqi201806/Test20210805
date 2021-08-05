#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

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
#include "ui_key.h"
#include "config.h"
#include "timer.h"

static void InitHardware(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	
	txandre_Init();
	
	LCD_HardwareInit();		
	USART_Configuration();
	timer_Configuration();
	ledandkey_Configuration(); 
}

void init_InitSystem(void)
{
	InitHardware();
	
	config_ReadAllConfig();
	
	SelfCheck(0);
}
