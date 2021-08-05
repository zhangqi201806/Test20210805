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

#define LED_ROW_SUM         11              //指示灯行扫描总数

static uint8_t	LedSta[LED_SUM];
static uint8_t  LedFlashSta[LED_SUM];
static uint8_t  LedRowNum[LED_ROW_SUM];

const uint8_t LedTable[LED_SUM] = 
{
    0,6, 12,18,24,30,36,42,48,54,60,
	1,7, 13,19,25,31,37,43,49,55,61,
	2,8, 14,20,26,32,38,44,50,56,62,
	3,9, 15,21,27,33,39,45,51,57,63,
	4,10,16,22,28,34,40,46,52,58,64,
	5,11,17,23,29,35,41,47,53,59,65,0
};

static uint8_t  FlashFlag;
static u8 nTimerCount;
void led_TimerIrq(void)
{
	nTimerCount++;
	if(nTimerCount >= 250) //500ms  
	{
		nTimerCount = 0;
		FlashFlag = ~FlashFlag;
	}
}

void led_CloseAllLed(void)
{ 
    LED_ROW1_L;
    LED_ROW2_L;
    LED_ROW3_L;
    LED_ROW4_L;
    LED_ROW5_L;
    LED_ROW6_L;
    LED_ROW7_L;
    LED_ROW8_L;
    LED_ROW9_L;
    LED_ROW10_L;
    LED_ROW11_L;
}

void led_LightLed(uint8_t lednum)
{
    LedSta[lednum] = 1;
	LedFlashSta[lednum] = 0;
}
void led_FlashLed(uint8_t lednum)
{
    LedSta[lednum] = 1;
    LedFlashSta[lednum] = 1;
}

void led_CloseLed(uint8_t lednum)
{
    LedSta[lednum] = 0;
    LedFlashSta[lednum] = 0;
}

void led_ToggleLed(uint8_t lednum)
{
		if(LedSta[lednum])
			LedSta[lednum] = 0;
		else
			LedSta[lednum] = 1;
}

void led_WriteLedVal(uint8_t i)
{
    uint8_t j, num;
    
    for(j=0; j<LED_ROW_SUM; j++)
    {
        num = LedTable[i*LED_ROW_SUM + j];
        if(LedSta[num] == 1)
        {
            if(LedFlashSta[num] == 1)
            {
                if(FlashFlag)
                {
                    LedRowNum[j] = 1;
                }
                else
                {
                    LedRowNum[j] = 0;
                }
            }
            else
            {
                LedRowNum[j] = 1;
            }
        }
        else
        {
            LedRowNum[j] = 0;
        }
    }
    
    if(LedRowNum[0])
        LED_ROW1_H;
    if(LedRowNum[1])
        LED_ROW2_H;
    if(LedRowNum[2])
        LED_ROW3_H;
    if(LedRowNum[3])
        LED_ROW4_H;
    if(LedRowNum[4])
        LED_ROW5_H;
    if(LedRowNum[5])
        LED_ROW6_H;
    if(LedRowNum[6])
        LED_ROW7_H;
    if(LedRowNum[7])
        LED_ROW8_H;
    if(LedRowNum[8])
        LED_ROW9_H;
    if(LedRowNum[9])
        LED_ROW10_H;
    if(LedRowNum[10])
        LED_ROW11_H;
}   

void led_ManageLed(void)
{
    led_WriteLedVal(KeyScanCount);
}
