#ifndef __POWAMP_H__
#define __POWAMP_H__

#include "stm32f10x.h"

#define PA_NUM  (10)

void PowerAmp_Online(int nAddr);

int PowerAmp_IsReged(int nAddr);
int PowerAmp_GetRegedCount(void);

enum {PA_UPDATE_NONE=0,PA_UPDATE_REG_ONLINE,PA_UPDATE_NOTREG_ONLINE,PA_UPDATE_REG_OFFLINE,PA_UPDATE_NOTREG_OFFLINE};
enum {PA_STATE_NOTEXIST=0,PA_STATE_REG_ONLINE,PA_STATE_REG_OFFLINE,PA_STATE_NOTREG_ONLINE};
void PowerAmp_CheckStateProc(void);
u8 PowerAmp_GetOfflinePa(void);

void PowerAmp_SetStartRegFlag(void);
int PowerAmp_IsReging(void);
int PowerAmp_IsRegCompleted(void);
void PowerAmp_OnRegCompleted(void);

#endif
