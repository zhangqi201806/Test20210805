#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"

extern int ReSendTime_Load,ReSendTime_Counter,ReSendTime_Flag;
extern int Time80ms_Counter,Timer80msFlag;//��ʱ���ж�ʹ��
extern int Timer500msCounter,Timer500msFlag;
extern int TimerBglcdCounter,TimerBglcdFlag;//lcd�����ʱ

void timer_Configuration(void);
void timer_TIM1_Irq(void);
void timer_TIM3_Irq(void);

#endif
