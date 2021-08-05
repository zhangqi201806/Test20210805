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

uint8_t  KeyScanCount;

void ledandkey_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	//LED1/L1/L2/L3/L4/L5
	GPIO_InitStructure.GPIO_Pin = PIN_FAULTLED | PIN_LINE1 | PIN_LINE2 | PIN_LINE3 | PIN_LINE4 | PIN_LINE5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, PIN_FAULTLED);
	
	//R8/R9/R10/R2/R3/R4/R5/R6/R11/buzzer
	GPIO_InitStructure.GPIO_Pin = PIN_ROW2 | PIN_ROW3 | PIN_ROW4 | PIN_ROW5 | PIN_ROW6 | PIN_ROW8 | PIN_ROW9 | PIN_ROW10 | PIN_ROW11 | PIN_BUZZER;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//L6/R7
	GPIO_InitStructure.GPIO_Pin = PIN_LINE6 | PIN_ROW7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//R1
	GPIO_InitStructure.GPIO_Pin = PIN_ROW1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
//K5/K6/K7
	GPIO_InitStructure.GPIO_Pin = PIN_KEY5 | PIN_KEY6 | PIN_KEY7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//K4
	GPIO_InitStructure.GPIO_Pin = PIN_KEY4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//K1/K2/K3
	GPIO_InitStructure.GPIO_Pin = PIN_KEY1 | PIN_KEY2 | PIN_KEY3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//KEY_UP KEY_DOWN	
	GPIO_InitStructure.GPIO_Pin  = PIN_UP|PIN_Down;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Fun_WriteLine1(void)
{
    LED_LINE6_H;
    LED_LINE1_L;
}

void Fun_WriteLine2(void)
{
    LED_LINE1_H;
    LED_LINE2_L;
}

void Fun_WriteLine3(void)
{
    LED_LINE2_H;
    LED_LINE3_L;
}

void Fun_WriteLine4(void)
{
    LED_LINE3_H;
    LED_LINE4_L;
}

void Fun_WriteLine5(void)
{
    LED_LINE4_H;
    LED_LINE5_L;
}

void Fun_WriteLine6(void)
{
    LED_LINE5_H;
    LED_LINE6_L;
}

void (*const Fun_ScanLine[KEY_LINE_SUM])(void) = 
{
    Fun_WriteLine1, Fun_WriteLine2, Fun_WriteLine3, Fun_WriteLine4, Fun_WriteLine5, Fun_WriteLine6
};
