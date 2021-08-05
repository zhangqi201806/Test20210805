#include "stimer.h"

static u32 nTimerCount;
void stimer_Irq(void)
{
	if(nTimerCount)
		nTimerCount--;
}

void stimer_Start(u32 nCount)
{
	nTimerCount = nCount;
}

int stimer_IsArrived(void)
{
	return (nTimerCount?0:1);
}

