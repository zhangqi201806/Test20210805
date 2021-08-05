#ifndef __TASK_H__
#define __TASK_H__

#include "stm32f10x.h"

void Play(void);
void Stop_Play(void);

void Fast_Forward(void);
void Fast_Back(void);
void Last_Song(void);
void Next_Song(void);

void Main_PlayFlag(int bSingle);

void PowAmp_PeriodTaskProc(void);
void PowAmp_All_Mic(void);
void PowAmp_All_Start(void);
void PowAmp_All_PlayOrStop(void);
void PowAmp_All_Stop(void);

void PowAmp_Play(int nArea);
void PowAmp_Stop(int nArea);

#endif
