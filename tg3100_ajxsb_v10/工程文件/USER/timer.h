#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"

extern int ReSendTime_Load,ReSendTime_Counter,ReSendTime_Flag;
extern int Time80ms_Counter,Timer80msFlag;//定时器中断使用
extern int Timer500msCounter,Timer500msFlag;
extern int TimerBglcdCounter,TimerBglcdFlag;//lcd背光计时

void timer_Configuration(void);
void timer_TIM1_Irq(void);
void timer_TIM3_Irq(void);

#endif
