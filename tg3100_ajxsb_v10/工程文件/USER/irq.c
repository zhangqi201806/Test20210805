#include "sys485.h"   
#include "key.h" 
#include "timer.h"

void TIM3_IRQHandler(void)  	//巡检使用  100ms
{
	timer_TIM3_Irq();
}
void USART1_IRQHandler(void)	   	
{	
	sys485_USART1_Irq();
} 
void TIM1_UP_IRQHandler(void)	//2ms定时 按键和LED
{
  timer_TIM1_Irq();
}
