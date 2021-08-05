#ifndef __PTX_H__
#define __PTX_H__

#include "stm32f10x.h"

//����ȫ��MicOn����
void ptx_Send_MicOn_Command(void);

//����ȫ����������
void ptx_Send_Start_Command(void);

//���� ����1-10
void ptx_Send_Play_Command(int nAddr);

//����ֹͣ ����1-10
void ptx_Send_Stop_Command(int nAddr);

void ptx_Send_AllStop_Command(void);

#endif
