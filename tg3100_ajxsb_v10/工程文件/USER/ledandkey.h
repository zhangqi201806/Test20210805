#ifndef __LEDANDKEY_H__
#define __LEDANDKEY_H__

#include "stm32f10x.h"
//指示灯列输出
#define PIN_LINE1           GPIO_Pin_3
#define PIN_LINE2           GPIO_Pin_4
#define PIN_LINE3           GPIO_Pin_5
#define PIN_LINE4           GPIO_Pin_6
#define PIN_LINE5           GPIO_Pin_7
#define PIN_LINE6           GPIO_Pin_4
#define LED_LINE1_H         GPIO_ResetBits(GPIOA, PIN_LINE1)
#define LED_LINE1_L         GPIO_SetBits(GPIOA, PIN_LINE1)
#define LED_LINE2_H         GPIO_ResetBits(GPIOA, PIN_LINE2)
#define LED_LINE2_L         GPIO_SetBits(GPIOA, PIN_LINE2)
#define LED_LINE3_H         GPIO_ResetBits(GPIOA, PIN_LINE3)
#define LED_LINE3_L         GPIO_SetBits(GPIOA, PIN_LINE3)
#define LED_LINE4_H         GPIO_ResetBits(GPIOA, PIN_LINE4)
#define LED_LINE4_L         GPIO_SetBits(GPIOA, PIN_LINE4)
#define LED_LINE5_H         GPIO_ResetBits(GPIOA, PIN_LINE5)
#define LED_LINE5_L         GPIO_SetBits(GPIOA, PIN_LINE5)
#define LED_LINE6_H         GPIO_ResetBits(GPIOC, PIN_LINE6)
#define LED_LINE6_L         GPIO_SetBits(GPIOC, PIN_LINE6)
//指示灯行输出
#define PIN_ROW1            GPIO_Pin_2
#define PIN_ROW2            GPIO_Pin_3
#define PIN_ROW3            GPIO_Pin_4
#define PIN_ROW4            GPIO_Pin_5
#define PIN_ROW5            GPIO_Pin_6
#define PIN_ROW6            GPIO_Pin_7
#define PIN_ROW7            GPIO_Pin_5
#define PIN_ROW8            GPIO_Pin_0
#define PIN_ROW9            GPIO_Pin_1
#define PIN_ROW10           GPIO_Pin_2
#define PIN_ROW11           GPIO_Pin_10

#define LED_ROW1_H          GPIO_ResetBits(GPIOD, PIN_ROW1)
#define LED_ROW1_L          GPIO_SetBits(GPIOD, PIN_ROW1)
#define LED_ROW2_H          GPIO_ResetBits(GPIOB, PIN_ROW2)
#define LED_ROW2_L          GPIO_SetBits(GPIOB, PIN_ROW2)
#define LED_ROW3_H          GPIO_ResetBits(GPIOB, PIN_ROW3)
#define LED_ROW3_L          GPIO_SetBits(GPIOB, PIN_ROW3)
#define LED_ROW4_H          GPIO_ResetBits(GPIOB, PIN_ROW4)
#define LED_ROW4_L          GPIO_SetBits(GPIOB, PIN_ROW4)
#define LED_ROW5_H          GPIO_ResetBits(GPIOB, PIN_ROW5)
#define LED_ROW5_L          GPIO_SetBits(GPIOB, PIN_ROW5)
#define LED_ROW6_H          GPIO_ResetBits(GPIOB, PIN_ROW6)
#define LED_ROW6_L          GPIO_SetBits(GPIOB, PIN_ROW6)
#define LED_ROW7_H          GPIO_ResetBits(GPIOC, PIN_ROW7)
#define LED_ROW7_L          GPIO_SetBits(GPIOC, PIN_ROW7)
#define LED_ROW8_H          GPIO_ResetBits(GPIOB, PIN_ROW8)
#define LED_ROW8_L          GPIO_SetBits(GPIOB, PIN_ROW8)
#define LED_ROW9_H          GPIO_ResetBits(GPIOB, PIN_ROW9)
#define LED_ROW9_L          GPIO_SetBits(GPIOB, PIN_ROW9)
#define LED_ROW10_H         GPIO_ResetBits(GPIOB, PIN_ROW10)
#define LED_ROW10_L         GPIO_SetBits(GPIOB, PIN_ROW10)
#define LED_ROW11_H         GPIO_ResetBits(GPIOB, PIN_ROW11)
#define LED_ROW11_L         GPIO_SetBits(GPIOB, PIN_ROW11)
//按键行输入
#define PIN_KEY1            GPIO_Pin_10
#define PIN_KEY2            GPIO_Pin_11
#define PIN_KEY3            GPIO_Pin_12
#define PIN_KEY4            GPIO_Pin_9
#define PIN_KEY5            GPIO_Pin_0
#define PIN_KEY6            GPIO_Pin_1
#define PIN_KEY7            GPIO_Pin_15
#define PIN_UP    		    GPIO_Pin_12
#define PIN_Down       	    GPIO_Pin_11

#define KEY_LINE_SUM        6               //指示灯列扫描总数
#define KEY_INPUT_SUM       7               //按键输入总数

extern uint8_t  KeyScanCount;
extern void (*const Fun_ScanLine[KEY_LINE_SUM])(void);

void ledandkey_Configuration(void);

#endif
