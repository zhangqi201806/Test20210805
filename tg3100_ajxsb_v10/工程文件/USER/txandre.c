#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

#include "main.h"
#include "adjust.h" 
#include "bitmap.h"
#include "flash.h"
#include "init.h"
#include "ui_main.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "ledandkey.h"
#include "sys.h"  
#include "sys485.h"   
#include "txandre.h"  
#include "powamp.h" 
#include "mtx.h" 
#include "mre.h" 
#include "ptx.h" 
#include "pre.h" 
#include "ui_key.h" 
#include "timer.h"  
#include "config.h"
#include "task.h"

u8 CfgNativeAddress;
u8 CfgPowerAmp[400]={0};
u8 Single_Or_Sequence_Flag;
u8 Record_Flag,ForceOn_Flag,DelFile_Flag;
u8 SlaveAdress;//发送巡检时的地址循环用
u8 Task_Flag; //插入命令标志
u8 TX_Task_Command_Flag[400]={0};

u8 Receive_Sucess_Flag;

void txandre_ClearAllTaskFlag(void)
{
	int i;
	
	Task_Flag=0;
	for(i=0;i<400;i++)
		TX_Task_Command_Flag[i]=0;
}

void txandre_Init(void)
{
	int i;
	
	txandre_ClearAllTaskFlag();
	
	for(i=0;i<200;i++)
	{
		USART1_TxBuffer[i]=0;
		USART1_RxBuffer[i]=0;
	}
	
	Timer80msFlag=0;
	Receive_Finsh_Flag=0;
	RxLength=0;
	SlaveAdress=0x00;
	MainInfSta_UOnline_Flag=0;
	
	Area_Startup_Num = 0;
	Area_Fault_Num = 0;
	for(i=0;i<270;i++)
	{
		Area_Startup_Flag[i]=0;//区启动标志
		Area_Fault_Flag[i]=0;//区故障标志
	}

	PowerAmp_Start_Num = 0;
	PowerAmp_Mic_Num = 0;
	PowerAmp_Play_Num = 0;
	PowerAmp_Fault_Num = 0;
	for(i=0;i<10;i++)
	{
		PowerAmp_Start_Flag[i]=0;//功放启动标志
		PowerAmp_Mic_Flag[i]=0;//功放Mic标志
		PowerAmp_Play_Flag[i]=0;//功放启动标志
		PowerAmp_Fault_Flag[i]=0;//功放故障标志
	}
}

void txandre_AddSetAddrTask(void)
{
	CfgNativeAddress=100*Main_Adress_ReadFlash_Buffer[0]+10*Main_Adress_ReadFlash_Buffer[1]+Main_Adress_ReadFlash_Buffer[2];
	
	Task_Flag=1;
	TX_Task_Command_Flag[86]=1;//配置本机地址	
}

void txandre_AddResetPowerAmpTask(void)
{
	txandre_ClearAllTaskFlag();

	//全局复位命令
	Task_Flag=1;
	TX_Task_Command_Flag[87]=1;
}

void txandre_AddSelfCheckTask(void)
{
	txandre_ClearAllTaskFlag();
	//全局自检命令
	Task_Flag=1;
	TX_Task_Command_Flag[88]=1;
}

void txandre_AddGetCurSongInfoTask(void)
{
	Task_Flag=1;	
	TX_Task_Command_Flag[27]=1;//获取歌曲总数	
	TX_Task_Command_Flag[29]=1;//获取当前歌曲排名
	TX_Task_Command_Flag[26]=1;//获取当前歌曲名字
}

void txandre_AddPeriodTask(void)
{
	if(!Timer500msFlag)
		return;
	
	Timer500msFlag = 0;
	Timer500msCounter=0;
	
	if(!PowerAmp_IsReging())
	{
		Task_Flag=1;
		TX_Task_Command_Flag[0]=1;//获取主板状态
	}
	
	if(Record_Flag)
	{
		Task_Flag=1;
		TX_Task_Command_Flag[34]=1;  				//应急广播时的时间显示
	}								
	else if(Emergency_Mode)//应急广播时的进度条和时间显示的获取
	{				
		Task_Flag=1;
		TX_Task_Command_Flag[35]=1;					//应急广播时的进度条	
	}
	else if(MP3_Mode && MainInfSta_UOnline_Flag)
	{
		txandre_AddGetCurSongInfoTask();
	}
	else if(PlayBack_Play_Flag)
	{
		//录音查询时
		if(DEF_RecordQuery==WndGrp[WndNum])
		{
			Task_Flag=1;
			TX_Task_Command_Flag[32]=1;  			//回放的时间显示
			TX_Task_Command_Flag[33]=1;				//回放的进度条						
		}				
	}
	if(DEF_FileInput == WndGrp[WndNum] && !FileInputFinish_Flag)
	{
		Task_Flag=1;
		TX_Task_Command_Flag[25]=1;  					//文件导入进度获取
	}
	
	PowAmp_PeriodTaskProc();
}

void txandre_FlashTaskFlag(void)
{
	int nIndex;
	
	for(nIndex=0;nIndex<400;nIndex++)
	{
		if(TX_Task_Command_Flag[nIndex])
		{
			Task_Flag = 1;
			return;
		}
	}
	Task_Flag = 0;
}

void txandre_SendPackage(int nAddr,int nCmd,int nLen,u8 Buf[]) 
{
	int nIndex,nPos;
	
	TIM_Cmd(TIM3, DISABLE);	
	RS485_SEND;
	nPos = 0;
	
	USART1_TxBuffer[nPos++]=Frame_head;
	USART1_TxBuffer[nPos++]=nAddr;
	USART1_TxBuffer[nPos++]=Data_length_1_Byte+nLen;
	USART1_TxBuffer[nPos++]=nCmd;
	
	for(nIndex=0;nIndex<nLen;nIndex++)
			USART1_TxBuffer[nPos++]=Buf[nIndex];
	
	USART1_TxBuffer[nPos] = 0; 
	for(nIndex=1;nIndex<nPos;nIndex++)
		USART1_TxBuffer[nPos] += USART1_TxBuffer[nIndex];
	nPos++;
	
	while((USART1->SR&0X40)==0);
	for(nIndex=0;nIndex<nPos;nIndex++)
	{
		USART1->DR=USART1_TxBuffer[nIndex];
		while((USART1->SR&0X40)==0);//等待发送结束
	}
	
	if(nAddr == MB_ADDR)
	{
		//if(nCmd != Into_MP3_Command && nCmd != Play_Command && nCmd != GetMainInf_Command && nCmd != GetSongName_Command && nCmd != GetSongCurrentNum_Command)
		//	sys485_SetReSendTimer(8);	
		//else
			sys485_SetReSendTimer(50);	
	}
	else if(nAddr == 0xFF)
		sys485_SetReSendTimer(8);
	else
		sys485_SetReSendTimer(3);
	
	RS485_RECE;
	TIM_Cmd(TIM3, ENABLE);
}

void Send_Inspection_Command(void)
{
	txandre_SendPackage(SlaveAdress,Inspection_Command,0,0);
}

void Send_Reset_Command(void)
{
	txandre_SendPackage(0xFF,Global_Reset_Command,0,0);
}

void Send_SelfCheck_Command(void)
{
	txandre_SendPackage(0xFF,Global_SelfCheck_Command,0,0);
}

void Send_Task_Command(void)
{
	if(TX_Task_Command_Flag[0]==1)
	{
		TX_Task_Command_Flag[0]=0;				
		mtx_GetMainInf_Command_Function();		//获取主板信息命令
	}
	else if(TX_Task_Command_Flag[3]==1)
	{
		TX_Task_Command_Flag[3]=0;
		mtx_Into_Emer_Command_Function();       //进入应急广播
	}
	else if(TX_Task_Command_Flag[4]==1)
	{
		TX_Task_Command_Flag[4]=0;
		mtx_Out_Emer_Command_Function();        //退出应急广播
	}
	else if(TX_Task_Command_Flag[5]==1)
	{
		TX_Task_Command_Flag[5]=0;
		mtx_Into_MP3_Command_Function();		   //进入MP3
	}
	else if(TX_Task_Command_Flag[6]==1)
	{
		TX_Task_Command_Flag[6]=0;
		mtx_Out_MP3_Command_Function();         //退出MP3
	}
	else if(TX_Task_Command_Flag[7]==1)
	{
		TX_Task_Command_Flag[7]=0;
		mtx_Into_Line1_Command_Function();     //进入Line1
	}
	else if(TX_Task_Command_Flag[8]==1)
	{
		TX_Task_Command_Flag[8]=0;
		mtx_Out_Line1_Command_Function();      //退出Line1
	}
	else if(TX_Task_Command_Flag[9]==1)
	{
		TX_Task_Command_Flag[9]=0;
		mtx_Into_Line2_Command_Function();     //进入Line2
	}
	else if(TX_Task_Command_Flag[10]==1)
	{
		TX_Task_Command_Flag[10]=0;
		mtx_Out_Line2_Command_Function();	  //退出Line2
	}
	else if(TX_Task_Command_Flag[11]==1)
	{
		TX_Task_Command_Flag[11]=0;
		mtx_Into_Playback_Command_Function();  //进入回放
	}
	else if(TX_Task_Command_Flag[12]==1)
	{
		TX_Task_Command_Flag[12]=0;
		mtx_Out_Playback_Command_Function();   //退出回放
	}
	else if(TX_Task_Command_Flag[13]==1)
	{
		TX_Task_Command_Flag[13]=0;
		mtx_Play_Command_Function();           //主板播放
	}
	else if(TX_Task_Command_Flag[14]==1)
	{
		TX_Task_Command_Flag[14]=0;
		mtx_StopPlay_Command_Function();       //主板停止播放
	}
	else if(TX_Task_Command_Flag[15]==1)
	{
		TX_Task_Command_Flag[15]=0;
		mtx_Forward_Command_Function();        //快进
	}
	else if(TX_Task_Command_Flag[16]==1)
	{
		TX_Task_Command_Flag[16]=0;
		mtx_Backward_Command_Function();       //快退
	}
	else if(TX_Task_Command_Flag[17]==1)
	{
		TX_Task_Command_Flag[17]=0;
		mtx_Up_Command_Function();             //上一曲                           //缺了18
	}
	else if(TX_Task_Command_Flag[18]==1)
	{
		TX_Task_Command_Flag[18]=0;
		mtx_PlayBackDel_Command_Function();  //回放删除命令                       
	}
	else if(TX_Task_Command_Flag[19]==1)
	{
		TX_Task_Command_Flag[19]=0;
		mtx_Down_Command_Function();			  //下一曲
	}
	else if(TX_Task_Command_Flag[20]==1)
	{
		TX_Task_Command_Flag[20]=0;
		mtx_Single_Command_Function();         //单曲循环
	}
	else if(TX_Task_Command_Flag[21]==1)
	{
		TX_Task_Command_Flag[21]=0;         
 		mtx_Sequence_Command_Function();        //顺序循环		
	}
	else if(TX_Task_Command_Flag[22]==1)
	{
		TX_Task_Command_Flag[22]=0;
		mtx_Automatic_Command_Function();       //自动
	}
	else if(TX_Task_Command_Flag[23]==1)
	{
		TX_Task_Command_Flag[23]=0;
		mtx_Manual_Command_Function();          //手动
	}
	else if(TX_Task_Command_Flag[24]==1)
	{
		TX_Task_Command_Flag[24]=0;
		mtx_FileInput_Command_Function();       //文件导入
	}
	else if(TX_Task_Command_Flag[25]==1)
	{
		TX_Task_Command_Flag[25]=0;
		mtx_GetFileInputProcess_Command_Function();//获取文件导入进度
	}
	else if(TX_Task_Command_Flag[26]==1)
	{
		TX_Task_Command_Flag[26]=0;	
		mtx_GetSongName_Command_Function();		//获取歌曲名字
	}
	else if(TX_Task_Command_Flag[27]==1)
	{
		TX_Task_Command_Flag[27]=0;
		mtx_GetSongSum_Command_Function();      //获取歌曲总数
	}
	else if(TX_Task_Command_Flag[28]==1)
	{
		TX_Task_Command_Flag[28]=0;
		mtx_GetPlayBackSum_Command_Function();  //获取回放总数
	}
	else if(TX_Task_Command_Flag[29]==1)
	{
		TX_Task_Command_Flag[29]=0;
		mtx_GetSongCurrentNum_Command_Function();//获取当前歌曲排名
	}
	else if(TX_Task_Command_Flag[30]==1)
	{
		TX_Task_Command_Flag[30]=0;
		mtx_GetPlayBackCurrentNum_Command_Function();//获取当前回放排名
	}
	else if(TX_Task_Command_Flag[31]==1)
	{
		TX_Task_Command_Flag[31]=0;
		mtx_GetPlayBackHistoryTime_Command_Function();//获取回放历史时间
	}
	else if(TX_Task_Command_Flag[32]==1)
	{
		TX_Task_Command_Flag[32]=0;
		mtx_GetPlayBackPlayProcessTime_Command_Function();//获取回放播放进度时间
	}
	else if(TX_Task_Command_Flag[33]==1)
	{
		TX_Task_Command_Flag[33]=0;
		mtx_GetPlayBackPlayProcessBar_Command_Function();//获取回放播放进度条
	}
	else if(TX_Task_Command_Flag[34]==1)
	{
		TX_Task_Command_Flag[34]=0;
		mtx_GetEmerPlayProcessTime_Command_Function();//获取应急广播播放进度时间
	}
	else if(TX_Task_Command_Flag[35]==1)
	{
		TX_Task_Command_Flag[35]=0;
		mtx_GetEmerPlayProcessBar_Command_Function();//获取应急广播播放进度条
	}
	else if(TX_Task_Command_Flag[84]==1)
	{
		TX_Task_Command_Flag[84]=0;
		mtx_DelAllFile_Command();    //删除全部文件
	}
	else if(TX_Task_Command_Flag[85]==1)
	{
		TX_Task_Command_Flag[85]=0;
		mtx_CfgSystemTime_Command();    //配置系统时间
	}
	else if(TX_Task_Command_Flag[86]==1)
	{
		TX_Task_Command_Flag[86]=0;
		mtx_CfgNativeAddress_Command();  //配置本机地址
	}
	else if(TX_Task_Command_Flag[87])//全局复位(主板，功放(1-10))
	{
		TX_Task_Command_Flag[87]=0;
		Send_Reset_Command();
	}
	else if(TX_Task_Command_Flag[88]) //全局自检命令,主板和功放(1-10)
	{
		TX_Task_Command_Flag[88]=0;
		Send_SelfCheck_Command();
	}
	else if(TX_Task_Command_Flag[89]) //功放全局启动命令
	{
		TX_Task_Command_Flag[89]=0;
		ptx_Send_Start_Command();
	}
	else if(TX_Task_Command_Flag[90]) //功放全局MicOn命令
	{
		TX_Task_Command_Flag[90]=0;
		ptx_Send_MicOn_Command();
	}
	else if(TX_Task_Command_Flag[91]) //功放全局停动命令
	{
		TX_Task_Command_Flag[91]=0;
		ptx_Send_AllStop_Command();
	}
	else 
	{
		//获取区(1-270,30区一组)异常信息
		int nIndex;
		for(nIndex=36;nIndex<=44;nIndex++)
		{
			if(TX_Task_Command_Flag[nIndex])
			{
				TX_Task_Command_Flag[nIndex]=0;
				mtx_Send_Get30AreaInf_Command(nIndex-35);
				return;
			}
		}

		//功放(1-10)播放
		for(nIndex=45;nIndex<=54;nIndex++)
		{
			if(TX_Task_Command_Flag[nIndex])
			{
				TX_Task_Command_Flag[nIndex]=0;
				ptx_Send_Play_Command(nIndex-44);
				return;
			}
		}
		
		//功放(1-10)停动
		for(nIndex=55;nIndex<=64;nIndex++)
		{
			if(TX_Task_Command_Flag[nIndex])
			{
				TX_Task_Command_Flag[nIndex]=0;
				ptx_Send_Stop_Command(nIndex-54);
				return;
			}
		}
		
		//区(1-30)启动和停止命令
		for(nIndex=151;nIndex<=210;nIndex++)
		{
			if(TX_Task_Command_Flag[nIndex])
			{
				TX_Task_Command_Flag[nIndex]=0;
				mtx_Send_StartupArea_Command(((nIndex-151)/2)+1,nIndex%2);
				return;
			}
		}
	}
}

u8 bSend_Command;
void txandre_SendCmdProc(void)
{			
	if(!Timer80msFlag)
		return;

	Timer80msFlag=0;
		
	if(Task_Flag)
	{		
		bSend_Command = 1;
		Send_Task_Command();							
	}	
	else    
	{
		bSend_Command = 2;
		Send_Inspection_Command();
		
		SlaveAdress++;	
		if(SlaveAdress==12)
		{			
			SlaveAdress=0;
			PowerAmp_OnRegCompleted();
		}	
	}	
}

//接收  主板和功放板 的 巡检
void Inspection_Handle(void)
{
	if(USART1_RxBuffer[1] == MB_ADDR)//主板
		mre_Recv_Inspection_Ack(USART1_RxBuffer[4]);
	else    								//功放板
		pre_Recv_Inspection_Ack(USART1_RxBuffer[1],USART1_RxBuffer[4]);		
}

//接收 主板和功放板的  播放 
void Recive_Handle_Play_Function(void)
{
	if(USART1_RxBuffer[1]==0x00)
	{
		if(MP3_Mode==1&&DEF_PATROLWND==WndGrp[WndNum]&&MainInfSta_UOnline_Flag==1)
		{
			Recive_Handle_SongPlay_Function();
		}
		if(Playback_Mode==1)
		{
			Recive_Handle_PlayBackPlay_Function();
		}
		if(line1_Mode==1&&DEF_PATROLWND==WndGrp[WndNum])
		{
			Recive_Handle_Line1Play_Function();
		}
		if(line2_Mode==1&&DEF_PATROLWND==WndGrp[WndNum])
		{
			Recive_Handle_Line2Play_Function();
		}
	}
}
//接收 主板和功放板的  停止播放
void Recive_Handle_StopPlay_Function(void)
{
	if(USART1_RxBuffer[1]==0x00)
	{
		if(MP3_Mode==1&&DEF_PATROLWND==WndGrp[WndNum]&&MainInfSta_UOnline_Flag==1)
		{
			Recive_Handle_SongStopPlay_Function();
		}
		if(Playback_Mode==1)
		{
			Recive_Handle_PlayBackStopPlay_Function();
		}
		if(line1_Mode==1&&DEF_PATROLWND==WndGrp[WndNum])
		{
			Recive_Handle_Line1StopPlay_Function();
		}
		if(line2_Mode==1&&DEF_PATROLWND==WndGrp[WndNum])
		{
			Recive_Handle_Line2StopPlay_Function();
		}
	}
}

//接收 主板和功放板的  播放 和停止播放
void Play_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) 	Recive_Handle_Play_Function();
}
void StopPlay_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_StopPlay_Function();
}

void txandre_RecvAckProc(void) //接收数据的处理
{
	u8 checksum=0,i=1;	
	if(Receive_Finsh_Flag==1)
	{		
		if(USART1_RxBuffer[0]==0xA5)  										//第一判断起始符对不对
		{		
			if(USART1_TxBuffer[1]==USART1_RxBuffer[1])						//第二判断 发的地址和接收的地址 是不是一个
			{
				if(USART1_TxBuffer[3]==USART1_RxBuffer[3])					//第三判断 发的命令和接收的命令 是不是一个
				{
					for(i=1;i<USART1_RxBuffer[2]+3;i++)						//计算校验和
					{
						checksum=checksum+USART1_RxBuffer[i];
					}
					if(USART1_RxBuffer[USART1_RxBuffer[2]+3]==checksum)		//第四判断  校验和对不对
					{		
						Receive_Sucess_Flag=1;
						
						if(USART1_RxBuffer[1] && USART1_RxBuffer[1]<11)
								PowerAmp_Online(USART1_RxBuffer[1]);
						
						switch(USART1_RxBuffer[3])//解析命令
						{
							case Inspection_Command:									Inspection_Handle();								break;
							case Into_Emer_Command:  									Into_Emer_Handle();									break;
							case Out_Emer_Command: 	 									Out_Emer_Handle();									break;				
							case Into_MP3_Command: 	 									Into_MP3_Handle();									break;				
							case Out_MP3_Command: 	 									Out_MP3_Handle();   								break;				
							case Into_Line1_Command: 									Into_Line1_Handle();								break; 				
							case Out_Line1_Command:  									Out_Line1_Handle();									break;				
							case Into_Line2_Command:									Into_Line2_Handle();								break;				
							case Out_Line2_Command:    								Out_Line2_Handle(); 								break;				
							case Into_PlayBack_Command: 							Into_PlayBack_Handle();							break;
							case Out_PlayBack_Command: 								Out_PlayBack_Handle();							break;
							case Play_Command: 												Play_Handle();											break;  
							case StopPlay_Command: 										StopPlay_Handle();									break;  
							case Forward_Command: 										Forward_Handle();										break;
							case Backward_Command: 										Backward_Handle();									break;
							case UP_Command: 													UP_Handle();												break;
							case Down_Command: 												Down_Handle();											break;	
							case Single_Command: 											Single_Handle();										break;
							case Sequence_Command: 										Sequence_Handle();									break;
							case Automatic_Command: 									Automatic_Handle();									break;
							case Manual_Command: 											Manual_Handle();										break;
//							case FileInput_Command: 									FileInput_Handle();									break;
//							case StartupPowerAmp_Command: 						StartupPowerAmp_Handle();						break;
//							case StopPowerAmp_Command: 								StopPowerAmp_Handle();							break;						
							case GetFileInputProcess_Command: 				GetFileInputProcess_Handle();				break;							
							case GetSongSum_Command:                	GetSongSum_Handle();								break;						
							case GetPlayBackSum_Command: 							GetPlayBackSum_Handle();						break;
							case GetSongCurrentNum_Command:						GetSongCurrentNum_Handle();					break;	
							case GetSongName_Command:	            		GetSongName_Handle();								break;							
							case GetPlayBackCurrentNum_Command: 	    GetPlayBackCurrentNum_Handle(); 		break;
							case GetPlayBackHistoryTime_Command: 			GetPlayBackHistoryTime_Handle();		break;
							case GetPlayBackPlayProcessTime_Command: 	GetPlayBackPlayProcessTime_Handle();break;
							case GetPlayBackPlayProcessBar_Command: 	GetPlayBackPlayProcessBar_Handle();	break;
							case GetEmerPlayProcessTime_Command: 			GetEmerPlayProcessTime_Handle();		break;
							case GetEmerPlayProcessBar_Command: 			GetEmerPlayProcessBar_Handle();			break;
							case PlayBackDel_Command: 								PlayBackDel_Handle();								break;
							case DelAllFile_Command: 									DelAllFile_Handle();								break;
							case GetMainInf_Command:									GetMainInf_Handle();      					break;
							case Get1To30AreaInf_Command: 						Get30AreaInf_Handle(0);						break;    
							case Get31To60AreaInf_Command: 						Get30AreaInf_Handle(1);					break;
							case Get61To90AreaInf_Command: 						Get30AreaInf_Handle(2);					break;
							case Get91To120AreaInf_Command: 					Get30AreaInf_Handle(3);					break;
							case Get121To150AreaInf_Command: 					Get30AreaInf_Handle(4);				break;
							case Get151To180AreaInf_Command: 					Get30AreaInf_Handle(5);				break;
							case Get181To210AreaInf_Command: 					Get30AreaInf_Handle(6);				break;
							case Get211To240AreaInf_Command: 					Get30AreaInf_Handle(7);				break;
							case Get241To270AreaInf_Command: 					Get30AreaInf_Handle(8);				break;												
							case StartupArea_Command: 								StartupArea_Handle();								break;
							case StopArea_Command: 										StopArea_Handle();									break;							
//							case CfgSystemTime_Command: 							CfgSystemTime_Handle();							break;
//							case CfgNativeAddress_Command: 						CfgNativeAddress_Handle();					break;
							case Cfg1PowerAmp1to60Area_Command:				break;
							case Cfg1PowerAmp61to120Area_Command: 		break;
							case Cfg1PowerAmp121to180Area_Command: 		break;
							case Cfg1PowerAmp181to240Area_Command:		break;
							case Cfg1PowerAmp241to270Area_Command: 		break;							
						}				
					}	
				}
			}
		}
		Receive_Finsh_Flag=0;
	}
}



