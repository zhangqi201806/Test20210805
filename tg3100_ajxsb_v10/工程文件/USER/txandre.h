#ifndef __FIREEMERGENCYBROADCASTSYSTEMPROTOCOL_H
#define __FIREEMERGENCYBROADCASTSYSTEMPROTOCOL_H	 
#include "stm32f10x.h"

#define RS485_SEND          GPIO_SetBits(GPIOA, PIN_485CTRL)
#define RS485_RECE          GPIO_ResetBits(GPIOA, PIN_485CTRL)

extern u8 CfgNativeAddress;//设置本机地址
extern u8 CfgPowerAmp[400];//配置功放和区对应关系数组 8(每个大区)*5(每个功放)*10(功放号码)=400
extern u8 Single_Or_Sequence_Flag;
extern u8 Record_Flag,ForceOn_Flag,DelFile_Flag;
extern u8 SlaveAdress;
extern u8 Task_Flag;
extern u8 TX_Task_Command_Flag[];
extern u8 Receive_Sucess_Flag;
extern u8 bSend_Command;
//起始符 固定值
#define Frame_head 						0xA5
//目的地址(从机地址 )
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
//数据长度	(主机：命令字节和下行数据的长度 从机：应答字节和上行数据的长度)
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
//命令字节
#define Inspection_Command 							0x01//巡检命令
#define Into_Emer_Command 							0x02//进入应急广播命令
#define Out_Emer_Command 							0x03//退出应急广播命令
#define Into_MP3_Command 							0x04//进入MP3命令
#define Out_MP3_Command 							0x05//退出MP3命令
#define Into_Line1_Command 							0x06//进入Line1命令
#define Out_Line1_Command 							0x07//退出Line1命令
#define Into_Line2_Command 							0x08//进入Line2命令
#define Out_Line2_Command 							0x09//进入Line2命令
#define Into_PlayBack_Command 						0x0A//进入回放命令
#define Out_PlayBack_Command 						0x0B//退出回放命令
#define Play_Command 								0x0C//播放命令
#define StopPlay_Command 							0x0D//停止命令
#define Forward_Command 							0x0E//快进命令
#define Backward_Command 							0x0F//快退命令
#define UP_Command 									0x10//上一首命令
#define Down_Command 								0x11//下一首命令
#define Single_Command 								0x12//单曲循环命令
#define Sequence_Command 							0x13//顺序循环命令
#define Automatic_Command 							0x14//自动模式命令
#define Manual_Command 								0x15//手动模式命令
#define FileInput_Command 							0x16//文件导入命令
#define StartupPowerAmp_Command 					0x17//启动功放命令
#define StopPowerAmp_Command 						0x18//停动功放命令
#define PlayBackDel_Command 						0x19//录音删除命令
#define Global_Reset_Command 						0x1A//全局复位命令
#define Global_SelfCheck_Command 					0x1B//全局自检命令
#define MicOn_Command 										0x1C//MIC模式 发送给功放的命令
#define Area_Fault_Mian_Command 					0x1E//某区故障 发送给主板的命令
#define Area_Normal_Mian_Command 					0x1F//某区正常 发送给主板的命令
#define DelAllFile_Command 								0x20//删除全部文件命令
#define GetMainInf_Command 							0x29//获取主板信息命令
#define GetFileInputProcess_Command 				0x31//获取文件导入进度命令
#define GetSongSum_Command 							0x32//获取歌曲总数量命令
#define GetPlayBackSum_Command 						0x33//获取回放总数量命令
#define GetSongCurrentNum_Command 	 				0x34//获取当前歌曲排名命令
#define GetPlayBackCurrentNum_Command 				0x35//获取当前回放排名命令
#define GetPlayBackHistoryTime_Command 			    0x36//获取回放历史时间命令
#define GetPlayBackPlayProcessTime_Command 			0x37//获取回放播放进度时间命令
#define GetPlayBackPlayProcessBar_Command 		    0x38//获取回放播放进度条命令
#define GetEmerPlayProcessTime_Command 				0x39//获取应急广播播放进度时间命令
#define GetEmerPlayProcessBar_Command 				0x3A//获取应急广播播放进度条命令
#define Get1To30AreaInf_Command 					0x3D//获取1-30区异常信息命令
#define Get31To60AreaInf_Command 					0x3E//获取31-60区异常信息命令
#define Get61To90AreaInf_Command 					0x3F//获取61-90区异常信息命令
#define Get91To120AreaInf_Command 					0x40//获取91-120区异常信息命令
#define Get121To150AreaInf_Command 					0x41//获取121-150区异常信息命令
#define Get151To180AreaInf_Command 					0x42//获取151-180区异常信息命令
#define Get181To210AreaInf_Command 					0x43//获取181-210区异常信息命令
#define Get211To240AreaInf_Command 					0x44//获取211-240区异常信息命令
#define Get241To270AreaInf_Command 					0x45//获取241-270区异常信息命令

#define StartupArea_Command 						0x60//启动区命令       +下行数据 (N0H(区高位)+N1H(区低位))
#define StopArea_Command 							0x61//停动区命令       +下行数据 (N0H(区高位)+N1H(区低位))

#define CfgSystemTime_Command 						0x70//配置系统时间命令               +下行数据 (N0H(年高位)+N1H(年低位)+N2H(月)+N3H(日)+N4H(时)+N5H(分))
#define CfgNativeAddress_Command 					0x71//配置本机地址命令               +下行数据 (N0H)
#define Cfg1PowerAmp1to60Area_Command 				0x72//配置地址1功放区(1-60)命令 	 +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg1PowerAmp61to120Area_Command 			0x73//配置地址1功放区(61-120)命令    +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg1PowerAmp121to180Area_Command 			0x74//配置地址1功放区(121-180)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg1PowerAmp181to240Area_Command 			0x75//配置地址1功放区(181-240)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg1PowerAmp241to270Area_Command 			0x76//配置地址1功放区(241-270)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg2PowerAmp1to60Area_Command 				0x77//配置地址2功放区(1-60)命令 	 +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg2PowerAmp61to120Area_Command 			0x78//配置地址2功放区(61-120)命令    +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg2PowerAmp121to180Area_Command 			0x79//配置地址2功放区(121-180)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg2PowerAmp181to240Area_Command 			0x7A//配置地址2功放区(181-240)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg2PowerAmp241to270Area_Command 			0x7B//配置地址2功放区(241-270)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg3PowerAmp1to60Area_Command 				0x7C//配置地址3功放区(1-60)命令 	 +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg3PowerAmp61to120Area_Command 			0x7D//配置地址3功放区(61-120)命令    +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg3PowerAmp121to180Area_Command 			0x7E//配置地址3功放区(121-180)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg3PowerAmp181to240Area_Command 			0x7F//配置地址3功放区(181-240)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg3PowerAmp241to270Area_Command 			0x80//配置地址3功放区(241-270)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg4PowerAmp1to60Area_Command 				0x81//配置地址4功放区(1-60)命令 	 +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg4PowerAmp61to120Area_Command 			0x82//配置地址4功放区(61-120)命令    +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg4PowerAmp121to180Area_Command 			0x83//配置地址4功放区(121-180)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg4PowerAmp181to240Area_Command 			0x84//配置地址4功放区(181-240)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg4PowerAmp241to270Area_Command 			0x85//配置地址4功放区(241-270)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg5PowerAmp1to60Area_Command 				0x86//配置地址5功放区(1-60)命令 	 +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg5PowerAmp61to120Area_Command 			0x87//配置地址5功放区(61-120)命令    +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg5PowerAmp121to180Area_Command 			0x88//配置地址5功放区(121-180)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg5PowerAmp181to240Area_Command 			0x89//配置地址5功放区(181-240)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg5PowerAmp241to270Area_Command 			0x8A//配置地址5功放区(241-270)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg6PowerAmp1to60Area_Command 				0x8B//配置地址6功放区(1-60)命令 	 +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg6PowerAmp61to120Area_Command 			0x8C//配置地址6功放区(61-120)命令    +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg6PowerAmp121to180Area_Command 			0x8D//配置地址6功放区(121-180)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg6PowerAmp181to240Area_Command 			0x8E//配置地址6功放区(181-240)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg6PowerAmp241to270Area_Command 			0x8F//配置地址6功放区(241-270)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg7PowerAmp1to60Area_Command 				0x90//配置地址7功放区(1-60)命令 	 +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg7PowerAmp61to120Area_Command 			0x91//配置地址7功放区(61-120)命令    +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg7PowerAmp121to180Area_Command 			0x92//配置地址7功放区(121-180)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg7PowerAmp181to240Area_Command 			0x93//配置地址7功放区(181-240)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg7PowerAmp241to270Area_Command 			0x94//配置地址7功放区(241-270)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg8PowerAmp1to60Area_Command 				0x95//配置地址8功放区(1-60)命令 	 +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg8PowerAmp61to120Area_Command 			0x96//配置地址8功放区(61-120)命令    +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg8PowerAmp121to180Area_Command 			0x97//配置地址8功放区(121-180)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg8PowerAmp181to240Area_Command 			0x98//配置地址8功放区(181-240)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg8PowerAmp241to270Area_Command 			0x99//配置地址8功放区(241-270)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg9PowerAmp1to60Area_Command 				0x9A//配置地址9功放区(1-60)命令 	 +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg9PowerAmp61to120Area_Command 			0x9B//配置地址9功放区(61-120)命令    +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg9PowerAmp121to180Area_Command 			0x9C//配置地址9功放区(121-180)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg9PowerAmp181to240Area_Command 			0x9D//配置地址9功放区(181-240)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg9PowerAmp241to270Area_Command 			0x9E//配置地址9功放区(241-270)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg10PowerAmp1to60Area_Command 				0x9F//配置地址10功放区(1-60)命令 	 +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg10PowerAmp61to120Area_Command 			0xA0//配置地址10功放区(61-120)命令   +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg10PowerAmp121to180Area_Command 			0xA1//配置地址10功放区(121-180)命令  +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg10PowerAmp181to240Area_Command 			0xA2//配置地址10功放区(181-240)命令  +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)
#define Cfg10PowerAmp241to270Area_Command 			0xA3//配置地址10功放区(241-270)命令  +下行数据 (N0H+N1H+N2H+N3H+N4H+N5H+N6H+N7H)

#define GetSongName_Command 						0xC0//获取歌曲名命令

void txandre_Init(void);
void txandre_ClearAllTaskFlag(void);
void txandre_AddSetAddrTask(void);
void txandre_AddPeriodTask(void);
void txandre_AddSelfCheckTask(void);
void txandre_AddResetPowerAmpTask(void);
void txandre_FlashTaskFlag(void);	//清除发送任务命令的全部标志 
void txandre_SendCmdProc(void);		//发送全部的命令
void txandre_RecvAckProc(void);   //接收全部的总处理

void txandre_SendPackage(int nAddr,int nCmd,int nLen,u8 Buf[]);

#endif
