#ifndef __MTX_H__
#define __MTX_H__

#include "stm32f10x.h"

#define mtx_SendCmd(PARAM)		txandre_SendPackage(MB_ADDR,PARAM,0,0)

#define mtx_Into_Emer_Command_Function()  								mtx_SendCmd(Into_Emer_Command)
#define mtx_Out_Emer_Command_Function()										mtx_SendCmd(Out_Emer_Command)
#define mtx_Into_MP3_Command_Function()										mtx_SendCmd(Into_MP3_Command)
#define mtx_Out_MP3_Command_Function()										mtx_SendCmd(Out_MP3_Command)
#define mtx_Into_Line1_Command_Function()									mtx_SendCmd(Into_Line1_Command)
#define mtx_Out_Line1_Command_Function()									mtx_SendCmd(Out_Line1_Command)
#define mtx_Into_Line2_Command_Function()									mtx_SendCmd(Into_Line2_Command)									
#define mtx_Out_Line2_Command_Function()									mtx_SendCmd(Out_Line2_Command)
#define mtx_Into_Playback_Command_Function()							mtx_SendCmd(Into_PlayBack_Command)
#define mtx_Out_Playback_Command_Function()								mtx_SendCmd(Out_PlayBack_Command)
#define mtx_Play_Command_Function()												mtx_SendCmd(Play_Command)
#define mtx_StopPlay_Command_Function()										mtx_SendCmd(StopPlay_Command)
#define mtx_Forward_Command_Function()										mtx_SendCmd(Forward_Command)
#define mtx_Backward_Command_Function()										mtx_SendCmd(Backward_Command)
#define mtx_Up_Command_Function()													mtx_SendCmd(UP_Command)
#define mtx_Down_Command_Function()												mtx_SendCmd(Down_Command)
#define mtx_Single_Command_Function()											mtx_SendCmd(Single_Command)
#define mtx_Sequence_Command_Function()										mtx_SendCmd(Sequence_Command)
#define mtx_Automatic_Command_Function()									mtx_SendCmd(Automatic_Command)
#define mtx_Manual_Command_Function()											mtx_SendCmd(Manual_Command)
#define mtx_PlayBackDel_Command_Function()								mtx_SendCmd(PlayBackDel_Command)
#define mtx_FileInput_Command_Function()									mtx_SendCmd(FileInput_Command)
#define mtx_GetFileInputProcess_Command_Function()				mtx_SendCmd(GetFileInputProcess_Command)
#define mtx_GetSongName_Command_Function()								mtx_SendCmd(GetSongName_Command)
#define mtx_GetSongSum_Command_Function()									mtx_SendCmd(GetSongSum_Command)
#define mtx_GetPlayBackSum_Command_Function()							mtx_SendCmd(GetPlayBackSum_Command)
#define mtx_GetSongCurrentNum_Command_Function()					mtx_SendCmd(GetSongCurrentNum_Command)
#define mtx_GetPlayBackCurrentNum_Command_Function()			mtx_SendCmd(GetPlayBackCurrentNum_Command)
#define mtx_GetPlayBackHistoryTime_Command_Function()			mtx_SendCmd(GetPlayBackHistoryTime_Command)
#define mtx_GetPlayBackPlayProcessTime_Command_Function()	mtx_SendCmd(GetPlayBackPlayProcessTime_Command)
#define mtx_GetPlayBackPlayProcessBar_Command_Function()	mtx_SendCmd(GetPlayBackPlayProcessBar_Command)
#define mtx_GetEmerPlayProcessTime_Command_Function()			mtx_SendCmd(GetEmerPlayProcessTime_Command)
#define mtx_GetEmerPlayProcessBar_Command_Function()			mtx_SendCmd(GetEmerPlayProcessBar_Command)
#define mtx_GetMainInf_Command_Function()									mtx_SendCmd(GetMainInf_Command)
#define mtx_DelAllFile_Command()													mtx_SendCmd(DelAllFile_Command)

void mtx_CfgNativeAddress_Command(void);//配置本机地址
void mtx_CfgSystemTime_Command(void);//配置系统时间

enum {INFO_AREA_1=1,INFO_AREA_2,INFO_AREA_3,INFO_AREA_4,INFO_AREA_5,INFO_AREA_6,INFO_AREA_7,INFO_AREA_8,INFO_AREA_9};
void mtx_Send_Get30AreaInf_Command(int nInfoArea);

//配置功放和区的对应关系
enum {PA_CFG_AREA_1=1,PA_CFG_AREA_2,PA_CFG_AREA_3,PA_CFG_AREA_4,PA_CFG_AREA_5};
void mtx_Send_CfgPa60Area_Command(int nAddr,int nCfgArea);

//启动 停动1-270区
void mtx_Send_StartupArea_Command(int nArea,int bStart);
#endif
