#ifndef __PRE_H__
#define __PRE_H__

#include "stm32f10x.h"

extern u8 PowerAmp_BuzzerOn_Flag;

extern u8 PowerAmp_Offline_Flag;

extern u8 PowerAmp_Fault_Flag[10];
extern u8 PowerAmp_Fault_Num;

extern u8 PowerAmp_Start_Flag[10];
extern u8 PowerAmp_Start_Num;

extern u8 PowerAmp_Play_Flag[10];
extern u8 PowerAmp_Play_Num;

extern u8 PowerAmp_Mic_Flag[10];
extern u8 PowerAmp_Mic_Num;

void pre_Recv_Inspection_Ack(int nAddr,int nStatus);

#endif
