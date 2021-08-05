#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"

//蜂鸣器输出
#define PIN_BUZZER          GPIO_Pin_11
//功放故障指示灯
#define PIN_FAULTLED        GPIO_Pin_2

//蜂鸣器输出
#define BUZZER_ON           GPIO_SetBits(GPIOB, PIN_BUZZER)
#define BUZZER_OFF          GPIO_ResetBits(GPIOB, PIN_BUZZER)
#define BUZZER_STATE				GPIO_ReadOutputDataBit(GPIOB, PIN_BUZZER)

//功放故障指示灯
#define FAULTLED_ON         GPIO_ResetBits(GPIOA, PIN_FAULTLED)
#define FAULTLED_OFF        GPIO_SetBits(GPIOA, PIN_FAULTLED)
#define FAULTLED_STATE			(!GPIO_ReadOutputDataBit(GPIOA, PIN_FAULTLED))

#define LED_BROADCAST       0
#define LED_MP3             1
#define LED_OUTSIDE1        2
#define LED_OUTSIDE2        3
#define LED_MANUAL          4
#define LED_MUTE            5
#define LED_WORK1           6
#define LED_WORK30          35
#define LED_FAULT1          36
#define LED_FAULT30         65
#define LED_FAULT           66              //功放故障灯
#define LED_SUM             67              //指示灯总数

void led_TimerIrq(void);
void led_CloseAllLed(void);
void led_LightLed(uint8_t lednum);
void led_FlashLed(uint8_t lednum);
void led_CloseLed(uint8_t lednum);
void led_ToggleLed(uint8_t lednum);
void led_WriteLedVal(uint8_t i);
void led_ManageLed(void);

#endif
