#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_usart.h"

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
#include "mre.h" 
#include "pre.h"
#include "ui_key.h"
#include "ui_poweramp.h"
#include "task.h"
#include "config.h"

//区相关变量 数组
u8 Area_Startup_Flag[270]={0};
u8 Area_Fault_Flag[270]={0};
u16 Area_Startup_Num,Area_Fault_Num;
u8 AreaFault_BuzzerOn_Flag;


//控制器区启动，要求切换进应急广播模式
static int bSwitchToEmeMode = 0;

//主板巡检状态
u8 MainBoard_Exist_Flag;

//当前模式
u8 Emergency_Mode,MP3_Mode,line1_Mode,line2_Mode,Playback_Mode;

//U盘，MIC在线状态
u8 MainInfSta_UOnline_Flag,MainInfSta_MicOnline_Flag;

//录音
u8 Record_Current_Time_Min,Record_Current_Time_Sec;

//应急广播
u8 Emergency_Current_Time_Min,Emergency_Current_Time_Sec;
u8 EmerPlayProcessBar_Percent;

//MP3
u16 MP3_Current_Num,MP3_Sum;

//回放相关变量
u16 PlayBack_Current_Num,PlayBack_Current_Sum=40;
u8 PlayBack_Current_Time_Min,PlayBack_Current_Time_Sec;
u8 PlaybackHistoryYear,PlaybackHistoryMon,PlaybackHistoryDay,PlaybackHistoryHou,PlaybackHistoryMin,PlaybackHistorySec;
u8 PlayBackPlayProcessBar_Percent;


void mre_AddStartAllAreaTask(int bStart)
{
	int nArea,nLedIndex,nAreaStartCmdIndex;

	for(nArea=0;nArea<270;nArea++)
	{
		if(!config_IsAreaCfgedAny(nArea))
			continue;
		
		if(Area_Fault_Flag[nArea])
			continue;
		
		if(Area_Startup_Flag[nArea] == bStart)
			continue;
		
		nLedIndex = (nArea/6)*12+nArea%6+6;
		nAreaStartCmdIndex = 151 + nArea*2;
		
		led_FlashLed(nLedIndex);
		
		Task_Flag=1;
		if(bStart)
			TX_Task_Command_Flag[nAreaStartCmdIndex]=1; //启动区 命令	
		else
			TX_Task_Command_Flag[nAreaStartCmdIndex+1]=1; //停动区 命令
	}
}

void mre_AddGetAreaInfoTask(void)
{
	Task_Flag=1;
	TX_Task_Command_Flag[36]=1;//获取1-30异常信息 启动和故障的
	TX_Task_Command_Flag[37]=1;//获取31-60异常信息 启动和故障的
	TX_Task_Command_Flag[38]=1;//获取61-90异常信息 启动和故障的
	TX_Task_Command_Flag[39]=1;//获取91-120异常信息 启动和故障的
	TX_Task_Command_Flag[40]=1;//获取121-150异常信息 启动和故障的
	TX_Task_Command_Flag[41]=1;//获取151-180异常信息 启动和故障的
	TX_Task_Command_Flag[42]=1;//获取181-210异常信息 启动和故障的
	TX_Task_Command_Flag[43]=1;//获取211-240异常信息 启动和故障的
	TX_Task_Command_Flag[44]=1;//获取241-270异常信息 启动和故障的
}

//5种模式进入和退出的处理函数
void Recive_Handle_Into_Emer_Function(void)
{	
	led_LightLed(0);
}

void Recive_Handle_Out_Emer_Function(void)
{
	led_CloseLed(0);
}

void Recive_Handle_Into_MP3_Function(void)
{
	led_LightLed(1);
}

void Recive_Handle_Out_MP3_Function(void)
{
	led_CloseLed(1);
}

void Recive_Handle_Into_Line1_Function(void)
{
	led_LightLed(2);
}

void Recive_Handle_Out_Line1_Function(void)
{
	led_CloseLed(2);
}

void Recive_Handle_Into_Line2_Function(void)
{
	led_LightLed(3);
}
void Recive_Handle_Out_Line2_Function(void)
{
	led_CloseLed(3);
}
void Recive_Handle_Into_PlayBack_Function(void)
{	
	Playback_Mode=1;
	
	PlayBack_Play_Flag=0;

	if(DEF_RecordQuery == WndGrp[WndNum])//录音查询界面
	{
		Task_Flag=1;
		TX_Task_Command_Flag[30]=1;//获取当前回放排名
		TX_Task_Command_Flag[28]=1;//获取回放总数  
		Bar_Proceess_Init_Interface(4);
	}
	else if(DEF_RecordDel == WndGrp[WndNum])//录音删除界面
	{
		Task_Flag=1;
		TX_Task_Command_Flag[30]=1;//获取当前回放排名
		TX_Task_Command_Flag[28]=1;//获取回放总数   
	}
}

void Recive_Handle_Out_PlayBack_Function(void)
{
	Playback_Mode=0;
	
	PlayBack_Play_Flag=0;
}

//MP3和回放的  上一曲 下一曲
void Recive_Handle_SongUP_Function(void)
{
}

void Recive_Handle_SongDown_Function(void)
{
}

void Recive_Handle_PlayBackUP_Function(void)
{
	if(DEF_RecordDel==WndGrp[WndNum])//录音删除界面		
	{
		if(PlayBack_Current_Num!=0&&PlayBack_Current_Sum!=0)
		{
			PlayBack_Current_Num--;
			if(PlayBack_Current_Num==0)
			PlayBack_Current_Num=PlayBack_Current_Sum;
			PlayBack_Del_Current_Num_Display();
			Task_Flag=1;
			TX_Task_Command_Flag[31]=1;//发送 获取历史时间命令
		}		
	}
	if(DEF_RecordQuery==WndGrp[WndNum])//录音查询界面
	{
		PlayBack_Current_Num--;
		if(PlayBack_Current_Num==0)
		PlayBack_Current_Num=PlayBack_Current_Sum;
		PlayBack_Current_Num_Display();
		Task_Flag=1;
		TX_Task_Command_Flag[31]=1;//发送 获取历史时间命令
		LCD_WriteLine(4,0x00);	//清除显示的进度条
		LCD_WriteLine(5,0x00);
		Bar_Proceess_Init_Interface(4);
		Bar_Process_Count=0;//进度条显示时计数用的变?
		PlayBack_Current_Time_Min=0;///清零 回放播放进度时间
		PlayBack_Current_Time_Sec=0;
	}
}

void Recive_Handle_PlayBackDown_Function(void)
{
	if(DEF_RecordDel==WndGrp[WndNum])//录音删除界面		
	{
		if(PlayBack_Current_Num!=0&&PlayBack_Current_Sum!=0)
		{
			PlayBack_Current_Num++;
			if(PlayBack_Current_Num>PlayBack_Current_Sum)
			PlayBack_Current_Num=1;
			PlayBack_Del_Current_Num_Display();
			Task_Flag=1;
			TX_Task_Command_Flag[31]=1;	//发送 获取历史时间命令
		}	
	}
	if(DEF_RecordQuery==WndGrp[WndNum])//录音查询界面
	{
		PlayBack_Current_Num++;
		if(PlayBack_Current_Num>PlayBack_Current_Sum)
		PlayBack_Current_Num=1;
		PlayBack_Current_Num_Display();	
		Task_Flag=1;
		TX_Task_Command_Flag[31]=1;	//发送 获取历史时间命令
		LCD_WriteLine(4,0x00);	//清除显示的进度条
		LCD_WriteLine(5,0x00);
		Bar_Proceess_Init_Interface(4);
		Bar_Process_Count=0;
		PlayBack_Current_Time_Min=0;///清零 回放播放进度时间
		PlayBack_Current_Time_Sec=0;
	}
}

void Recive_Handle_UP_Function(void)
{
	if(MP3_Mode==1&&DEF_PATROLWND==WndGrp[WndNum]&&MainInfSta_UOnline_Flag==1)	
	{															
		Recive_Handle_SongUP_Function();
	}
	if(Playback_Mode==1)
	{
		Recive_Handle_PlayBackUP_Function();
	}
}
void Recive_Handle_Down_Function(void)
{
	if(MP3_Mode==1&&DEF_PATROLWND==WndGrp[WndNum]&&MainInfSta_UOnline_Flag==1)	
	{
		Recive_Handle_SongDown_Function();
	}
	if(Playback_Mode==1)
	{
		Recive_Handle_PlayBackDown_Function();
	}
}

//MP3和回放的  快进快退
void Recive_Handle_SongForward_Function(void)
{

}
void Recive_Handle_SongBackward_Function(void)
{

}
void Recive_Handle_PlayBackForward_Function(void)
{

}
void Recive_Handle_PlayBackward_Function(void)
{

}

void Recive_Handle_Forward_Function(void)
{
	if(MP3_Mode==1&&DEF_PATROLWND==WndGrp[WndNum]&&MainInfSta_UOnline_Flag==1)
	{
		Recive_Handle_SongForward_Function();
	}
	if(Playback_Mode==1)
	{
		Recive_Handle_PlayBackForward_Function();
	}
}
void Recive_Handle_Backward_Function(void)
{
	if(MP3_Mode==1&&DEF_PATROLWND==WndGrp[WndNum]&&MainInfSta_UOnline_Flag==1)
	{
		Recive_Handle_SongBackward_Function();
	}
	if(Playback_Mode==1)
	{
		Recive_Handle_PlayBackward_Function();
	}
}
//MP3 LINE1 LINE2 回放的播放和停止函数
void Recive_Handle_SongPlay_Function(void)
{
}
void Recive_Handle_SongStopPlay_Function(void)
{
}
void Recive_Handle_Line1Play_Function(void)
{
}
void Recive_Handle_Line1StopPlay_Function(void)
{
}
void Recive_Handle_Line2Play_Function(void)
{
}
void Recive_Handle_Line2StopPlay_Function(void)
{
}
void Recive_Handle_PlayBackPlay_Function(void)
{
	PlayBack_Play_Flag=1;
	
	LCD_WriteLine(4,0x00);	//清除显示的进度条
	LCD_WriteLine(5,0x00);
	
	Bar_Proceess_Init_Interface(4);
	Bar_Process_Count=0;
	
	PlayBack_Current_Time_Min=0;///清零 回放播放进度时间
	PlayBack_Current_Time_Sec=0;
}

void Recive_Handle_PlayBackStopPlay_Function(void)
{
	PlayBack_Play_Flag=0;
	
	LCD_WriteLine(4,0x00);	//清除显示的进度条
	LCD_WriteLine(5,0x00);
	
	Bar_Proceess_Init_Interface(4);//初始化 进度条显示
	Bar_Process_Count=0;
	
	PlayBack_Current_Time_Min=0;///清零 回放播放进度时间
	PlayBack_Current_Time_Sec=0;
}

///MP3 回放 应急广播显示的相关命令回复
//MP3
void Recive_Handle_SongCurrentNum_Function(void)
{
	if(MP3_Mode && MainInfSta_UOnline_Flag && !Record_Flag)
	{
		MP3_Current_Num=USART1_RxBuffer[5]*256+USART1_RxBuffer[6];;
		MP3_Song_Current_Num_Display();
	}	
}
void Recive_Handle_SongCurrentSum_Function(void)
{
	if(MP3_Mode && MainInfSta_UOnline_Flag && !Record_Flag)
	{
		MP3_Sum=USART1_RxBuffer[5]*256+USART1_RxBuffer[6];;
		MP3_Song_Sum_Num_Display();
	}
}

void Recive_Handle_SongName_Function(void)
{
	if(MP3_Mode && MainInfSta_UOnline_Flag && !Record_Flag)
	{
		u8 nIndex;
		
		if(!(USART1_RxBuffer[2]-2))
			return;

		Clear_Location_Code();
		
		nIndex=0;
		while(nIndex<USART1_RxBuffer[2]-2)
		{
			if(USART1_RxBuffer[nIndex+5] < 0x80)
			{
				GBK_to_hex_Asiic(USART1_RxBuffer[nIndex+5],nIndex);			
				nIndex++;
			}
			else
			{		
				GBK_to_hex_quwei(USART1_RxBuffer[nIndex+5],USART1_RxBuffer[nIndex+6],nIndex);
				nIndex+=2;
			}
		}
		
		Clear_16_Location_Code();
		
		MP3_Song_Name_Display();///第三行 显示歌名 只能显示前8个汉字			
	}
}
//回放
void Recive_Handle_PlayBackCurrentNum_Function(void)
{
	if(Record_Flag)
			return;
	
	if(DEF_RecordQuery==WndGrp[WndNum])//录音查询界面  
	{										
		PlayBack_Current_Num=USART1_RxBuffer[5]*256+USART1_RxBuffer[6];
		PlayBack_Current_Num_Display();
		if(PlayBack_Current_Num>0)
		{
			Task_Flag=1;
			TX_Task_Command_Flag[31]=1;//获取历史时间命令
		}		
	}	
	if(DEF_RecordDel==WndGrp[WndNum])//录音删除界面  
	{
		PlayBack_Current_Num=USART1_RxBuffer[5]*256+USART1_RxBuffer[6];
		PlayBack_Del_Current_Num_Display();
		if(PlayBack_Current_Num>0)
		{
			Task_Flag=1;
			TX_Task_Command_Flag[31]=1;//获取历史时间命令
		}		
	}
}
void Recive_Handle_PlayBackSum_Function(void)
{
	if(Record_Flag)
			return;

	if(DEF_RecordQuery==WndGrp[WndNum])//录音查询界面
	{										
		PlayBack_Current_Sum=USART1_RxBuffer[5]*256+USART1_RxBuffer[6];
		PlayBack_Current_Sum_Display();
	}	
	if(DEF_RecordDel==WndGrp[WndNum])//录音删除界面  
	{
		PlayBack_Current_Sum=USART1_RxBuffer[5]*256+USART1_RxBuffer[6];
		PlayBack_Del_Sum_Display();
	}
}
void Recive_Handle_PlayBackHistoryTime_Function(void)
{
	if(Record_Flag)
			return;
	
	PlaybackHistoryYear=USART1_RxBuffer[5];//年
	PlaybackHistoryMon=USART1_RxBuffer[6];//月
	PlaybackHistoryDay=USART1_RxBuffer[7];//日
	PlaybackHistoryHou=USART1_RxBuffer[8];//时
	PlaybackHistoryMin=USART1_RxBuffer[9];//分
	PlaybackHistorySec=USART1_RxBuffer[10];//秒	
	PlayBack_History_Time_Dispaly();		
}
void Recive_Handle_PlayBackPlayProcessTime_Function(void)
{
	if(Record_Flag)
			return;
	
	PlayBack_Current_Time_Min=USART1_RxBuffer[5];
	PlayBack_Current_Time_Sec=USART1_RxBuffer[6];
	Playback_Current_Time_Display();//送给显示											
}
void Recive_Handle_PlayBackPlayProcessBar_Function(void)
{
	if(Record_Flag)
			return;
	
	PlayBackPlayProcessBar_Percent=USART1_RxBuffer[5];
	switch(PlayBackPlayProcessBar_Percent/5)
	{
		case 1:		Bar_Process_Count=1; Bar_Change(0,4);	break;
		case 2:		Bar_Process_Count=2; Bar_Change(0,4);	break;
		case 3:		Bar_Process_Count=3; Bar_Change(0,4);	break;
		case 4:		Bar_Process_Count=4; Bar_Change(0,4);	break;
		case 5:		Bar_Process_Count=5; Bar_Change(0,4);	break;
		case 6:		Bar_Process_Count=6; Bar_Change(0,4);	break;
		case 7:		Bar_Process_Count=7; Bar_Change(0,4);	break;
		case 8:		Bar_Process_Count=8; Bar_Change(0,4);	break;
		case 9:		Bar_Process_Count=9; Bar_Change(0,4);	break;
		case 10:	Bar_Process_Count=10;Bar_Change(0,4);	break;
		case 11:	Bar_Process_Count=11;Bar_Change(0,4);	break;
		case 12:	Bar_Process_Count=12;Bar_Change(0,4);	break;
		case 13:	Bar_Process_Count=13;Bar_Change(0,4);	break;
		case 14:	Bar_Process_Count=14;Bar_Change(0,4);	break;
		case 15:	Bar_Process_Count=15;Bar_Change(0,4);	break;
		case 16:	Bar_Process_Count=16;Bar_Change(0,4);	break;
		case 17:	Bar_Process_Count=17;Bar_Change(0,4);	break;;
		case 18:	Bar_Process_Count=18;Bar_Change(0,4);	break;
		case 19:	Bar_Process_Count=19;Bar_Change(0,4);	break;
		case 20:	Bar_Process_Count=20;Bar_Change(0,4);	break;
	}
}
void Recive_Handle_PlayBackDel_Function(void)
{
	if(Record_Flag)
			return;
	
	if(PlayBack_Current_Num && PlayBack_Current_Sum)
	{
		PlayBack_Current_Sum--;
		if(PlayBack_Current_Num>PlayBack_Current_Sum)
				PlayBack_Current_Num=PlayBack_Current_Sum;
		
		Task_Flag=1;
		TX_Task_Command_Flag[31]=1;//获取历史时间命令
	}			
	PlayBack_Del_Current_Num_Display();
	PlayBack_Del_Sum_Display();
}

void Recive_Handle_DelAllFile_Function(void)
{
	DelAllFile_Flag=1;
}

//应急广播
void Recive_Handle_EmerPlayProcessTime_Function(void)
{
	if(Record_Flag)
	{
		Record_Current_Time_Min=USART1_RxBuffer[5];
		Record_Current_Time_Sec=USART1_RxBuffer[6];	
		Record_Current_Time_Display();//送给显示
	}	
}
void Recive_Handle_EmerPlayProcessBar_Function(void)
{
	if(Record_Flag) return;
	if(!Emergency_Mode) return;
	
	Bar_Proceess_Init_Interface(3);
	EmerPlayProcessBar_Percent=USART1_RxBuffer[5];
	switch(EmerPlayProcessBar_Percent/5)
	{
		case 1:		Bar_Process_Count=1; Bar_Change(0,3);	break;
		case 2:		Bar_Process_Count=2; Bar_Change(0,3);	break;
		case 3:		Bar_Process_Count=3; Bar_Change(0,3);	break;
		case 4:		Bar_Process_Count=4; Bar_Change(0,3);	break;
		case 5:		Bar_Process_Count=5; Bar_Change(0,3);	break;
		case 6:		Bar_Process_Count=6; Bar_Change(0,3);	break;
		case 7:		Bar_Process_Count=7; Bar_Change(0,3);	break;
		case 8:		Bar_Process_Count=8; Bar_Change(0,3);	break;
		case 9:		Bar_Process_Count=9; Bar_Change(0,3);	break;
		case 10:	Bar_Process_Count=10;Bar_Change(0,3);	break;
		case 11:	Bar_Process_Count=11;Bar_Change(0,3);	break;
		case 12:	Bar_Process_Count=12;Bar_Change(0,3);	break;
		case 13:	Bar_Process_Count=13;Bar_Change(0,3);	break;
		case 14:	Bar_Process_Count=14;Bar_Change(0,3);	break;
		case 15:	Bar_Process_Count=15;Bar_Change(0,3);	break;
		case 16:	Bar_Process_Count=16;Bar_Change(0,3);	break;
		case 17:	Bar_Process_Count=17;Bar_Change(0,3);	break;;
		case 18:	Bar_Process_Count=18;Bar_Change(0,3);	break;
		case 19:	Bar_Process_Count=19;Bar_Change(0,3);	break;
		case 20:	Bar_Process_Count=20;Bar_Change(0,3);	break;
	}
}

//文件导入进度
void Recive_Handle_GetFileInputProcess_Function(void)
{
	if(USART1_RxBuffer[5]==0x64)
	{
		FileInputFinish_Flag=1;	
		FileInputFinish_Display();
	}		
}

//U和MIC的状态
void Recive_Handle_U_Oline_Function(void)
{	
	if(!Emergency_Mode && !MP3_Mode && !line1_Mode && !line2_Mode)
	{
		if(!WndGrp[WndNum])
			WndUpdate = 1;
	}
	else if(MP3_Mode)
	{
		MP3_Interface();
		Reset_MP3_Display();
	}
}

void Recive_Handle_U_NOline_Function(void)
{
	if(!Emergency_Mode && !MP3_Mode && !line1_Mode && !line2_Mode)
	{
		if(!WndGrp[WndNum])
			WndUpdate = 1;
	}
	else if(MP3_Mode)
		MP3_Interface();
}

void Recive_Handle_MIC_Oline_Function(void)
{
	if(!Emergency_Mode && !MP3_Mode && !line1_Mode && !line2_Mode)
	{
		if(!WndGrp[WndNum])
			WndUpdate = 1;
	}
}

void Recive_Handle_MIC_NOline_Function(void)
{
	if(!Emergency_Mode && !MP3_Mode && !line1_Mode && !line2_Mode)
	{
		if(!WndGrp[WndNum])
			WndUpdate = 1;
	}
}


//获取主板信息的回复
void Recive_Handle_GetMainInf_Function(void)
{
	mTimeBuffer[0]=USART1_RxBuffer[5];//年 低两位 
	mTimeBuffer[1]=USART1_RxBuffer[6];//月
	mTimeBuffer[2]=USART1_RxBuffer[7];//日
	mTimeBuffer[3]=USART1_RxBuffer[8];//时				
	mTimeBuffer[4]=USART1_RxBuffer[9];//分
	mTimeBuffer[5]=USART1_RxBuffer[10];//秒
	
	if(Emergency_Mode || MP3_Mode || line1_Mode || line2_Mode || Record_Flag)
		DrawDateTimeLine(0);
		
	
	if(USART1_RxBuffer[11] & 0x10) //global reset  bit4
	{
		Lcd_BackLightOn();

		txandre_AddResetPowerAmpTask();
	
		sys485_Proc();

		SCB->AIRCR =0X05FA0000|(u32)0x04; //系统复位。
	}
	
	if(USART1_RxBuffer[11] & 0x08) //self checks bit3 
	{
		Lcd_BackLightOn();
		SelfCheck(1);	
		return;
	}

	if(USART1_RxBuffer[11] & 0x80)//recording  bit7
	{									
		if(!Record_Flag)	
		{
			Record_Flag=1;
			
			BUZZER_OFF;
			led_CloseLed(0);	
			led_CloseLed(1);
			led_CloseLed(2);
			led_CloseLed(3);
			
			Lcd_BackLightOn();
			
			txandre_ClearAllTaskFlag();
			Playback_Mode = 0;
			if(DEF_PowerAmpCfg == WndGrp[WndNum])
					Clear_PowAmpCfg_Var();
			ui_RemoveAllWnd();//清除界面的 标志
			
			PowAmp_All_Mic();
			
			Record_Interface();//喊话界面
		}						
	}
	else//不处于喊话状态
	{
		if(Record_Flag)	
		{
			Record_Flag=0;
			
			Lcd_BackLightOn();

			if(ForceOn_Flag)
			{
				bSwitchToEmeMode = 0;
				if(!Emergency_Mode)
					Fun_KEY_Emer_Function();
				mre_AddStartAllAreaTask(1);
			}
			else if(bSwitchToEmeMode)
			{
				bSwitchToEmeMode = 0;
				if(!Emergency_Mode)
					Fun_KEY_Emer_Function();
			}
			else if(Emergency_Mode)
				PowAmp_All_Start();
			else if(Area_Startup_Num && (MP3_Mode || line1_Mode || line2_Mode))
				PowAmp_All_PlayOrStop();
			else
				PowAmp_All_Stop();
					
			if(Emergency_Mode)	//退出喊话界面 回到原来界面
			{
				led_LightLed(0);	
				Emer_Interface();
			}
			else if(MP3_Mode)
			{
				led_LightLed(1);	
				MP3_Interface();
				Reset_MP3_Display();
			}
			else if(line1_Mode)
			{
				led_LightLed(2);	
				LINE1_Interface();
			}
			else if(line2_Mode)
			{
				led_LightLed(3);	
				LINE2_Interface();
			}
			else 
				WndUpdate = 1;
		}	
	}
	
	if(USART1_RxBuffer[11] & 0x04)//force on  bit2
	{
		if(!ForceOn_Flag)
		{
			ForceOn_Flag = 1;
			
			if(!Record_Flag)
			{
				if(!Emergency_Mode)
					Fun_KEY_Emer_Function();
			}
			mre_AddStartAllAreaTask(1);
		}
	}
	else
	{
		if(ForceOn_Flag)
		{
			ForceOn_Flag = 0;
			
			if(!Record_Flag)
			{
				if(Emergency_Mode)
				{
					Fun_KEY_Emer_Function();
					mre_AddStartAllAreaTask(0);
				}
			}
		}
	}
	
	if(USART1_RxBuffer[11] & 0x02)//删除文件  bit1
	{
		DelFile_Flag = 1;
	}
	else
	{
		DelFile_Flag = 0;
	}
		
	if(USART1_RxBuffer[11] & 0x01)//删除全部文件  bit0
	{
		if(!DelAllFile_Flag)
		{
			DelAllFile_Flag = 1;
		}
	}
	else
	{
		if(DelAllFile_Flag)
		{
			DelAllFile_Flag = 0;
			
			if(DEF_DelAllFile == WndGrp[WndNum])
				DelAllFileFinish_Display();
			else if(DEF_FactoryConfig == WndGrp[WndNum])
				FactoryCfgFinish_Display();
		}
	}

	if(DEF_PATROLWND == WndGrp[WndNum])
	{
		if(USART1_RxBuffer[11] & 0x40)//U盘在线  bit6
		{
			if(!MainInfSta_UOnline_Flag)
			{
				MainInfSta_UOnline_Flag = 1;

				Lcd_BackLightOn();

				Recive_Handle_U_Oline_Function();
			}
		}
		else
		{
				if(MainInfSta_UOnline_Flag)
				{
					MainInfSta_UOnline_Flag = 0;
					
					Lcd_BackLightOn();

					Recive_Handle_U_NOline_Function();
				}
		}
		
		if(USART1_RxBuffer[11] & 0x20)//MIC在线   bit5
		{
			if(!MainInfSta_MicOnline_Flag)
			{
				MainInfSta_MicOnline_Flag = 1;
				
				Lcd_BackLightOn();

				Recive_Handle_MIC_Oline_Function();
			}
		}
		else
		{
			if(MainInfSta_MicOnline_Flag)
			{
				MainInfSta_MicOnline_Flag = 0;

				Lcd_BackLightOn();

				Recive_Handle_MIC_NOline_Function();
			}
		}
	}
}

void Recive_Handle_InfoArea_Normal_Function(int nInfoArea)
{
	int nArea,nLedIndex;
	int nFirstArea = 30 * nInfoArea;

	for(nArea =nFirstArea;nArea<nFirstArea+30;nArea++)
	{
		if(!config_IsAreaCfgedAny(nArea))
			continue;
		
		if(Area_Startup_Flag[nArea])
		{
			Area_Startup_Flag[nArea]=0;
			Area_Startup_Num--;
			
			if(nArea<30)
			{
				nLedIndex = (nArea/6)*12+nArea%6+6;
				led_CloseLed(nLedIndex);
			}
			
		}
		
		if(Area_Fault_Flag[nArea])
		{
			Area_Fault_Flag[nArea]=0;
			Area_Fault_Num--;
			
			if(!Area_Fault_Num)
			{
				AreaFault_BuzzerOn_Flag = 0;
				
				if(!PowerAmp_BuzzerOn_Flag)
					BUZZER_OFF;
			}
			
			if(nArea<30)
			{
				nLedIndex = (nArea/6)*12+nArea%6+12;
				led_CloseLed(nLedIndex);
			}
		}
	}
}

void Recive_Handle_InfoArea_Exception_Function(int nInfoArea)
{
	u8 nMask;
	int nIndex,nArea,nLedIndex;
	int nFirstArea = 30 * nInfoArea;
	
	for(nIndex = 0;nIndex<30;nIndex++)
	{
		nMask = 1<<(nIndex%8);
		nArea = nFirstArea + nIndex;
		
		if(!config_IsAreaCfgedAny(nArea))
			continue;
		
		if(USART1_RxBuffer[5+(nIndex>>3)] & nMask)
		{
			if(!Area_Startup_Flag[nArea])
			{
				Area_Startup_Flag[nArea]=1;
				Area_Startup_Num++;
				
				if(nArea<30)
				{
					nLedIndex = (nArea/6)*12+nArea%6+6;
					led_LightLed(nLedIndex);
				}
				
				if(!Emergency_Mode)
				{
					if(!Record_Flag)
					{
						Lcd_BackLightOn();
						Fun_KEY_Emer_Function();
					}
					else
						bSwitchToEmeMode = 1;
				}
			}
		}
		else
		{
			if(Area_Startup_Flag[nArea])
			{
				Area_Startup_Flag[nArea]=0;
				Area_Startup_Num--;
				
				if(nArea<30)
				{
					nLedIndex = (nArea/6)*12+nArea%6+6;
					led_CloseLed(nLedIndex);
				}
			}
		}
		
		if(USART1_RxBuffer[9+(nIndex>>3)] & nMask)
		{
			if(!Area_Fault_Flag[nArea])
			{
				Area_Fault_Flag[nArea]=1;
				Area_Fault_Num++;
				
				AreaFault_BuzzerOn_Flag = 1;
				if(!Record_Flag)
					BUZZER_ON;
				
				if(nArea<30)
				{
					nLedIndex = (nArea/6)*12+nArea%6+12;
					led_LightLed(nLedIndex);
				}
			}
		}
		else
		{
			if(Area_Fault_Flag[nArea])
			{
				Area_Fault_Flag[nArea]=0;
				Area_Fault_Num--;
				
				if(!Area_Fault_Num)
				{
					AreaFault_BuzzerOn_Flag = 0;
					
					if(!PowerAmp_BuzzerOn_Flag)
						BUZZER_OFF;
				}
				
				if(nArea<30)
				{
					nLedIndex = (nArea/6)*12+nArea%6+12;
					led_CloseLed(nLedIndex);
				}
			}
		}
	}
}

//5种模式进入和退出
void Into_Emer_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_Into_Emer_Function();
}
void Out_Emer_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_Out_Emer_Function();  
}
void Into_MP3_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_Into_MP3_Function();
}
void Out_MP3_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_Out_MP3_Function();
}
void Into_Line1_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_Into_Line1_Function();
}
void Out_Line1_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_Out_Line1_Function();
}
void Into_Line2_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_Into_Line2_Function();
}
void Out_Line2_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_Out_Line2_Function();
}
void Into_PlayBack_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) 	Recive_Handle_Into_PlayBack_Function();
}
void Out_PlayBack_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_Out_PlayBack_Function();
}

//播放相关的
void UP_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) Recive_Handle_UP_Function();
}
void Down_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) Recive_Handle_Down_Function();
}
void Forward_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_Forward_Function();
}
void Backward_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	Recive_Handle_Backward_Function();
}

//单曲播放
void Single_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) 	Single_Or_Sequence_Flag=1;
}

//顺序播放
void Sequence_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) 	Single_Or_Sequence_Flag=0;
}

//手动
void Automatic_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	led_CloseLed(0x04); 	
}

//手动
void Manual_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)	led_LightLed(0x04);
}

//录音删除
void PlayBackDel_Handle(void)
{
	if(USART1_RxBuffer[4]==0)    			Recive_Handle_PlayBackDel_Function();
}

//删除全部文件
void DelAllFile_Handle(void)
{
	if(USART1_RxBuffer[4]==0)    			Recive_Handle_DelAllFile_Function();
}

//获取主板信息
void GetMainInf_Handle(void)
{
	if(USART1_RxBuffer[4]==0)   		 	Recive_Handle_GetMainInf_Function();
}

//文件导入进度
void GetFileInputProcess_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) 			Recive_Handle_GetFileInputProcess_Function();	
}

//获取MP3歌曲名字
void GetSongName_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)			Recive_Handle_SongName_Function();
}

//获取MP3总数量
void GetSongSum_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) 			Recive_Handle_SongCurrentSum_Function();
}

//获取回放总数量
void GetPlayBackSum_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)			Recive_Handle_PlayBackSum_Function();
}

//获取MP3当前排名
void GetSongCurrentNum_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) 			Recive_Handle_SongCurrentNum_Function();
}

//获取回放当前排名
void GetPlayBackCurrentNum_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)			Recive_Handle_PlayBackCurrentNum_Function();
}

//获取回放历史时间
void GetPlayBackHistoryTime_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)			Recive_Handle_PlayBackHistoryTime_Function();
}

//获取回放播放进度时间
void GetPlayBackPlayProcessTime_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) 			Recive_Handle_PlayBackPlayProcessTime_Function();
}

//获取回放播放进度条
void GetPlayBackPlayProcessBar_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) 			Recive_Handle_PlayBackPlayProcessBar_Function();
}

//获取应急广播播放进度时间
void GetEmerPlayProcessTime_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00)			Recive_Handle_EmerPlayProcessTime_Function();
}
//获取应急广播播放进度条
void GetEmerPlayProcessBar_Handle(void)
{
	if(USART1_RxBuffer[4]==0x00) 			Recive_Handle_EmerPlayProcessBar_Function();
}

//获取区异常信息
void Get30AreaInf_Handle(int nInfoArea)
{
	if(!USART1_RxBuffer[4])
		Recive_Handle_InfoArea_Normal_Function(nInfoArea);
	else
		Recive_Handle_InfoArea_Exception_Function(nInfoArea);
}

//启动某区
void StartupArea_Handle(void)
{
	int nArea = USART1_TxBuffer[4]*256+USART1_TxBuffer[5]-1;
	int nLedIndex = (nArea/6)*12+nArea%6+6;

	if(Area_Startup_Flag[nArea])
		return;

	Area_Startup_Flag[nArea]=1;
	Area_Startup_Num++;
	
	led_LightLed(nLedIndex);
}

//停动某区
void StopArea_Handle(void)
{
	int nArea = USART1_TxBuffer[4]*256+USART1_TxBuffer[5]-1;
	int nLedIndex = (nArea/6)*12+nArea%6+6;

	if(!Area_Startup_Flag[nArea])
		return;
	
	Area_Startup_Flag[nArea]=0;
	Area_Startup_Num--;
	
	led_CloseLed(nLedIndex);
}

//巡检中主板的回复
void mre_Recv_Inspection_Ack(int bFault)
{
	if(bFault)
	{
		MainBoard_Exist_Flag=1;
		
		mre_AddGetAreaInfoTask();
	}
	else
	{
		if(MainBoard_Exist_Flag)
		{
			MainBoard_Exist_Flag = 0;
			
			mre_AddGetAreaInfoTask();
		}
	}
}		 

