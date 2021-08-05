#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"

#include "main.h"
#include "adjust.h" 
#include "bitmap.h"
#include "flash.h"
#include "init.h"
#include "ui_main.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "ledandkey.h"
#include "sys.h"  
#include "sys485.h"   
#include "txandre.h"  
#include "timer.h"
#include "powamp.h"

u8 USART1_TxBuffer[TXBufferSize],USART1_RxBuffer[RXBufferSize];//接收发送缓存
u8 Receive_Finsh_Flag,RxLength;//接收中断和处理使用 

void USART_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);

	USART_DeInit(USART1);

	GPIO_InitStructure.GPIO_Pin = PIN_485TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_485RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_485CTRL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	RS485_RECE;

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);
		
	USART_InitStructure.USART_BaudRate = BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);    
}

void sys485_USART1_Irq(void)
{
	u8 Res;		
	if(USART_GetITStatus(USART1, USART_IT_RXNE)!= RESET)  //接收中断
	{
		Res=USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);

		if(Receive_Finsh_Flag==0)///接收完成了吗  =1 完成  =0 未完成
		{				
			if((Res == 0xa5)&&(RxLength == 0))
			{
				USART1_RxBuffer[RxLength] = Res;
				RxLength++;    			
			}
			else if(((Res==0x00)||(Res==0x01)||(Res==0x02)||(Res==0x03)||(Res==0x04)||(Res==0x05)||(Res==0x06)||(Res==0x07)||(Res==0x08)||(Res==0x09)||(Res==0x0A))&&(RxLength == 1))
			{
				USART1_RxBuffer[RxLength] = Res;
				RxLength++;    
			}      
			else if(RxLength > 1)
			{
				USART1_RxBuffer[RxLength] = Res;
				RxLength++;			
				if(USART1_RxBuffer[2]+4== RxLength)
				{   
					RxLength = 0;
					Receive_Finsh_Flag=1;
				}
			}											              
			else 
			{
				RxLength = 0;
			}	
		}						
	} 
}

void USART1_TX(unsigned char *var ,unsigned char len)
{
	unsigned char i = 0;
	
	TIM_Cmd(TIM3, DISABLE);
	RS485_SEND;
	
	while((USART1->SR&0X40)==0);	
	for(i=0;i<len;i++)
	{ 				
		USART_SendData(USART1,*var++);
		while((USART1->SR&0X40)==0);		
	}
		
	RS485_RECE;	
	TIM_Cmd(TIM3, ENABLE);
}

void sys485_SetReSendTimer(int nCount)
{
	ReSendTime_Load = nCount;
	ReSendTime_Flag=0;
	ReSendTime_Counter=nCount;
	Receive_Sucess_Flag=0;
}

static void RecvAckProc(void)
{
	if(bSend_Command == 1) 
	{
		int nTryCount = 0;
		
		bSend_Command = 0;
		
		while(1)
		{
			do
			{
				txandre_RecvAckProc();
			}
			while(!ReSendTime_Flag && !Receive_Sucess_Flag);

			if(Receive_Sucess_Flag)
			{					
				Time80ms_Counter=0;
				Timer80msFlag	= 0;
				
				txandre_FlashTaskFlag();
				break;
			}
			else if(ReSendTime_Flag) 
			{
				if(nTryCount<3)
				{
					USART1_TX(USART1_TxBuffer,USART1_TxBuffer[2]+4);//重发上一条
					ReSendTime_Flag	= 0;
					if(!USART1_TxBuffer[1])
					{
						ReSendTime_Flag	= 0;
					}
					ReSendTime_Counter = ReSendTime_Load;
					nTryCount++;
				}
				else
				{
					if(USART1_TxBuffer[1] == MB_ADDR && !DelAllFile_Flag) //显示主板错误
						ui_ShowSys485Error(USART1_TxBuffer[1],USART1_TxBuffer[3]);
					else if(USART1_TxBuffer[1] < 11 && !PowerAmp_IsReging()) //显示功放错误,注册时错误不显示
						ui_ShowSys485Error(USART1_TxBuffer[1],USART1_TxBuffer[3]);
					
					Time80ms_Counter=0;
					Timer80msFlag	= 0;
					
					txandre_FlashTaskFlag();
					break;
				}
			}
		}
	}
	else if(bSend_Command == 2)
	{
		bSend_Command = 0;
		do
		{
			txandre_RecvAckProc();
		}
		while(!ReSendTime_Flag && !Receive_Sucess_Flag);
	}	
}

void sys485_Proc(void)
{
	if(!Task_Flag)
		txandre_AddPeriodTask();

	do
	{
		txandre_SendCmdProc();
		RecvAckProc();
	}
	while(Task_Flag);
}


