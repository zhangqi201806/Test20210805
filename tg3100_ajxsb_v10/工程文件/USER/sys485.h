#ifndef __USART1TO485_H
#define __USART1TO485_H	 

#include "stm32f10x.h"

#define BAUDRATE            4800            //������ = 9600bps
#define PIN_485CTRL         GPIO_Pin_8
#define PIN_485TX           GPIO_Pin_9
#define PIN_485RX           GPIO_Pin_10

#define RXBufferSize 200//���ջ�������С
#define TXBufferSize 200//���ͻ�������С

extern u8 USART1_TxBuffer[TXBufferSize],USART1_RxBuffer[RXBufferSize];//���շ��ͻ���
extern u8 Receive_Finsh_Flag,RxLength;//�����жϺʹ���ʹ�� 

//���� TIM3 ��ʼ�� 
void USART_Configuration(void); //485���ڳ�ʼ��  9600 ������ 104.16us ʵ��104.2usһλ		
void sys485_USART1_Irq(void);
void sys485_Proc(void);

void sys485_SetReSendTimer(int nCount);

#endif
