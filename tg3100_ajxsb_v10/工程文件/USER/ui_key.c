#include "stm32f10x_gpio.h"
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
#include "mre.h" 
#include "pre.h"
#include "ui_key.h"
#include "config.h"
#include "ui_poweramp.h"
#include "timer.h"
#include "task.h"

u8 Mute_Flag,bBackGround_Flag,bCheckSelf_Flag;//�����ͱ���,�Լ�
static u8 bManual_Flag = 0;
static u8 single_or_Sequential;

void CheckUpdateWnd(void)
{
	if(DEF_PATROLWND != WndGrp[WndNum] 
		&& !Emergency_Mode 
		&& !MP3_Mode 
		&& !line1_Mode 
		&& !line2_Mode
		&& !Record_Flag)	
		WndUpdate = 1;
}

void Fun_KEY_Play_Function(void)
{
	if(MP3_Mode && MainInfSta_UOnline_Flag && !Record_Flag)
		Play();
	
	CheckUpdateWnd();
}

void Fun_KEY_Stop_Function(void)
{
	if(MP3_Mode && MainInfSta_UOnline_Flag && !Record_Flag)
		Stop_Play();									
	
	CheckUpdateWnd();
}

void Fun_KEY_DOWN_Short_Function(void)
{
	if(MP3_Mode && !Record_Flag)	
		Next_Song();

	CheckUpdateWnd();
}
void Fun_KEY_DOWN_Long_Function(void)
{
	if(MP3_Mode && !Record_Flag)	
		Fast_Forward(); 			

	CheckUpdateWnd();
}

void Fun_KEY_UP_Short_Function(void)
{
	if(MP3_Mode && !Record_Flag)	
		Last_Song();					

	CheckUpdateWnd();
}

void Fun_KEY_UP_Long_Function(void)
{
	if(MP3_Mode && !Record_Flag)	
		Fast_Back();				

	CheckUpdateWnd();
}

u8 SysPassword_Flag = 0;
void Fun_KEY_RPT_Function(void)
{
	if(MP3_Mode && !Record_Flag)
	{
		if(!single_or_Sequential)
		{			
			Main_PlayFlag(1);
			
			Fun_WriteStr(64,6,(uint8_t *)("����ѭ��"),0);
			single_or_Sequential=1;					
		}
		else
		{	
			Main_PlayFlag(0);
			
			Fun_WriteStr(64,6,(uint8_t *)("˳�򲥷�"),0);
			single_or_Sequential=0;		
		}	
	}
	else if(DEF_CheckGovPassword == WndGrp[WndNum])
		Set_SysPassword(0);
}

void SelfCheck(int bGlobal)
{
	u16 i;
	int nFaultLedState;
		
	bCheckSelf_Flag = 1;
	
	if(bGlobal)
	{
		txandre_AddSelfCheckTask();
		
		sys485_Proc();
	}
		
	for(i=0;i<LED_SUM-1;i++)
		led_LightLed(i);
	
	nFaultLedState = FAULTLED_STATE;
	
	FAULTLED_ON;
	BUZZER_ON;
	
	//LcdFill(0xFF);
	Fun_DisTandaBMP();
	
	if(bGlobal)
		delay_ms(3000);  
	else
		delay_ms(1500);  
	
	for(i=0;i<LED_SUM-1;i++)
		led_CloseLed(i);
	
	bCheckSelf_Flag = 0;
	
	BUZZER_OFF;
	FAULTLED_OFF;
	
	
	LcdFill(0x00);
	
	//�ָ����Ź��ϵƺͷ�����
	if(nFaultLedState)
		FAULTLED_ON;
	if(PowerAmp_BuzzerOn_Flag || AreaFault_BuzzerOn_Flag)
		BUZZER_ON;
	
	//�ָ��ֶ�ָʾ��
	if(bManual_Flag)
		led_LightLed(0x04);
	
	//�ָ���������ָʾ��
	if(bBackGround_Flag)
		led_LightLed(0x05);
	else if(Mute_Flag)
		led_FlashLed(0x05);
	
	//�ָ��������͹��ϵ�
	if(DEF_PowerAmpCfg != WndGrp[WndNum])
		ShowAreaLed();

	//�˳��Լ���� �ص�ԭ������
	if(Emergency_Mode)
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
	{		
		ui_ReDrawWnd();
	}
}

void Fun_KEY_SelfCheck_Function(void)
{
	if(Emergency_Mode || MP3_Mode || line1_Mode || line2_Mode || Playback_Mode || Record_Flag)
		return;
	
	if(WndGrp[WndNum] == DEF_PATROLWND)
		ui_ShowSelfCheckWnd();
	else if(DEF_CheckGovPassword == WndGrp[WndNum])
		Set_SysPassword(1);
}

void Key_Menu(void)
{
	//�˵���
	if(Emergency_Mode || MP3_Mode || line1_Mode || line2_Mode || Playback_Mode || Record_Flag)
		return;
	
	if(WndGrp[WndNum] == DEF_PATROLWND)
		ui_ShowMainMenu();
	else if(DEF_CheckGovPassword == WndGrp[WndNum])
		Set_SysPassword(2);
}	

void Fun_KEY_Emer_Function(void)
{
	if(Record_Flag)
		return;
	
	if(!Emergency_Mode)
	{
		PowAmp_All_Start();
		
		Emergency_Mode=1;
		MP3_Mode=0;
		line2_Mode=0;
		line1_Mode=0;
		Playback_Mode=0;
		
		PlayBack_Play_Flag=0;
		
		led_FlashLed(0);	
		led_CloseLed(1);
		led_CloseLed(2);
		led_CloseLed(3);
		
		if(DEF_PowerAmpCfg == WndGrp[WndNum])
			Clear_PowAmpCfg_Var();
		
		ui_RemoveAllWnd();
		
		Emer_Interface();
		
		Task_Flag=1;
		TX_Task_Command_Flag[3]=1;		 //����Ӧ���㲥ģʽ		
	}
	else
	{	
		if(ForceOn_Flag)
			return;
		
		Emergency_Mode=0;
		MP3_Mode=0;
		line2_Mode=0;
		line1_Mode=0;
		Playback_Mode=0;
		
		Bar_Process_Count=0;
		
		led_FlashLed(0);
		
		Task_Flag=1;
		TX_Task_Command_Flag[4]=1;		//�˳�Ӧ���㲥ģʽ
		
		PowAmp_All_Stop();
	}
}

void Fun_KEY_MP3_Function(void)
{
	if(Record_Flag || ForceOn_Flag)
		return;
	
	if(!MP3_Mode)
	{		
		if(!line1_Mode && !line2_Mode)
		{
			if(Area_Startup_Num)
				PowAmp_All_PlayOrStop();
			else
				PowAmp_All_Stop();
		}

		Emergency_Mode=0;
		MP3_Mode=1;
		line2_Mode=0;
		line1_Mode=0;
		Playback_Mode=0;

		PlayBack_Play_Flag=0;
		
		led_CloseLed(0);
		led_FlashLed(1);		
		led_CloseLed(2);
		led_CloseLed(3);
		
		if(DEF_PowerAmpCfg == WndGrp[WndNum])
			Clear_PowAmpCfg_Var();
		
		MP3_Interface();
		
		ui_RemoveAllWnd();
		
		if(!MainInfSta_UOnline_Flag)
			Fun_WriteStr(23,4,(uint8_t *)("��U��"),0);
		else
			Reset_MP3_Display();


		Task_Flag=1;		
		TX_Task_Command_Flag[5]=1;	//����MP3ģʽ		

		Task_Flag=1;
		TX_Task_Command_Flag[26]=1;//��ȡ������������
	}
	else
	{			
		Emergency_Mode=0;
		MP3_Mode=0;
		line2_Mode=0;
		line1_Mode=0;
		Playback_Mode=0;

		single_or_Sequential=0;
		
		led_FlashLed(1);	
		
		Task_Flag=1;
		TX_Task_Command_Flag[6]=1;//�˳�MP3ģʽ
		
		PowAmp_All_Stop();
	}
}

void Fun_KEY_LINE1_Function(void)
{
	if(Record_Flag || ForceOn_Flag)
		return;
	
	if(!line1_Mode)
	{
		if(!MP3_Mode && !line2_Mode)
		{
			if(Area_Startup_Num)
				PowAmp_All_PlayOrStop();
			else
				PowAmp_All_Stop();
		}

		Emergency_Mode=0;
		MP3_Mode=0;
		line1_Mode=1;
		line2_Mode=0;
		Playback_Mode=0;
	
		PlayBack_Play_Flag=0;
		
		led_CloseLed(0);
		led_CloseLed(1);
		led_FlashLed(2);						
		led_CloseLed(3);
		
		if(DEF_PowerAmpCfg == WndGrp[WndNum])
			Clear_PowAmpCfg_Var();
		
		LINE1_Interface();

		ui_RemoveAllWnd();	

		Task_Flag=1;
		TX_Task_Command_Flag[7]=1;//����Line1
	}
	else
	{				
		Emergency_Mode=0;
		MP3_Mode=0;
		line1_Mode=0;
		line2_Mode=0;
		Playback_Mode=0;
		
		led_FlashLed(2);
		
		Task_Flag=1;
		TX_Task_Command_Flag[8]=1;//�˳�Line1

		PowAmp_All_Stop();
	}
}

void Fun_KEY_LINE2_Function(void)
{
	if(Record_Flag || ForceOn_Flag)
		return;
	
	if(!line2_Mode)
	{
		if(!MP3_Mode && !line1_Mode)
		{
			if(Area_Startup_Num)
				PowAmp_All_PlayOrStop();
			else
				PowAmp_All_Stop();
		}
		
		Emergency_Mode=0;
		MP3_Mode=0;
		line1_Mode=0;
		line2_Mode=1;
		Playback_Mode=0;
	
		PlayBack_Play_Flag=0;
		
		led_CloseLed(0);
		led_CloseLed(1);
		led_CloseLed(2);
		led_FlashLed(3);
		
		if(DEF_PowerAmpCfg==WndGrp[WndNum])
			Clear_PowAmpCfg_Var();

		LINE2_Interface();

		ui_RemoveAllWnd();	

		Task_Flag=1;
		TX_Task_Command_Flag[9]=1;	//����Line2	
	}
	else
	{				
		Emergency_Mode=0;
		MP3_Mode=0;
		line1_Mode=0;
		line2_Mode=0;
		Playback_Mode=0;
		
		led_FlashLed(3);
		
		Task_Flag=1;
		TX_Task_Command_Flag[10]=1;	//�˳�Line2

		PowAmp_All_Stop();
	}
}

void Fun_KEY_MANUAL_Function(void)
{
	if(!bManual_Flag)
	{
		bManual_Flag = 1;
		
		led_FlashLed(0x04);
		
		Task_Flag=1;
		TX_Task_Command_Flag[23]=1;//�ֶ���־���� 		
	}
	else
	{
		bManual_Flag = 0;
		
		led_FlashLed(0x04);
		
		Task_Flag=1;
		TX_Task_Command_Flag[22]=1;//�Զ���־���� 
	}
}

void Fun_KEY_MUTE_Function(void)
{
	if(PowerAmp_BuzzerOn_Flag || AreaFault_BuzzerOn_Flag)
	{
		PowerAmp_BuzzerOn_Flag = 0;
		AreaFault_BuzzerOn_Flag = 0;
		
		Mute_Flag = 1;
		
		if(!bBackGround_Flag)
			led_FlashLed(0x05);
	}
	else if(Mute_Flag)
	{
		Mute_Flag = 0;
		
		if(!bBackGround_Flag)
			led_CloseLed(0x05);
	}
}

void Fun_KEY_BackGround_Function(void)
{
	if(!bBackGround_Flag)
	{
		bBackGround_Flag = 1;	//����ģʽ
		
		led_LightLed(0x05);
	}
	else
	{
		bBackGround_Flag=0;
		
		if(Mute_Flag)
			led_FlashLed(0x05); 
		else
			led_CloseLed(0x05); 
	}
}

void OnAreaKey(int nArea)
{
	int nLedIndex,nAreaStartCmdIndex;
	
	if(ForceOn_Flag)
		return;
	
	if(nArea<0 || nArea>=270)
		return;
	
	nLedIndex = (nArea/6)*12+nArea%6+6;
	nAreaStartCmdIndex = 151 + nArea*2;

	if(DEF_PowerAmpCfg == WndGrp[WndNum])
	{
		led_ToggleLed(nLedIndex);
		config_ToggleAreaFlag(nArea);
	}
	else if(config_IsAreaCfgedAny(nArea))
	{	
		if(!Area_Fault_Flag[nArea]) 
		{
			ui_RemoveAllWnd();
			
			if(!Area_Startup_Flag[nArea])//������
			{				
				led_FlashLed(nLedIndex);
				
				Task_Flag=1;
				
				if(Emergency_Mode || MP3_Mode || line1_Mode || line2_Mode || bBackGround_Flag)//����ģʽ
					TX_Task_Command_Flag[nAreaStartCmdIndex]=1; //������ ����		
				else
				{
					TX_Task_Command_Flag[nAreaStartCmdIndex]=1; //������ ����	
					Fun_KEY_Emer_Function();
				}
				
				if(!Record_Flag  && (MP3_Mode || line1_Mode || line2_Mode))
					PowAmp_Play(nArea);	//�����ڹ��Ų���ģʽ
			}
			else 	//��ͣ��
			{	
				Task_Flag=1;
				TX_Task_Command_Flag[nAreaStartCmdIndex+1]=1; //ͣ���� ����
				
				led_FlashLed(nLedIndex); 		//��������ָʾ��
				
				if(!Record_Flag  && (MP3_Mode || line1_Mode || line2_Mode))
					PowAmp_Stop(nArea);	//�����ڹ��Ų���ģʽ
			}	
		}	
	}
}

void ui_KeyProc(void)
{
	if(DelAllFile_Flag)
		return;
	
	switch (SystemKeyValue)
	{
		case KEY_Play:			Fun_KEY_Play_Function();			break; 
		case KEY_Stop:			Fun_KEY_Stop_Function();			break; 	
		case KEY_DOWN_Short:Fun_KEY_DOWN_Short_Function();break; 
		case KEY_UP_Short:	Fun_KEY_UP_Short_Function();	break; 
		case KEY_DOWN_Long:	Fun_KEY_DOWN_Long_Function();	break; 
		case KEY_UP_Long:		Fun_KEY_UP_Long_Function();		break;
		
		case KEY_RPT:				Fun_KEY_RPT_Function();				break;
		case KEY_SelfCheck:	Fun_KEY_SelfCheck_Function();	break; 	
		case KEY_MENU:			Key_Menu();										break;	
			
		case KEY_Emer:			Fun_KEY_Emer_Function();			break; 
		case KEY_MP3:				Fun_KEY_MP3_Function();				break; 
		case KEY_LINE1:			Fun_KEY_LINE1_Function();			break; 
		case KEY_LINE2:	 		Fun_KEY_LINE2_Function();			break; 
		case KEY_MANUAL:		Fun_KEY_MANUAL_Function();		break; 
		case KEY_MUTE:			Fun_KEY_MUTE_Function();			break; 
		case KEY_BG:				Fun_KEY_BackGround_Function();break;
		
		default:
			
			if(SystemKeyValue >= KEY_1 && SystemKeyValue <= KEY_30)
				OnAreaKey(SystemKeyValue-KEY_1);
	}
}
