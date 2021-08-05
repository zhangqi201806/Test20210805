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
#include "mre.h" 
#include "pre.h" 
#include "timer.h" 
#include "ui_key.h"

#define KEY_INPUT1          GPIO_ReadInputDataBit(GPIOC, PIN_KEY1)
#define KEY_INPUT2          GPIO_ReadInputDataBit(GPIOC, PIN_KEY2)
#define KEY_INPUT3          GPIO_ReadInputDataBit(GPIOC, PIN_KEY3)
#define KEY_INPUT4          GPIO_ReadInputDataBit(GPIOB, PIN_KEY4)
#define KEY_INPUT5          GPIO_ReadInputDataBit(GPIOA, PIN_KEY5)
#define KEY_INPUT6          GPIO_ReadInputDataBit(GPIOA, PIN_KEY6)
#define KEY_INPUT7          GPIO_ReadInputDataBit(GPIOA, PIN_KEY7)
#define KEY_UP_Value  			GPIO_ReadInputDataBit(GPIOA,PIN_UP)
#define KEY_Down_Value  		GPIO_ReadInputDataBit(GPIOA,PIN_Down)


uint8_t  SystemKeyValue;

static uint8_t  KeyFlag;
static uint8_t  KeyValSave1;
static u32  		KeyCount;
static uint8_t  KeyLineSave;
static uint8_t  KeyValue;
static u32 			Long_or_Short_press_Counter;
static uint8_t  KeySoundTime;

union Bbyte 
{
    uint8_t Byte;
    struct
    {
        uint8_t Bit0  :1;   // Data Direction for Byte Bit 0 
        uint8_t Bit1  :1;   // Data Direction for Byte Bit 1 
        uint8_t Bit2  :1;   // Data Direction for Byte Bit 2 
        uint8_t Bit3  :1;   // Data Direction for Byte Bit 3 
        uint8_t Bit4  :1;   // Data Direction for Byte Bit 4 
        uint8_t Bit5  :1;   // Data Direction for Byte Bit 5 
        uint8_t Bit6  :1;   // Data Direction for Byte Bit 6 
        uint8_t Bit7  :1;   // Data Direction for Byte Bit 7 
    }Bits;
};

const uint8_t KeyCode[KEY_LINE_SUM][KEY_INPUT_SUM] =
{
    KEY_Emer,		KEY_1,KEY_7, KEY_13,KEY_19,KEY_25,KEY_Play,
    KEY_MP3, 		KEY_2,KEY_8, KEY_14,KEY_20,KEY_26,KEY_Stop,
    KEY_LINE1, 	KEY_3,KEY_9, KEY_15,KEY_21,KEY_27,KEY_RPT,
    KEY_LINE2, 	KEY_4,KEY_10,KEY_16,KEY_22,KEY_28,KEY_SelfCheck,
    KEY_MANUAL,	KEY_5,KEY_11,KEY_17,KEY_23,KEY_29,KEY_MENU,
		KEY_MUTE,		KEY_6,KEY_12,KEY_18,KEY_24,KEY_30,KEY_NULL,
};

uint8_t Fun_GetKeyData(void)
{
	union Bbyte keydata;

	keydata.Byte = KEY_NULL;
	keydata.Bits.Bit0 = KEY_INPUT1;
	keydata.Bits.Bit1 = KEY_INPUT2;
	keydata.Bits.Bit2 = KEY_INPUT3;
	keydata.Bits.Bit3 = KEY_INPUT4;
	keydata.Bits.Bit4 = KEY_INPUT5;
	keydata.Bits.Bit5 = KEY_INPUT6;
	keydata.Bits.Bit6 = KEY_INPUT7;
	keydata.Bits.Bit7 = 1;
	return (keydata.Byte);
}

static int nKeyInterTime;
static int nLastKey;
int Fun_ScanUpDownKey(void)
{
	if(nKeyInterTime)
	{
		nKeyInterTime--;
		return 0;
	}
	if(KeyFlag)
		return 0;

	if(KEY_UP_Value)
	{
		if(!Long_or_Short_press_Counter || nLastKey != 1) 
		{
			Long_or_Short_press_Counter = 1;
			nLastKey = 1;
		}
		else 
			Long_or_Short_press_Counter++;
		
	}
	else if(KEY_Down_Value)
	{
		if(!Long_or_Short_press_Counter || nLastKey != 2) 
		{
			Long_or_Short_press_Counter = 1;
			nLastKey = 2;
		}
		else 
			Long_or_Short_press_Counter++;
		
	}
	else
	{
		 if(Long_or_Short_press_Counter> 0x80)
		 {
			 KeyValue = (nLastKey == 1)?KEY_DOWN_Long:KEY_UP_Long;
		 }
		 else if(Long_or_Short_press_Counter)
		 {
			 KeyValue = (nLastKey == 2)?KEY_DOWN_Short:KEY_UP_Short;
		 }
		 else
			 return 0;
		 
		 Long_or_Short_press_Counter = 0;
		 KeyFlag = 1;
		 nKeyInterTime = 100;
		 
		 return 1;
	}
	return 0;
}

int Fun_ScanOtherKey(void)
{
	uint8_t first_val, second_val;
	
	if(nKeyInterTime)
	{
		nKeyInterTime--;
		return 0;
	}
	if(KeyFlag)
		return 0;
	
	first_val = Fun_GetKeyData();       
	if(first_val != KEY_NULL)       
	{
			second_val = Fun_GetKeyData();
			if(second_val != first_val)   
					KeyCount = 0;
			else 
			{
				KeyCount++;
				if(KeyCount == 1)
				{
					KeyLineSave = KeyScanCount;
					KeyValSave1 = second_val;
				}
				else if(second_val != KeyValSave1 || KeyLineSave != KeyScanCount)    
				{
					KeyCount = 1;
					KeyLineSave = KeyScanCount;
					KeyValSave1 = second_val;
					
				}
			}
	}
	else if(KeyCount && (KeyLineSave == KeyScanCount))       
	{
		uint8_t i, j;
		
		j = 1;
		for(i=0; i<KEY_INPUT_SUM; i++)
		{
			if((KeyValSave1 & j) == 0)
			{
					KeyValue = KeyCode[KeyLineSave][i];
					break;
			}
			j <<= 1;
		}
		if(KeyValue < KEY_SUM)
		{
			if(KeyValue == KEY_MUTE && KeyCount > 0x80)
				KeyValue = KEY_BG;

			KeyCount = 0;
			KeyFlag = 1;
			nKeyInterTime = 100;
			return 1;
		}
		KeyCount = 0;
	}
	return 0;
}

static u32 nScanFlag = 0;
void key_ManageKey(void)
{
	led_CloseAllLed();
	
	nScanFlag++;
	if(nScanFlag%2)
	{
		KeyScanCount++;
		if(KeyScanCount == KEY_LINE_SUM)
			 KeyScanCount = 0;
		Fun_ScanLine[KeyScanCount]();
	}
	else if(Fun_ScanUpDownKey())
	{
		BUZZER_ON;
		KeySoundTime = 40;
		
		//WndUpdate = 1;
		//OldWndFocus = CurWndFocus;
	}
	else if(Fun_ScanOtherKey())
	{
		BUZZER_ON;
		KeySoundTime = 40;
		
		//WndUpdate = 1;
		//OldWndFocus = CurWndFocus;
	}
	
	if(KeySoundTime)
	{
		KeySoundTime--;
		if(!KeySoundTime && !bCheckSelf_Flag)
			BUZZER_OFF;
	}
	
	if(!Record_Flag && (PowerAmp_BuzzerOn_Flag || AreaFault_BuzzerOn_Flag))
		BUZZER_ON;
}

void key_GetKey(void)
{
	if(KeyFlag)
	{
		SystemKeyValue = KeyValue;
		KeyFlag = 0;
		
		Lcd_BackLightOn();
	}
	else
		SystemKeyValue = 0xFF;
}
