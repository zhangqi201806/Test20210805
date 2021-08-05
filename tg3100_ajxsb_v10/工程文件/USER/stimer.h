#ifndef __STIMER_H__
#define __STIMER_H__

#include "stm32f10x.h"

void stimer_Irq(void);
void stimer_Start(u32 nCount);
int stimer_IsArrived(void);

#endif

