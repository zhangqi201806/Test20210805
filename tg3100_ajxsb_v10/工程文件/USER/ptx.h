#ifndef __PTX_H__
#define __PTX_H__

#include "stm32f10x.h"

//功放全局MicOn命令
void ptx_Send_MicOn_Command(void);

//功放全局启动命令
void ptx_Send_Start_Command(void);

//播放 功放1-10
void ptx_Send_Play_Command(int nAddr);

//功放停止 功放1-10
void ptx_Send_Stop_Command(int nAddr);

void ptx_Send_AllStop_Command(void);

#endif
