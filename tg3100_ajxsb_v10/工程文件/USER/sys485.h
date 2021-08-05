#ifndef __USART1TO485_H
#define __USART1TO485_H	 

#include "stm32f10x.h"

#define BAUDRATE            4800            //波特率 = 9600bps
#define PIN_485CTRL         GPIO_Pin_8
#define PIN_485TX           GPIO_Pin_9
#define PIN_485RX           GPIO_Pin_10

#define RXBufferSize 200//接收缓冲区大小
#define TXBufferSize 200//发送缓冲区大小

extern u8 USART1_TxBuffer[TXBufferSize],USART1_RxBuffer[RXBufferSize];//接收发送缓存
extern u8 Receive_Finsh_Flag,RxLength;//接收中断和处理使用 

//串口 TIM3 初始化 
void USART_Configuration(void); //485串口初始化  9600 波特率 104.16us 实际104.2us一位		
void sys485_USART1_Irq(void);
void sys485_Proc(void);

void sys485_SetReSendTimer(int nCount);

#endif
