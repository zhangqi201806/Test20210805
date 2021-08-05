#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f10x.h"

#define KEY_NULL            0xFF
#define KEY_Play            0
#define KEY_Stop            1
#define KEY_RPT             2
#define KEY_SelfCheck       3
#define KEY_MENU            4
#define KEY_Emer            5
#define KEY_MP3             6
#define KEY_LINE1           7
#define KEY_LINE2           8
#define KEY_MANUAL          9
#define KEY_MUTE            10
#define KEY_1               11
#define KEY_2               12
#define KEY_3               13
#define KEY_4               14
#define KEY_5               15
#define KEY_6               16
#define KEY_7               17
#define KEY_8               18
#define KEY_9               19
#define KEY_10              20
#define KEY_11              21
#define KEY_12              22
#define KEY_13              23
#define KEY_14              24
#define KEY_15              25
#define KEY_16              26
#define KEY_17              27
#define KEY_18              28
#define KEY_19              29
#define KEY_20              30
#define KEY_21              31
#define KEY_22              32
#define KEY_23              33
#define KEY_24              34
#define KEY_25              35
#define KEY_26              36
#define KEY_27              37
#define KEY_28              38
#define KEY_29              39
#define KEY_30              40
#define KEY_SUM             41
#define KEY_UP_Short        42          
#define KEY_UP_Long         43         
#define KEY_DOWN_Short      44         
#define KEY_DOWN_Long       45 
#define KEY_BG      				46


extern uint8_t  SystemKeyValue;

void key_ManageKey(void);
void key_GetKey(void);

#endif

