#ifndef __MRE_H__
#define __MRE_H__

#include "stm32f10x.h"

extern u8 Area_Startup_Flag[270];   //��������־
extern u8 Area_Fault_Flag[270];			//�����ϱ�־
extern u16 Area_Startup_Num,Area_Fault_Num;
extern u8 AreaFault_BuzzerOn_Flag;

extern u8 MainInfSta_UOnline_Flag,MainInfSta_MicOnline_Flag;

extern u16 MP3_Current_Num,MP3_Sum;

//�ط���ر���
extern u8 PlaybackHistoryYear,PlaybackHistoryMon,PlaybackHistoryDay,PlaybackHistoryHou,PlaybackHistoryMin,PlaybackHistorySec;
extern u8 PlayBack_Current_Time_Min,PlayBack_Current_Time_Sec;;
extern u16 PlayBack_Current_Num,PlayBack_Current_Sum;

//Ӧ���㲥��ر���
extern u8 Emergency_Current_Time_Min,Emergency_Current_Time_Sec;
extern u8 Record_Current_Time_Min,Record_Current_Time_Sec;

extern u8 Emergency_Mode,MP3_Mode,line1_Mode,line2_Mode,Playback_Mode;

///************************************************�����ܴ���**************************************************///
//���� 5��ģʽ������˳�
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

//������ص�
void Forward_Handle(void);
void Backward_Handle(void);
void UP_Handle(void);
void Down_Handle(void);
void Single_Handle(void);
void Sequence_Handle(void);

void Automatic_Handle(void);					//���� �Զ�
void Manual_Handle(void);					//���� �ֶ�
void PlayBackDel_Handle(void);				//¼��ɾ��
void DelAllFile_Handle(void);				//ɾ��ȫ���ļ�
void GetMainInf_Handle(void);				//��ȡ������Ϣ
void GetFileInputProcess_Handle(void);		//�ļ��������
void GetSongName_Handle(void);				//��ȡMP3��������
void GetSongSum_Handle(void);				//��ȡMP3������
void GetPlayBackSum_Handle(void);			//��ȡ�ط�������
void GetSongCurrentNum_Handle(void);			//��ȡMP3��ǰ����
void GetPlayBackCurrentNum_Handle(void);		//��ȡ�طŵ�ǰ����
void GetPlayBackHistoryTime_Handle(void);	//��ȡ�ط���ʷʱ��
void GetPlayBackPlayProcessTime_Handle(void);//��ȡ�طŲ��Ž���ʱ��
void GetPlayBackPlayProcessBar_Handle(void);	//��ȡ�طŲ��Ž�����
void GetEmerPlayProcessTime_Handle(void);	//��ȡӦ���㲥���Ž���ʱ��
void GetEmerPlayProcessBar_Handle(void);		//��ȡӦ���㲥���Ž�����
void Get30AreaInf_Handle(int nInfoArea);			//��ȡ1-30���쳣��Ϣ
void StartupArea_Handle(void);				//����ĳ��
void StopArea_Handle(void);					//ͣ��ĳ��

///************************************************���շִ���**************************************************///
//Ѳ��������Ļظ�
void mre_Recv_Inspection_Ack(int bFault);

//MP3 LINE1 LINE2 �طŵ� ���ź�ֹͣ���ź���
void Recive_Handle_SongPlay_Function(void);
void Recive_Handle_SongStopPlay_Function(void);
void Recive_Handle_Line1Play_Function(void);
void Recive_Handle_Line1StopPlay_Function(void);
void Recive_Handle_Line2Play_Function(void);
void Recive_Handle_Line2StopPlay_Function(void);
void Recive_Handle_PlayBackPlay_Function(void);
void Recive_Handle_PlayBackStopPlay_Function(void);

#endif
