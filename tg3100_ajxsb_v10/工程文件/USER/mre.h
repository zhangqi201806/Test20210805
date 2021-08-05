#ifndef __MRE_H__
#define __MRE_H__

#include "stm32f10x.h"

extern u8 Area_Startup_Flag[270];   //区启动标志
extern u8 Area_Fault_Flag[270];			//区故障标志
extern u16 Area_Startup_Num,Area_Fault_Num;
extern u8 AreaFault_BuzzerOn_Flag;

extern u8 MainInfSta_UOnline_Flag,MainInfSta_MicOnline_Flag;

extern u16 MP3_Current_Num,MP3_Sum;

//回放相关变量
extern u8 PlaybackHistoryYear,PlaybackHistoryMon,PlaybackHistoryDay,PlaybackHistoryHou,PlaybackHistoryMin,PlaybackHistorySec;
extern u8 PlayBack_Current_Time_Min,PlayBack_Current_Time_Sec;;
extern u16 PlayBack_Current_Num,PlayBack_Current_Sum;

//应急广播相关变量
extern u8 Emergency_Current_Time_Min,Emergency_Current_Time_Sec;
extern u8 Record_Current_Time_Min,Record_Current_Time_Sec;

extern u8 Emergency_Mode,MP3_Mode,line1_Mode,line2_Mode,Playback_Mode;

///************************************************接收总处理**************************************************///
//接收 5种模式进入和退出
void Into_Emer_Handle(void);
void Out_Emer_Handle(void);
void Into_MP3_Handle(void);
void Out_MP3_Handle(void);
void Into_Line1_Handle(void);
void Out_Line1_Handle(void);
void Into_Line2_Handle(void);
void Out_Line2_Handle(void);
void Into_PlayBack_Handle(void);
void Out_PlayBack_Handle(void);

//播放相关的
void Forward_Handle(void);
void Backward_Handle(void);
void UP_Handle(void);
void Down_Handle(void);
void Single_Handle(void);
void Sequence_Handle(void);

void Automatic_Handle(void);					//接收 自动
void Manual_Handle(void);					//接收 手动
void PlayBackDel_Handle(void);				//录音删除
void DelAllFile_Handle(void);				//删除全部文件
void GetMainInf_Handle(void);				//获取主板信息
void GetFileInputProcess_Handle(void);		//文件导入进度
void GetSongName_Handle(void);				//获取MP3歌曲名字
void GetSongSum_Handle(void);				//获取MP3总数量
void GetPlayBackSum_Handle(void);			//获取回放总数量
void GetSongCurrentNum_Handle(void);			//获取MP3当前排名
void GetPlayBackCurrentNum_Handle(void);		//获取回放当前排名
void GetPlayBackHistoryTime_Handle(void);	//获取回放历史时间
void GetPlayBackPlayProcessTime_Handle(void);//获取回放播放进度时间
void GetPlayBackPlayProcessBar_Handle(void);	//获取回放播放进度条
void GetEmerPlayProcessTime_Handle(void);	//获取应急广播播放进度时间
void GetEmerPlayProcessBar_Handle(void);		//获取应急广播播放进度条
void Get30AreaInf_Handle(int nInfoArea);			//获取1-30区异常信息
void StartupArea_Handle(void);				//启动某区
void StopArea_Handle(void);					//停动某区

///************************************************接收分处理**************************************************///
//巡检中主板的回复
void mre_Recv_Inspection_Ack(int bFault);

//MP3 LINE1 LINE2 回放的 播放和停止播放函数
void Recive_Handle_SongPlay_Function(void);
void Recive_Handle_SongStopPlay_Function(void);
void Recive_Handle_Line1Play_Function(void);
void Recive_Handle_Line1StopPlay_Function(void);
void Recive_Handle_Line2Play_Function(void);
void Recive_Handle_Line2StopPlay_Function(void);
void Recive_Handle_PlayBackPlay_Function(void);
void Recive_Handle_PlayBackStopPlay_Function(void);

#endif
