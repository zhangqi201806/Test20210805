#ifndef __FIREEMERGENCYBROADCASTSYSTEMPROTOCOL_H
#define __FIREEMERGENCYBROADCASTSYSTEMPROTOCOL_H	 
#include "stm32f10x.h"

#define RS485_SEND          GPIO_SetBits(GPIOA, PIN_485CTRL)
#define RS485_RECE          GPIO_ResetBits(GPIOA, PIN_485CTRL)

extern u8 CfgNativeAddress;//���ñ�����ַ
extern u8 CfgPowerAmp[400];//���ù��ź�����Ӧ��ϵ���� 8(ÿ������)*5(ÿ������)*10(���ź���)=400
extern u8 Single_Or_Sequence_Flag;
extern u8 Record_Flag,ForceOn_Flag,DelFile_Flag;
extern u8 SlaveAdress;
extern u8 Task_Flag;
extern u8 TX_Task_Command_Flag[];
extern u8 Receive_Sucess_Flag;
extern u8 bSend_Command;
//��ʼ�� �̶�ֵ
#define Frame_head 						0xA5
//Ŀ�ĵ�ַ(�ӻ���ַ )
#define MB_ADDR 				0x00
#define PA1_ADDR  	 		0x01
#define PA2_ADDR   		0x02
#define PA3_ADDR  	 	0x03
#define PA4_ADDR  	 	0x04
#define PA5_ADDR  	 	0x05
#define PA6_ADDR  		0x06
#define PA7_ADDR  		0x07
#define PA8_ADDR  		0x08
#define PA9_ADDR  		0x09
#define PA10_ADDR	  	0x0A

#define Plate_1_Adress  				0x0B
#define Plate_2_Adress  				0x0C
#define Plate_3_Adress	  				0x0D
#define Plate_4_Adress	  				0x0E
//���ݳ���	(�����������ֽں��������ݵĳ��� �ӻ���Ӧ���ֽں��������ݵĳ���)
#define Data_length_1_Byte 				0x01
#define Data_length_2_Byte  			0x02
#define Data_length_3_Byte  			0x03
#define Data_length_4_Byte  			0x04
#define Data_length_5_Byte  			0x05
#define Data_length_6_Byte  			0x06
#define Data_length_7_Byte  			0x07
#define Data_length_8_Byte 				0x08
#define Data_length_9_Byte 				0x09
#define Data_length_10_Byte 			0x0A
#define Data_length_11_Byte 			0x0B
#define Data_length_12_Byte 			0x0C
#define Data_length_13_Byte 			0x0D
#define Data_length_14_Byte 			0x0E
#define Data_length_15_Byte 			0x0F
#define Data_length_16_Byte 			0x10
#define Data_length_17_Byte 			0x11
#define Data_length_18_Byte 			0x12
#define Data_length_19_Byte 			0x13
#define Data_length_20_Byte 			0x14
//�����ֽ�
#define Inspection_Command 							0x01//Ѳ������
#define Into_Emer_Command 							0x02//����Ӧ���㲥����
#define Out_Emer_Command 							0x03//�˳�Ӧ���㲥����
#define Into_MP3_Command 							0x04//����MP3����
#define Out_MP3_Command 							0x05//�˳�MP3����
#define Into_Line1_Command 							0x06//����Line1����
#define Out_Line1_Command 							0x07//�˳�Line1����
#define Into_Line2_Command 							0x08//����Line2����
#define Out_Line2_Command 							0x09//����Line2����
#define Into_PlayBack_Command 						0x0A//����ط�����
#define Out_PlayBack_Command 						0x0B//�˳��ط�����
#define Play_Command 								0x0C//��������
#define StopPlay_Command 							0x0D//ֹͣ����
#define Forward_Command 							0x0E//�������
#define Backward_Command 							0x0F//��������
#define UP_Command 									0x10//��һ������
#define Down_Command 								0x11//��һ������
#define Single_Command 								0x12//����ѭ������
#define Sequence_Command 							0x13//˳��ѭ������
#define Automatic_Command 							0x14//�Զ�ģʽ����
#define Manual_Command 								0x15//�ֶ�ģʽ����
#define FileInput_Command 							0x16//�ļ���������
#define StartupPowerAmp_Command 					0x17//������������
#define StopPowerAmp_Command 						0x18//ͣ����������
#define PlayBackDel_Command 						0x19//¼��ɾ������
#define Global_Reset_Command 						0x1A//ȫ�ָ�λ����
#define Global_SelfCheck_Command 					0x1B//ȫ���Լ�����
#define MicOn_Command 										0x1C//MICģʽ ���͸����ŵ�����
#define Area_Fault_Mian_Command 					0x1E//ĳ������ ���͸����������
#define Area_Normal_Mian_Command 					0x1F//ĳ������ ���͸����������
#define DelAllFile_Command 								0x20//ɾ��ȫ���ļ�����
#define GetMainInf_Command 							0x29//��ȡ������Ϣ����
#define GetFileInputProcess_Command 				0x31//��ȡ�ļ������������
#define GetSongSum_Command 							0x32//��ȡ��������������
#define GetPlayBackSum_Command 						0x33//��ȡ�ط�����������
#define GetSongCurrentNum_Command 	 				0x34//��ȡ��ǰ������������
#define GetPlayBackCurrentNum_Command 				0x35//��ȡ��ǰ�ط���������
#define GetPlayBackHistoryTime_Command 			    0x36//��ȡ�ط���ʷʱ������
#define GetPlayBackPlayProcessTime_Command 			0x37//��ȡ�طŲ��Ž���ʱ������
#define GetPlayBackPlayProcessBar_Command 		    0x38//��ȡ�طŲ��Ž���������
#define GetEmerPlayProcessTime_Command 				0x39//��ȡӦ���㲥���Ž���ʱ������
#define GetEmerPlayProcessBar_Command 				0x3A//��ȡӦ���㲥���Ž���������
#define Get1To30AreaInf_Command 					0x3D//��ȡ1-30���쳣��Ϣ����
#define Get31To60AreaInf_Command 					0x3E//��ȡ31-60���쳣��Ϣ����
#define Get61To90AreaInf_Command 					0x3F//��ȡ61-90���쳣��Ϣ����
#define Get91To120AreaInf_Command 					0x40//��ȡ91-120���쳣��Ϣ����
#define Get121To150AreaInf_Command 					0x41//��ȡ121-150���쳣��Ϣ����
#define Get151To180AreaInf_Command 					0x42//��ȡ151-180���쳣��Ϣ����
#define Get181To210AreaInf_Command 					0x43//��ȡ181-210���쳣��Ϣ����
#define Get211To240AreaInf_Command 					0x44//��ȡ211-240���쳣��Ϣ����
#define Get241To270AreaInf_Command 					0x45//��ȡ241-270���쳣��Ϣ����

#define StartupArea_Command 						0x60//����������       +�������� (N0H(����λ)+N1H(����λ))
#define StopArea_Command 							0x61//ͣ��������       +�������� (N0H(����λ)+N1H(����λ))

#define CfgSystemTime_Command 						0x70//����ϵͳʱ������               +�������� (N0H(���λ)+N1H(���λ)+N2H(��)+N3H(��)+N4H(ʱ)+N5H(��))
#define CfgNativeAddress_Command 					0x71//���ñ�����ַ����               +�������� (N0H)
#define Cfg1PowerAmp1to60Area_Command 				0x72//���õ�ַ1������(1-60)���� 	 +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg1PowerAmp61to120Area_Command 			0x73//���õ�ַ1������(61-120)����    +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg1PowerAmp121to180Area_Command 			0x74//���õ�ַ1������(121-180)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg1PowerAmp181to240Area_Command 			0x75//���õ�ַ1������(181-240)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg1PowerAmp241to270Area_Command 			0x76//���õ�ַ1������(241-270)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg2PowerAmp1to60Area_Command 				0x77//���õ�ַ2������(1-60)���� 	 +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg2PowerAmp61to120Area_Command 			0x78//���õ�ַ2������(61-120)����    +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg2PowerAmp121to180Area_Command 			0x79//���õ�ַ2������(121-180)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg2PowerAmp181to240Area_Command 			0x7A//���õ�ַ2������(181-240)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg2PowerAmp241to270Area_Command 			0x7B//���õ�ַ2������(241-270)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg3PowerAmp1to60Area_Command 				0x7C//���õ�ַ3������(1-60)���� 	 +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg3PowerAmp61to120Area_Command 			0x7D//���õ�ַ3������(61-120)����    +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg3PowerAmp121to180Area_Command 			0x7E//���õ�ַ3������(121-180)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg3PowerAmp181to240Area_Command 			0x7F//���õ�ַ3������(181-240)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg3PowerAmp241to270Area_Command 			0x80//���õ�ַ3������(241-270)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg4PowerAmp1to60Area_Command 				0x81//���õ�ַ4������(1-60)���� 	 +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg4PowerAmp61to120Area_Command 			0x82//���õ�ַ4������(61-120)����    +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg4PowerAmp121to180Area_Command 			0x83//���õ�ַ4������(121-180)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg4PowerAmp181to240Area_Command 			0x84//���õ�ַ4������(181-240)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg4PowerAmp241to270Area_Command 			0x85//���õ�ַ4������(241-270)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg5PowerAmp1to60Area_Command 				0x86//���õ�ַ5������(1-60)���� 	 +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg5PowerAmp61to120Area_Command 			0x87//���õ�ַ5������(61-120)����    +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg5PowerAmp121to180Area_Command 			0x88//���õ�ַ5������(121-180)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg5PowerAmp181to240Area_Command 			0x89//���õ�ַ5������(181-240)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg5PowerAmp241to270Area_Command 			0x8A//���õ�ַ5������(241-270)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg6PowerAmp1to60Area_Command 				0x8B//���õ�ַ6������(1-60)���� 	 +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg6PowerAmp61to120Area_Command 			0x8C//���õ�ַ6������(61-120)����    +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg6PowerAmp121to180Area_Command 			0x8D//���õ�ַ6������(121-180)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg6PowerAmp181to240Area_Command 			0x8E//���õ�ַ6������(181-240)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg6PowerAmp241to270Area_Command 			0x8F//���õ�ַ6������(241-270)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg7PowerAmp1to60Area_Command 				0x90//���õ�ַ7������(1-60)���� 	 +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg7PowerAmp61to120Area_Command 			0x91//���õ�ַ7������(61-120)����    +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg7PowerAmp121to180Area_Command 			0x92//���õ�ַ7������(121-180)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg7PowerAmp181to240Area_Command 			0x93//���õ�ַ7������(181-240)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg7PowerAmp241to270Area_Command 			0x94//���õ�ַ7������(241-270)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg8PowerAmp1to60Area_Command 				0x95//���õ�ַ8������(1-60)���� 	 +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg8PowerAmp61to120Area_Command 			0x96//���õ�ַ8������(61-120)����    +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg8PowerAmp121to180Area_Command 			0x97//���õ�ַ8������(121-180)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg8PowerAmp181to240Area_Command 			0x98//���õ�ַ8������(181-240)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg8PowerAmp241to270Area_Command 			0x99//���õ�ַ8������(241-270)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg9PowerAmp1to60Area_Command 				0x9A//���õ�ַ9������(1-60)���� 	 +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg9PowerAmp61to120Area_Command 			0x9B//���õ�ַ9������(61-120)����    +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg9PowerAmp121to180Area_Command 			0x9C//���õ�ַ9������(121-180)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg9PowerAmp181to240Area_Command 			0x9D//���õ�ַ9������(181-240)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg9PowerAmp241to270Area_Command 			0x9E//���õ�ַ9������(241-270)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg10PowerAmp1to60Area_Command 				0x9F//���õ�ַ10������(1-60)���� 	 +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg10PowerAmp61to120Area_Command 			0xA0//���õ�ַ10������(61-120)����   +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg10PowerAmp121to180Area_Command 			0xA1//���õ�ַ10������(121-180)����  +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg10PowerAmp181to240Area_Command 			0xA2//���õ�ַ10������(181-240)����  +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg10PowerAmp241to270Area_Command 			0xA3//���õ�ַ10������(241-270)����  +�������� (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)

#define GetSongName_Command 						0xC0//��ȡ����������

void txandre_Init(void);
void txandre_ClearAllTaskFlag(void);
void txandre_AddSetAddrTask(void);
void txandre_AddPeriodTask(void);
void txandre_AddSelfCheckTask(void);
void txandre_AddResetPowerAmpTask(void);
void txandre_FlashTaskFlag(void);	//����������������ȫ����־ 
void txandre_SendCmdProc(void);		//����ȫ��������
void txandre_RecvAckProc(void);   //����ȫ�����ܴ���

void txandre_SendPackage(int nAddr,int nCmd,int nLen,u8 Buf[]);

#endif
