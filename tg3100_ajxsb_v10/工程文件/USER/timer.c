#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

#include "lcd.h"
#include "led.h"
#include "timer.h"
#include "pre.h"
#include "key.h"
#include "stimer.h"

//T = (1+TIM_Prescaler)*(1+TIM_Period)/72M = (1+719)*(1+199)/72M = 2ms
void TIM1_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_DeInit(TIM1);                              
	TIM_TimeBaseStructure.TIM_Prescaler = 719;
	TIM_TimeBaseStructure.TIM_Period = 199;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);          
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); 
	TIM_Cmd(TIM1, ENABLE); 
}

//T = (1+TIM_Prescaler)*(1+TIM_Period)/72M = (1+7199)*(1+99)/72M = 10ms
void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 99; 
	TIM_TimeBaseStructure.TIM_Prescaler =7199; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE);  		
}

void timer_Configuration(void)
{
	TIM1_Configuration();
	TIM3_Configuration();
}

void timer_TIM1_Irq(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) 
	{
		TIM_ClearFlag(TIM1, TIM_FLAG_Update);
		
		led_TimerIrq();
		
		key_ManageKey();
		
		led_ManageLed();
		
		stimer_Irq();
	}
}

int ReSendTime_Load,ReSendTime_Counter,ReSendTime_Flag;
int Time80ms_Counter,Timer80msFlag;//定时器中断使用
int Timer500msCounter,Timer500msFlag;
int TimerBglcdCounter,TimerBglcdFlag;//lcd背光计时

void timer_TIM3_Irq(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		
		if(ReSendTime_Counter)
		{
			ReSendTime_Counter--;
			if(!ReSendTime_Counter)
				ReSendTime_Flag=1;
		}
		
		Time80ms_Counter++;
		if(Time80ms_Counter==8)
		{
			Time80ms_Counter=0;
			Timer80msFlag=1;
		}
		
		Timer500msCounter++;
		if(Timer500msCounter==50) //间隔6次巡检 发一次 主板信息获取
		{
			Timer500msCounter=0;
			Timer500msFlag=1;
		}
		
		TimerBglcdCounter++;
		if(TimerBglcdCounter == 18000) //lcd背光计时 3分钟
		{
			TimerBglcdCounter = 0;
			if(!TimerBglcdFlag)
			{
				TimerBglcdFlag = 1;
				BACK_LIGHT_OFF();
			}
		}
	}
}

