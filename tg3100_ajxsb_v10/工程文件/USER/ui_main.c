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
#include "mre.h" 
#include "powamp.h" 
#include "ui_key.h"
#include "config.h"
#include "ui_poweramp.h"
#include "ui_menu.h"
#include "ui_file.h"
#include "ui_setting.h"
#include "timer.h"

#define SONG_NAME_LEN 100
u8 Location_Code[SONG_NAME_LEN];

uint8_t  mTimeBuffer[6]={17,7,20,8,45,0}; //时间缓存：年月日时分秒
uint8_t  Last_Time[7]={0,58,12,04,0,5,17};  
const uint8_t CtrlEditStartWndFocus[]= {0,0,0,0,0, 0,0,0,0,1,  1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,};
const uint8_t Bar_Process_Init_Code[][16]=
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01
};

uint8_t Area_No_Stratup_Dis;
uint8_t Bar_Process_Count;
uint8_t Time_Adjust_Play;
u8 PlayBack_Play_Flag;
u8 RecordQueryWnd_Flag,RecordDelWnd_Flag,CfgNativeAddressWnd_Flag;
u8 FileInputWnd_Flag,FileInputFinish_Flag;
u8 DelAllFileWnd_Flag,DelAllFile_Flag;
u8 UserPasswordWnd_Flag,GovPasswordWnd_Flag,CheckUserPasswordWnd_Flag,CheckGovPasswordWnd_Flag,NextWnd_Flag,PasswordSaveWnd_Flag;
u8 PowerAmpRegWnd_Flag;
u8 Cfg_PowAmp1to10_InitSta_OneEnter_Flag;
u8 FactoryCfgWnd_Flag;

uint16_t CurWndFocus;                               //窗口焦点位置
uint16_t OldWndFocus;                               //  
uint8_t  WndUpdate;                                 //窗口更新
uint8_t  WndNum;                                    //窗口嵌套个数
uint8_t  InputSum;                                  //输入字符总数
uint8_t  WndGrp[6];                                 //窗口组
uint32_t  CusCnt;                                    //光标计数
uint8_t  CusState;                                  //光标状态
uint8_t  Cus_x;                                     /*光标X坐标*/
uint8_t  Cus_y;                                     /*光标Y坐标*/
uint8_t  CusType;                                   //闪烁类型
uint8_t  CusData;                                   //光标内容
uint8_t  WndCtrlBuffer[DEF_MAXEDITSUM][DEF_MAXEDITLENGTH];		/*窗口输入信息缓存*/
ErrorStatus HSEStartUpStatus;

const struct StructHead_SysWndType Struct_SystemWnd[] =
{
	{DEF_PATROLWND      , 1, 	0                    					,0},//00正常监控相关界面
	{DEF_MENUWND        , 4,  Struct_CtrlMainMenuWnd 				,0},//01系统主菜单界面控件
	{DEF_FileOperate    , 4,  Struct_CtrlFileOperateWnd			,0},//02文件操作界面控件
	{DEF_PoweAmpRegis   , 4,  Struct_CtrlPoweiAmpRegisWnd		,0},//03功放注册界面控件
	{DEF_SystemConfig   , 4,  Struct_CtrlSystemCfgWnd  			,0},//04系统设置界面控件
	{DEF_RecordQuery    , 3,  Struct_CtrlRecordQueryWnd			,0},//05录音查询界面控件
	{DEF_RecordDel      , 3,  Struct_CtrlRecordDelWnd				,0},//06录音删除界面控件
	{DEF_FileInput      , 2,  Struct_CtrlFileInputWnd				,0},//07文件导入界面控件
	{DEF_PowerAmpCfg    , 3,  Struct_CtrlPowerAmpCfgWnd			,0},//08功放配置界面控件
	{DEF_TimeCfg        , 13, Struct_CtrlSysTimeCfgWnd			,0},//09时间设置界面控件
	{DEF_AddressCfg     , 1,  Struct_CtrlAddressCfgWnd			,0},//10本机地址界面控件
	{DEF_PasswordCfg    , 2,  Struct_CtrlPasswordCfgWnd			,0},//11密码设置界面控件
	{DEF_UserPasswordCfg, 2,  Struct_CtrlUserPasswordCfgWnd	,0},//12用户密码设置界面控件
	{DEF_GovPasswordCfg , 2,  Struct_CtrlGovPasswordCfgWnd	,0},//13管理密码设置界面控件
	{DEF_CheckUserPassword,2, Struct_CtrlCheckUserPasswordWnd,0},//14核对用户密码界面控件
	{DEF_CheckGovPassword,2,  Struct_CtrlCheckGovPasswordWnd,0},//15核对管理密码界面控件
	{DEF_DelAllFile				,2, Struct_CtrlDelAllFileWnd,0},//16删除全部文件界面控件
	{DEF_FactoryConfig		,2, Struct_CtrlFactoryCfgWnd,0},//17恢复出厂设置界面控件
	{DEF_PasswordSave			,3, Struct_CtrlPasswordSaveWnd,0},//18密码保存确认界面控件
};

void Com_itoa(uint16_t i, uint8_t *a)
{
	uint8_t len = 1;
	uint8_t ch;
	uint16_t k;
	uint8_t j;
	uint8_t m;

	k = i;
	while((k / 10) != 0)
	{
		len++;
		k = k / 10;
	}
	for(j=0; j<len; j++)
	{
		ch = i % 10;
		i = i / 10;
		m = len - j - 1;
		*(a + m) = ch + 48;
	}
	*( a + len) = 0;
}


void Fun_EditBoxInits(uint8_t udata)
{
	uint8_t i, j;

	for(i=0; i<DEF_MAXEDITSUM; i++)
	{
		for(j=0; j<DEF_MAXEDITLENGTH; j++)
		{
			WndCtrlBuffer[i][j] = udata;
		}
	}
	InputSum = 0;
}

void delay_ms(u16 ms)
{
	u32 j;
	u16 i;
	for(i=ms;i>0;i--)
	for(j=14253;j>0;j--);	  
}

void Bar_Proceess_Init_Interface(u8 y)
{
	u8 i;
	delayus(150);		
	LCD_GotoXY(32,y);
	delayus(150);
	LCD_WriteData((0xFF));
	for(i=0;i<15;i++)
	{
		LCD_WriteData((0x01));
	}
	LCD_GotoXY(32,y+1);
	delayus(150);
	LCD_WriteData((0xFF));
	for(i=0;i<15;i++)
	{
		LCD_WriteData((0x80));
	}
	
	delayus(150);		
	LCD_GotoXY(48,y);
	delayus(150);
	for(i=0;i<16;i++)
	{
		LCD_WriteData((0x01));
	}
	LCD_GotoXY(48,y+1);
	delayus(150);
	for(i=0;i<16;i++)
	{
		LCD_WriteData((0x80));
	}
	
	delayus(150);		
	LCD_GotoXY(64,y);
	delayus(150);
	for(i=0;i<16;i++)
	{
		LCD_WriteData((0x01));
	}
	LCD_GotoXY(64,y+1);
	delayus(150);
	for(i=0;i<16;i++)
	{
		LCD_WriteData((0x80));
	}
	
	delayus(150);		
	LCD_GotoXY(80,y);
	delayus(150);
	for(i=0;i<15;i++)
	{
		LCD_WriteData((0x01));
	}
	LCD_WriteData((0xFF));
	LCD_GotoXY(80,y+1);
	delayus(150);
	for(i=0;i<15;i++)
	{
		LCD_WriteData((0x80));
	}
	LCD_WriteData((0xFF));
}

void Bar_Change(u8 type,u8 y)
{
	u8 i,x=35;
	if(Bar_Process_Count>0)
	{
		for(i=0;i<Bar_Process_Count;i++)
		{		
			LCD_GotoXY(x+i*3,y);
			if(type==0)
			{
				LCD_WriteData(0xF9);
				LCD_WriteData(0xF9);
			}
			else
			{
				LCD_WriteData(0x06);
				LCD_WriteData(0x06);
			}		
			LCD_GotoXY(x+i*3,y+1);
			if(type==0)
			{
				LCD_WriteData(0x9F);
				LCD_WriteData(0x9F);
			}
			else
			{
				LCD_WriteData(0x60);
				LCD_WriteData(0x60);
			}		
		}		
	}		
}


 
static void Byte2Hex(u8 nValue,u8 *strBuf)
{
	u8 nHalf;
	
	nHalf = nValue>>4;
	if(nHalf<0x0A)
		nHalf += 0x30;
	else
		nHalf += ('A' - 0x0A);
	
	strBuf[0] = nHalf;
	
	nHalf = nValue&0x0F;
	if(nHalf<0x0A)
		nHalf += 0x30;
	else
		nHalf += ('A' - 0x0A);
	
	strBuf[1] = nHalf;
}

void Byte2Str(u8 nValue,u8 *pBuf)
{
	int nPos;
	
	nPos=0;
	if(nValue>9)
	{
		pBuf[nPos++] = nValue/10+0x30;
		nValue %= 10;
	}
	else
		pBuf[nPos++] = 0x30;
	
	pBuf[nPos++] = nValue+0x30;
	pBuf[nPos++] = 0;
}

int DrawDateTimeLine(int bReDraw)
{
	int bUpdated;	
	u8 a[4];
	
	bUpdated = 0;

	if(bReDraw)
	{
		Fun_WriteStr(0,0,(uint8_t *)("   月  日   :   "),0);
		bUpdated = 1;
	}

	//月
	if(Last_Time[1] != mTimeBuffer[1] || bReDraw)
	{
		Last_Time[1] = mTimeBuffer[1];//月
		bUpdated = 1;
		
		Byte2Str(mTimeBuffer[1],a);
		Fun_WriteStr(8,0,a,0);
	}		

	//日
	if(Last_Time[2] != mTimeBuffer[2] || bReDraw)
	{
		Last_Time[2] = mTimeBuffer[2];//日
		bUpdated = 1;
		
		Byte2Str(mTimeBuffer[2],a);
		Fun_WriteStr(39,0,a,0);
	}

	//时
	if(Last_Time[3] != mTimeBuffer[3] || bReDraw)
	{
		Last_Time[3] = mTimeBuffer[3];//时
		bUpdated = 1;
		
		Byte2Str(mTimeBuffer[3],a);
		Fun_WriteStr(78,0,a,0);
	}
		
	//分
	if(Last_Time[4] != mTimeBuffer[4] || bReDraw)
	{
		Last_Time[4] = mTimeBuffer[4];//时
		bUpdated = 1;
		
		Byte2Str(mTimeBuffer[4],a);
		Fun_WriteStr(102,0,a,0);
	}
	return bUpdated;
}


static u32 nFlashCount,bFlash_Flag;
static void DrawMicStatus(void)
{
	if(MainInfSta_MicOnline_Flag)
	{
		if(nFlashCount)
		{
			nFlashCount = 0;
			bFlash_Flag = 0;
		}
		return;
	}
	
	nFlashCount++;
	if(nFlashCount>0x10000)
	{
		nFlashCount = 0;
		bFlash_Flag = !bFlash_Flag;
		
		if(bFlash_Flag)
			Fun_WriteStr(0,4,(uint8_t *)("MIC"),0);
		else
			LcdFillRect(0,4,24,2,0);
	}
}
	
void ui_ShowSys485Error(int nAddr,int nCmd)
{
	u8 strBuf[3];
	
	LcdFill(0x00);
	Fun_WriteStr(16,0,(uint8_t *)("总线通信错误"),0);
	
	strBuf[2] = 0;
	Fun_WriteStr(24,4,(uint8_t *)("地址:0x"),0);
	Byte2Hex(nAddr,strBuf);
	Fun_WriteStr(80,4,strBuf,0);
	
	Fun_WriteStr(24,6,(uint8_t *)("命令:0x"),0);
	Byte2Hex(nCmd,strBuf);
	Fun_WriteStr(80,6,strBuf,0);
}


void Emer_Interface(void)
{
	int nRow;
	DrawDateTimeLine(1);
	
	for(nRow=2;nRow<8;nRow++)
		LCD_WriteLine(nRow,0);
	
	Bar_Proceess_Init_Interface(3); 
	Fun_WriteStr(32,6,(uint8_t *)("应急广播"),0);
}

void MP3_Interface(void)
{
	int nRow;
	DrawDateTimeLine(1);
	
	for(nRow=2;nRow<8;nRow++)
		LCD_WriteLine(nRow,0);
	
	Fun_WriteStr(0,2,(uint8_t *)("MP3"),0);

	if(!MainInfSta_UOnline_Flag)
		Fun_WriteStr(23,4,(uint8_t *)("无U盘"),0);
	
	if(Single_Or_Sequence_Flag==0)	
		Fun_WriteStr(64,6,(uint8_t *)("顺序播放"),0);
	else	
		Fun_WriteStr(64,6,(uint8_t *)("单曲循环"),0);
}

void LINE1_Interface(void)
{
	int nRow;
	DrawDateTimeLine(1);
	
	for(nRow=2;nRow<8;nRow++)
		LCD_WriteLine(nRow,0);

	Fun_WriteStr(32,6,(uint8_t *)("外线 1"),0);
}

void LINE2_Interface(void)
{
	int nRow;
	DrawDateTimeLine(1);
	
	for(nRow=2;nRow<8;nRow++)
		LCD_WriteLine(nRow,0);
	
	Fun_WriteStr(32,6,(uint8_t *)("外线 2"),0);
}

void Record_Interface(void)
{
	int nRow;
	DrawDateTimeLine(1);
	
	for(nRow=2;nRow<8;nRow++)
		LCD_WriteLine(nRow,0);
	
	Fun_WriteStr(40,4,(uint8_t *)("00:00"),0);
	
	Fun_WriteStr(32,6,(uint8_t *)("应急播音"),0);
}


static u16 Show_Num,Show_Sum;
static u8 Show_Name[17];
void Reset_MP3_Display(void)
{
	int nIndex;
	
	Show_Num = 0xFF;
	Show_Sum = 0xFF;
	for(nIndex=0;nIndex<17;nIndex++)
		Show_Name[nIndex] = 0xFF;
}

void MP3_Song_Current_Num_Display(void)
{
	int i,j;
	u8 str[5];	
	
	if(MP3_Current_Num>999)
		MP3_Current_Num = 999;
	
	if(Show_Num == MP3_Current_Num)
		return;
	
	Show_Num = MP3_Current_Num;
	
	Com_itoa(Show_Num,str);
	
	for(i=0;i<3;i++)
		if(!str[i])
			break;
		
	if(i<3)
	{
		for(i--,j=2;i>=0;i--,j--)
			str[j]=str[i];
		for(;j>=0;j--)
			str[j]=0x30;
	}
	str[3] = '/';
	str[4] = 0;
  Fun_WriteStr(0,6,str,0);
}

void MP3_Song_Sum_Num_Display(void)
{
	int i,j;
	u8 str[4];
	
	if(MP3_Sum>999)
		MP3_Sum = 999;

	if(Show_Sum == MP3_Sum)
		return;
	
	Show_Sum = MP3_Sum;
	
	Com_itoa(Show_Sum,str);
	
	for(i=0;i<3;i++)
		if(!str[i])
			break;
		
	if(i<3)
	{
		for(i--,j=2;i>=0;i--,j--)
			str[j]=str[i];
		for(;j>=0;j--)
			str[j]=0x30;
	}
	
	str[3] = 0;
  Fun_WriteStr(32,6,str,0);
}

void GBK_to_hex_quwei(u8 H,u8 L,u8 Location)
{
	Location_Code[Location]=H;
	Location_Code[Location+1]=L;		
}
void GBK_to_hex_Asiic(u8 data,u8 Location)
{	
	Location_Code[Location]=data;		
}
void Clear_Location_Code(void)
{	
	u8 i;
	for(i=0;i<SONG_NAME_LEN;i++)
	{
		Location_Code[i]=0;
	}		
}
void Clear_16_Location_Code(void)
{	
	u8 i;
	for(i=16;i<SONG_NAME_LEN;i++)
	{
		Location_Code[i]=0;
	}		
}

void MP3_Song_Name_Display(void)
{
	int nIndex;
	
	for(nIndex=0;nIndex<16;nIndex++)
		if(Location_Code[nIndex] != Show_Name[nIndex])
			break;
		
	if(nIndex<16)
	{
		for(nIndex=0;nIndex<16;nIndex++)
			Show_Name[nIndex] = Location_Code[nIndex];
		Show_Name[16]=0;
		
		LCD_WriteLine(4,0);
		LCD_WriteLine(5,0);
		
		Fun_WriteStr(0,4,Show_Name,0);
	}
}

void PlayBack_Current_Num_Display(void)
{
	int i,j;
	u8 str[5];
	
	if(PlayBack_Current_Num>999)
		PlayBack_Current_Num = 999;
	
	Com_itoa(PlayBack_Current_Num,str);
	
	for(i=0;i<3;i++)
		if(!str[i])
			break;
		
	if(i<3)
	{
		for(i--,j=2;i>=0;i--,j--)
			str[j]=str[i];
		for(;j>=0;j--)
			str[j]=0x30;
	}
	str[3] = '/';
	str[4] = 0;
	Fun_WriteStr(64,6,str,0);
}

void PlayBack_Current_Sum_Display(void)
{
	int i,j;
	u8 str[4];
	
	if(PlayBack_Current_Sum>999)
		PlayBack_Current_Sum = 999;
	
	Com_itoa(PlayBack_Current_Sum,str);
	
	for(i=0;i<3;i++)
		if(!str[i])
			break;
		
	if(i<3)
	{
		for(i--,j=2;i>=0;i--,j--)
			str[j]=str[i];
		for(;j>=0;j--)
			str[j]=0x30;
	}
	
	str[3] = 0;
  Fun_WriteStr(96,6,str,0);
}

void Playback_Current_Time_Display(void)
{
	u8 str[3];

	if(PlayBack_Current_Time_Min > 99)
		PlayBack_Current_Time_Min = 99;
	
	if(PlayBack_Current_Time_Sec >99) 
		PlayBack_Current_Time_Sec = 99;
	
	Byte2Str(PlayBack_Current_Time_Min,str);
	
	Fun_WriteStr(8,6,str,0);
	
	Byte2Str(PlayBack_Current_Time_Sec,str);
	
	Fun_WriteStr(32,6,str,0);
}

void PlayBack_History_Time_Dispaly(void)
{
	u8 str[3];
	
	if(PlaybackHistoryYear>99)
		PlaybackHistoryYear=99;
	if(PlaybackHistoryMon>99)
		PlaybackHistoryMon=99;
	if(PlaybackHistoryDay>99)
		PlaybackHistoryDay=99;
	if(PlaybackHistoryHou>99)
		PlaybackHistoryHou=99;
	if(PlaybackHistoryMin>99)
		PlaybackHistoryMin=99;
	if(PlaybackHistorySec>99)
		PlaybackHistorySec=99;
	
	if(PlaybackHistoryYear)
		Byte2Str(20,str);
	else
		Byte2Str(0,str);
	
	Fun_WriteStr(8,0,str,0);

	Byte2Str(PlaybackHistoryYear,str);
	
	Fun_WriteStr(24,0,str,0);
	
	Byte2Str(PlaybackHistoryMon,str);
	
	Fun_WriteStr(55,0,str,0);

	Byte2Str(PlaybackHistoryDay,str);
	
	Fun_WriteStr(86,0,str,0);
	
	Byte2Str(PlaybackHistoryHou,str);
	
	Fun_WriteStr(16,2,str,0);
	
	Byte2Str(PlaybackHistoryMin,str);
	
	Fun_WriteStr(47,2,str,0);
	
	Byte2Str(PlaybackHistorySec,str);
	
	Fun_WriteStr(78,2,str,0);

}
void Emergency_broadcast_Current_Time_Display(void)
{
	u8 str[3];

	if(Emergency_Current_Time_Min > 99)
		Emergency_Current_Time_Min = 99;
	
	if(Emergency_Current_Time_Sec>99)
		Emergency_Current_Time_Sec = 99;
	
	Byte2Str(Emergency_Current_Time_Min,str);
	
	Fun_WriteStr(40,6,str,0);
	
	Byte2Str(Emergency_Current_Time_Sec,str);
	
	Fun_WriteStr(64,6,str,0);
}

void Record_Current_Time_Display(void)
{
	u8 str[3];	
	
	if(Record_Current_Time_Min == 0xFF)
	{
		Fun_WriteStr(32,4,"录制失败",0);
	}
	else if(Record_Current_Time_Min == 0xFE)
	{
		Fun_WriteStr(32,4,"录制超时",0);
	}
	else
	{
		if(Record_Current_Time_Min > 99)
			Record_Current_Time_Min = 99;
		if(Record_Current_Time_Sec > 99)
			Record_Current_Time_Sec = 99;
		
		Byte2Str(Record_Current_Time_Min,str);
		
		Fun_WriteStr(40,4,str,0);
		
		Byte2Str(Record_Current_Time_Sec,str);
		
		Fun_WriteStr(64,4,str,0);
	}
}

void PlayBack_Del_Current_Num_Display(void)
{
	int i,j;
	u8 str[5];
	
	if(PlayBack_Current_Num>999)
		PlayBack_Current_Num = 999;
	
	Com_itoa(PlayBack_Current_Num,str);
	
	for(i=0;i<3;i++)
		if(!str[i])
			break;
		
	if(i<3)
	{
		for(i--,j=2;i>=0;i--,j--)
			str[j]=str[i];
		for(;j>=0;j--)
			str[j]=0x30;
	}
	str[3] = '/';
	str[4] = 0;
	Fun_WriteStr(32,5,str,0);
}

void PlayBack_Del_Sum_Display(void)
{
	int i,j;
	u8 str[4];
	
	if(PlayBack_Current_Sum>999)
		PlayBack_Current_Sum = 999;
	
	Com_itoa(PlayBack_Current_Sum,str);
	
	for(i=0;i<3;i++)
		if(!str[i])
			break;
		
	if(i<3)
	{
		for(i--,j=2;i>=0;i--,j--)
			str[j]=str[i];
		for(;j>=0;j--)
			str[j]=0x30;
	}
	
	str[3] = 0;
  Fun_WriteStr(64,5,str,0);
}

void FileInputFinish_Display(void)
{
	LcdFill(0x00);
	Fun_WriteStr(16,2,(uint8_t *)("文件导入完成"),0);
	Fun_WriteStr(8,4,(uint8_t *)("请按退出键退出"),0);
}

void DelAllFileFinish_Display(void)
{
	LcdFill(0x00);
	Fun_WriteStr(16,2,(uint8_t *)("文件删除完成"),0);
	Fun_WriteStr(8,4,(uint8_t *)("请按退出键退出"),0);
}

void FactoryCfgFinish_Display(void)
{
	LcdFill(0x00);
	Fun_WriteStr(16,2,(uint8_t *)("出厂设置完成"),0);
	Fun_WriteStr(8,4,(uint8_t *)("请重新启动设备"),0);
}

void Fun_GetCtrlValue(uint8_t wnd)
{
	const struct StructHead_SysWndType *wndtype;
	const struct StructHead_CtrlType *p;
	const struct StructHead_CtrlType *m;
	
	wndtype = &Struct_SystemWnd[wnd];
	p = wndtype->SelfCtrlType;
	m = p + CurWndFocus;
	
	if(CurWndFocus == m->CtrlID)
	{
		switch(m->CtrlType)     
		{
			case DEF_CTRLTEXT:   
			{						
				if(DEF_RecordQuery != WndGrp[WndNum]
					&& DEF_RecordDel != WndGrp[WndNum]
					&& DEF_FileInput != WndGrp[WndNum]
					&& DEF_DelAllFile != WndGrp[WndNum]
					&& DEF_PoweAmpRegis != WndGrp[WndNum]
					&& DEF_PowerAmpCfg != WndGrp[WndNum]
					&& DEF_AddressCfg != WndGrp[WndNum]
					&& DEF_UserPasswordCfg != WndGrp[WndNum]
					&& DEF_GovPasswordCfg != WndGrp[WndNum]
					&& DEF_PasswordSave != WndGrp[WndNum]
					&& DEF_CheckUserPassword != WndGrp[WndNum]
					&& DEF_CheckGovPassword != WndGrp[WndNum]
					&& DEF_TimeCfg != WndGrp[WndNum]
					&& DEF_FactoryConfig != WndGrp[WndNum])
				{
					CurWndFocus++;   
				}				
				WndUpdate = 1;	
				
				if(DEF_RecordQuery == WndGrp[WndNum]) //查询录音时 	
				{
					if(!RecordQueryWnd_Flag)
					{
						RecordQueryWnd_Flag = 1;
						
						Task_Flag=1;
						TX_Task_Command_Flag[11]=1;	 //进入回放				
					}
				}
				else if(DEF_RecordDel == WndGrp[WndNum]) //查询删除时			
				{
					if(!RecordDelWnd_Flag)
					{
						RecordDelWnd_Flag = 1;
						
						Task_Flag=1;
						TX_Task_Command_Flag[11]=1;	 //进入回放				
					}
				}
				else if(DEF_FileInput == WndGrp[WndNum]) //文件导入界面		
				{
					if(!FileInputWnd_Flag)
					{
						FileInputWnd_Flag = 1;	
				
						Task_Flag=1;
						TX_Task_Command_Flag[24]=1;	//文件导入	
					}
				}					
				else if(DEF_DelAllFile == WndGrp[WndNum]) //删除全部文件界面		
				{
					if(!DelAllFileWnd_Flag)
					{
						DelAllFileWnd_Flag = 1;	
						
						DelAllFile_Flag=1;
				
						Task_Flag=1;
						TX_Task_Command_Flag[84]=1;	//删除全部文件	
					}
				}					
				else if(DEF_PoweAmpRegis == WndGrp[WndNum]) //功放注册
				{
					if(!PowerAmpRegWnd_Flag)
					{
						PowerAmp_SetStartRegFlag();
						
						PowerAmpRegWnd_Flag = 1;
					}
					else if(PowerAmpRegWnd_Flag == 1)
					{
						if(PowerAmp_IsRegCompleted())
						{
							PowerAmpRegWnd_Flag = 2;
							ui_Register_Finshed();
						}
					}
				}
				else if(DEF_PowerAmpCfg == WndGrp[WndNum]) //功放配置	
				{
					if(!CfgPowerAmpWnd_Flag)
					{						
						CfgPowerAmpWnd_Flag = 1;
				
						CfgPowerAmp_Dispaly_Function();
					}
					
					if(!Cfg_PowAmp1to10_InitSta_OneEnter_Flag)
					{
						Cfg_PowAmp1to10_InitSta_OneEnter_Flag = 1;
						
						if(CfgPowerAmpNum >= 1 && CfgPowerAmpNum <= 10)
						{
							Edit_PowAmpCfg(CfgPowerAmpNum-1);
							Cfg_PowAmp_AreaLed();
						}
					}			
				}										
				else if(DEF_AddressCfg == WndGrp[WndNum]) //本机地址配置			
				{
					if(!CfgNativeAddressWnd_Flag)
					{
						CfgNativeAddressWnd_Flag = 1;

						Read_Main_Adress();
						LoadNativeAddress();
						CfgNativeAddress_Display();
					}
				}					
				else if(DEF_UserPasswordCfg == WndGrp[WndNum]) //用户密码设置界面		
				{
					if(!UserPasswordWnd_Flag)
					{
						UserPasswordWnd_Flag = 1;
						
						Load_PasswordBuf(0);
						Password_Display();
					}
				}					
				else if(DEF_GovPasswordCfg == WndGrp[WndNum]) //管理密码设置界面		
				{
					if(!GovPasswordWnd_Flag)
					{
						GovPasswordWnd_Flag = 1;
						
						Load_PasswordBuf(1);
						Password_Display();
					}
				}
				else if(DEF_PasswordSave == WndGrp[WndNum]) //密码保存确认界面		
				{
					if(!PasswordSaveWnd_Flag)
					{
						PasswordSaveWnd_Flag = 1;
					}
				}
				else if(DEF_CheckUserPassword == WndGrp[WndNum])
				{
					if(!CheckUserPasswordWnd_Flag)
					{
						CheckUserPasswordWnd_Flag = 1;
						
						Load_PasswordBuf(2);
						
						Password_Display();
					}
				}
				else if(DEF_CheckGovPassword == WndGrp[WndNum])
				{
					if(!CheckGovPasswordWnd_Flag)
					{
						CheckGovPasswordWnd_Flag = 1;
						
						Load_PasswordBuf(2);
						
						Password_Display();
					}
				}
				else if(DEF_TimeCfg == WndGrp[WndNum])    		//时间调节界面
				{
					if(!Time_Adjust_Flag)
					{
						Time_Adjust_Flag = 1;
						
						LoadTimeStore();
						
						DateTime_Display();
					}
				}
				else if(DEF_FactoryConfig == WndGrp[WndNum])    		//出厂设置界面
				{
					if(!FactoryCfgWnd_Flag)
					{
						FactoryCfgWnd_Flag = 1;
						
						DelAllFile_Flag=1;
						Task_Flag=1;
						TX_Task_Command_Flag[84]=1;	//删除全部文件	
						
						config_RemoveAllCfg();
					}
				}

				
				if(SystemKeyValue == KEY_UP_Short)
				{
					if(DEF_RecordQuery == WndGrp[WndNum])			//录音查询界面
					{
						if(PlayBack_Current_Num != 0 && PlayBack_Current_Sum != 0)
						{
							Task_Flag=1;
							TX_Task_Command_Flag[17]=1;				//发送回放上一曲命令							
						}
					}
					else if(DEF_RecordDel == WndGrp[WndNum])			//录音删除界面		
					{
						if(PlayBack_Current_Num != 0 && PlayBack_Current_Sum != 0)
						{
							Task_Flag=1;
							TX_Task_Command_Flag[17]=1;			//发送回放上一曲命令		
						}				
					}						
					else if(DEF_AddressCfg == WndGrp[WndNum])			//配置本机地址界面
					{
						Reduce_CfgNativeAddress();	
					}	
					else if(DEF_PowerAmpCfg == WndGrp[WndNum])    		//功放配置界面   KEY_UP_Short
					{
						Reduce_CfgPowerAmp_Function();
						Cfg_PowAmp1to10_InitSta_OneEnter_Flag=0;
					}	
					else if(DEF_UserPasswordCfg == WndGrp[WndNum]) //用户密码设置界面		
					{
						Password_Dec_Display();
					}
					else if(DEF_GovPasswordCfg == WndGrp[WndNum]) //管理密码设置界面		
					{
						Password_Dec_Display();
					}
					else if(DEF_CheckUserPassword == WndGrp[WndNum]) //核对用户密码界面		
					{
						Password_Dec_Display();
					}
					else if(DEF_CheckGovPassword == WndGrp[WndNum]) //核对管理密码界面		
					{
						Password_Dec_Display();
					}
					else if(DEF_TimeCfg == WndGrp[WndNum]) //时间调节界面
					{
						Reduce_Time_Adjust_Function();	
						DateTime_Display();
					}	
				} 
				else if(SystemKeyValue == KEY_DOWN_Short)
				{
					if(DEF_RecordQuery == WndGrp[WndNum])			//录音查询界面
					{
						if(PlayBack_Current_Num != 0 && PlayBack_Current_Sum != 0)
						{
							Task_Flag=1;
							TX_Task_Command_Flag[19]=1;				//发送回放下一曲命令				
						}
					}
					else if(DEF_RecordDel == WndGrp[WndNum])			//录音删除界面
					{
						if(PlayBack_Current_Num != 0 && PlayBack_Current_Sum != 0)
						{
							Task_Flag=1;
							TX_Task_Command_Flag[19]=1;			//发送回放下一曲命令		
						}				
					}
					else if(DEF_AddressCfg == WndGrp[WndNum])			//配置本机地址界面
					{
						Add_CfgNativeAddress();	
					}
					else if(DEF_PowerAmpCfg == WndGrp[WndNum])     	//功放配置界面   KEY_DOWN_Short
					{
						Add_CfgPowerAmp_Function();	
						Cfg_PowAmp1to10_InitSta_OneEnter_Flag=0;						
					}
					else if(DEF_UserPasswordCfg == WndGrp[WndNum]) //用户密码设置界面		
					{
						Password_Inc_Display();
					}
					else if(DEF_GovPasswordCfg == WndGrp[WndNum]) //管理密码设置界面		
					{
						Password_Inc_Display();
					}
					else if(DEF_CheckUserPassword == WndGrp[WndNum]) //核对用户密码界面		
					{
						Password_Inc_Display();
					}
					else if(DEF_CheckGovPassword == WndGrp[WndNum]) //核对管理密码界面		
					{
						Password_Inc_Display();
					}
					else if(DEF_TimeCfg == WndGrp[WndNum])    //时间调节界面
					{
						Add_Time_Adjust_Function();
						
						DateTime_Display();	
					}		

				}
				else if(SystemKeyValue == KEY_Play)
				{
					if(DEF_RecordQuery == WndGrp[WndNum])			//录音查询界面
					{
						if(PlayBack_Current_Num != 0 && PlayBack_Current_Sum != 0)
						{
							Task_Flag=1;
							TX_Task_Command_Flag[13]=1;				//发送 回放播放命令
						}
					}
					else if(DEF_RecordDel == WndGrp[WndNum])			//录音删除界面
					{
						if(!DelFile_Flag && PlayBack_Current_Num != 0 && PlayBack_Current_Sum != 0)
						{
							DelFile_Flag = 1;
							
							Task_Flag=1;
							TX_Task_Command_Flag[18]=1;			//发送录音删除命令	
						}				
					}
					else if(DEF_AddressCfg == WndGrp[WndNum])			//配置本机地址界面
					{
						if(CfgNativeAddressWnd_Flag<4)
							CfgNativeAddressWnd_Flag++;
						else
							CfgNativeAddressWnd_Flag = 1;
						
						CfgNativeAddress_Display();
						
						if(CfgNativeAddressWnd_Flag == 4)
						{
							if(StoreNativeAddress())
							{
								Write_Main_Adress();//保存新本机地址	
								
								txandre_AddSetAddrTask();//下传配置本机地址命令
							}
						}
					}
					else if(DEF_PowerAmpCfg == WndGrp[WndNum])   		//功放配置界面  
					{				
						if(CfgPowerAmpNum > 0 && CfgPowerAmpNum < 11)
						{
							Write_PowAmpCfg(CfgPowerAmpNum-1);
						}
					}		
					else if(DEF_UserPasswordCfg == WndGrp[WndNum]) //用户密码设置界面		
					{
						if(UserPasswordWnd_Flag<4)
							UserPasswordWnd_Flag++;
						else
							UserPasswordWnd_Flag = 1;
						
						Password_Display();
					}
					else if(DEF_GovPasswordCfg == WndGrp[WndNum]) //管理密码设置界面		
					{
						if(GovPasswordWnd_Flag<4)
							GovPasswordWnd_Flag++;
						else
							GovPasswordWnd_Flag = 1;
						
						Password_Display();
					}
					else if(DEF_PasswordSave == WndGrp[WndNum]) //密码保存确认界面		
					{
						if(NextWnd_Flag == 6)
							Write_Password(0);
						else if(NextWnd_Flag == 7)
							Write_Password(1);
						
						PasswordSaveWnd_Flag = 0;
						Fun_WndExitDeal();
					}
					else if(DEF_CheckUserPassword == WndGrp[WndNum]) //核对用户密码界面		
					{
						if(Check_Password(0))
						{
							CheckUserPasswordWnd_Flag = 0;
							Fun_WndExitDeal();
							
							SelfCheck(1);

							return;
						}
						else if(CheckUserPasswordWnd_Flag<4)
							CheckUserPasswordWnd_Flag++;
						else
							CheckUserPasswordWnd_Flag = 1;
						
						Password_Display();
					}
					else if(DEF_CheckGovPassword == WndGrp[WndNum]) //核对管理密码界面		
					{
						if(Check_Password(1) || Check_SysPassword())
						{
							CheckGovPasswordWnd_Flag = 0;
							Fun_WndExitDeal();
							
							if(NextWnd_Flag == 1)    
								Fun_WndPreDeal(DEF_RecordDel);
							else if(NextWnd_Flag == 2) 
							  Fun_WndPreDeal(DEF_FileInput);
							else if(NextWnd_Flag == 3) 
							  Fun_WndPreDeal(DEF_DelAllFile);
							else if(NextWnd_Flag == 4)  
								Fun_WndPreDeal(DEF_SystemConfig);
							else if(NextWnd_Flag == 5)
								Fun_WndPreDeal(DEF_FactoryConfig);
							return;
						}
						else if(CheckGovPasswordWnd_Flag<4)
							CheckGovPasswordWnd_Flag++;
						else
							CheckGovPasswordWnd_Flag = 1;
						
						Password_Display();
					}
					else if(DEF_TimeCfg == WndGrp[WndNum])    		//时间调节界面
					{
						if(Time_Adjust_Flag<7)
							Time_Adjust_Flag++;
						else
							Time_Adjust_Flag = 1;
						
						DateTime_Display();
						
						if(Time_Adjust_Flag == 7)
						{
							StoreTimeStore();
							
							Task_Flag=1;
							TX_Task_Command_Flag[85]=1;//设置系统时间
						}
					}	
				}				
				else if(SystemKeyValue == KEY_Stop)  
				{		
					if(DEF_RecordQuery == WndGrp[WndNum])
					{
						if(PlayBack_Play_Flag)
						{					
							Task_Flag=1;
							TX_Task_Command_Flag[14]=1;//发送 回放停止播放命令
							return;
						}
						else
						{
							RecordQueryWnd_Flag=0;
							Task_Flag=1;
							TX_Task_Command_Flag[12]=1;//发送 退出回放命令
						}
					}
					else if(DEF_RecordDel == WndGrp[WndNum])
					{
						RecordDelWnd_Flag=0;
						Task_Flag=1;
						TX_Task_Command_Flag[12]=1;//发送 退出回放命令
					}
					else if(DEF_FileInput == WndGrp[WndNum])
					{
						if(FileInputFinish_Flag==1)
						{
							FileInputFinish_Flag=0;
							FileInputWnd_Flag=0;
							Fun_WndExitDeal();	
						}
					}
					else if(DEF_DelAllFile == WndGrp[WndNum])
					{
						if(!DelAllFile_Flag)
						{
							DelAllFileWnd_Flag=0;
							Fun_WndExitDeal();	
						}
					}
					else if(DEF_PoweAmpRegis == WndGrp[WndNum]) //功放注册
					{
						if(PowerAmpRegWnd_Flag == 2)
						{
							PowerAmpRegWnd_Flag = 0;
							Fun_WndExitDeal();	
						}
					}
					else if(DEF_PowerAmpCfg == WndGrp[WndNum])//功放配置界面  
					{	
						Clear_PowAmpCfg_Var();	
					}
					else if(DEF_AddressCfg == WndGrp[WndNum])//配置本机地址界面
					{		
						CfgNativeAddressWnd_Flag=0;
					}		
					else if(DEF_UserPasswordCfg == WndGrp[WndNum])	//用户密码
					{
						UserPasswordWnd_Flag = 0;
						if(Password_IsUpdated(0))
						{
							Fun_WndExitDeal();
							Fun_WndPreDeal(DEF_PasswordSave);
							NextWnd_Flag = 6;
						}
					}	
					else if(DEF_GovPasswordCfg == WndGrp[WndNum]) //管理密码
					{
						GovPasswordWnd_Flag = 0;
						if(Password_IsUpdated(1))
						{
							Fun_WndExitDeal();
							Fun_WndPreDeal(DEF_PasswordSave);
							NextWnd_Flag = 7;
						}
					}
					else if(DEF_PasswordSave == WndGrp[WndNum]) //密码保存确认界面		
					{
						UserPasswordWnd_Flag=0;
						GovPasswordWnd_Flag=0;	
						PasswordSaveWnd_Flag = 0;
						Fun_WndExitDeal();
						return;
					}
					else if(DEF_CheckUserPassword == WndGrp[WndNum]) //核对管理密码界面		
					{
						CheckUserPasswordWnd_Flag = 0;
					}
					else if(DEF_CheckGovPassword == WndGrp[WndNum]) //核对管理密码界面		
					{
						CheckGovPasswordWnd_Flag = 0;
					}
					else if(DEF_TimeCfg == WndGrp[WndNum])		 //时间调节界面
					{					
						Time_Adjust_Flag=0;
					}																
					else if(DEF_FactoryConfig == WndGrp[WndNum])    		//出厂设置界面
					{
						if(!DelAllFile_Flag)
						{
							FactoryCfgWnd_Flag=0;
							Fun_WndExitDeal();	
						}
					}
					
					if(DEF_FileInput != WndGrp[WndNum] 
						&& DEF_DelAllFile != WndGrp[WndNum] 
						&& DEF_FactoryConfig != WndGrp[WndNum] 
						&& DEF_PoweAmpRegis != WndGrp[WndNum] 
						&& DEF_PasswordSave != WndGrp[WndNum]) //不能直接退出窗口
					{
						Fun_WndExitDeal();	
					}
				}	
				break;								
			}
			case DEF_CTRLBUTTON:    
			case DEF_CTRLRADIO:    
			{
				if(SystemKeyValue == KEY_Play)
				{
					m->CtrlFun();  			
					if(m->CtrlType == DEF_CTRLBUTTON)
					{
						CurWndFocus = 0;
						InputSum = 0;
					}
				}
				else if(SystemKeyValue == KEY_Stop)
				{
					Fun_WndExitDeal();
				}
				else if(SystemKeyValue == KEY_UP_Short)
				{
					if(CurWndFocus > 0)
						CurWndFocus--;      //焦点上移
					else
						CurWndFocus = wndtype->CtrlNum - 1;
					InputSum = 0;
				}
				else if(SystemKeyValue == KEY_DOWN_Short)
				{
					if(CurWndFocus < wndtype->CtrlNum - 1)
						CurWndFocus++;      //焦点下移
					else
						CurWndFocus = 0;
					InputSum = 0;
				}								
				break;
			}
			case DEF_CTRLEDIT:      
			case DEF_CTRLPASSWORD: 
			{
				break;
			}
		}
	}
}

void Fun_WndCtrlDeal(uint8_t wnd)
{
	uint8_t i;
	const struct StructHead_SysWndType *wndtype;
	const struct StructHead_CtrlType *p;
	const struct StructHead_CtrlType *m;
	
	if(!wnd)
		return;

	if((OldWndFocus / 6) != (CurWndFocus / 6))  
		LCD_WriteAll(0);

	wndtype = &Struct_SystemWnd[wnd];
	p = wndtype->SelfCtrlType;
	for(i=0; i<wndtype->CtrlNum; i++)
	{
		m = p + i;
		switch(m->CtrlType)
		{
			case DEF_CTRLTEXT:          
			{
				Fun_WriteStr(m->Ctrlx, m->Ctrly, m->CtrlData, m->CtrlDispMode);
				if(CurWndFocus==wndtype->CtrlNum-1)
					CurWndFocus=0;			
				break;
			}
			case DEF_CTRLEDIT:          
			{
				if(m->CtrlFun)
						m->CtrlFun();
				break;
			}
			case DEF_CTRLPASSWORD:      
                break;
			case DEF_CTRLBUTTON:        
			case DEF_CTRLRADIO:       
			{
				if((CurWndFocus / 6) == (m->CtrlID / 6))
				{
					if(CurWndFocus == m->CtrlID)
							Fun_WriteStr(m->Ctrlx, m->Ctrly, m->CtrlData, 1);
					else
							Fun_WriteStr(m->Ctrlx, m->Ctrly, m->CtrlData, 0);
					
					if(m->CtrlType == DEF_CTRLRADIO)
						m->CtrlFun();
				}
				break;
			}
		}
	}
}

void Fun_WndPreDeal(uint8_t wnd)
{
	WndNum++;
	WndGrp[WndNum] = wnd;
	WndUpdate = 1;
	CurWndFocus = 0;
	LCD_WriteAll(0);
}

void Fun_WndExitDeal(void)
{	
	if(WndNum > 0)
	{
		WndNum--;
	}
	WndUpdate = 1;
	CurWndFocus = 0;
	LCD_WriteAll(0);
}

static void ShowDesktopWnd(void)
{
	DrawDateTimeLine(WndUpdate);
	
	DrawMicStatus();
	
	Show_PowerAmp_Offline(WndUpdate);
	
	if(WndUpdate)
	{
		WndUpdate = 0;

		LCD_WriteLine(4,0);
		LCD_WriteLine(5,0);
		Fun_WriteStr(32,4,(uint8_t *)("工作正常"),0);
		if(MainInfSta_MicOnline_Flag)
			Fun_WriteStr(0,4,(uint8_t *)("MIC"),0);
	
		LCD_WriteLine(6,0);
		LCD_WriteLine(7,0);
		if(MainInfSta_UOnline_Flag)
			Fun_WriteStr(0,6,(uint8_t *)("U"),0);
		Fun_WriteStr(16,6,(uint8_t *)("消防广播系统"),0);
	}
}

void ui_ShowSelfCheckWnd(void)
{
	Fun_WndPreDeal(DEF_CheckUserPassword);
	Fun_WndCtrlDeal(WndGrp[WndNum]);
}

void ui_ShowMainMenu(void)
{
	Fun_WndPreDeal(DEF_MENUWND);   
}

void ui_WndProc(void)
{
	uint8_t wnd;
	
	if(Emergency_Mode || MP3_Mode || line1_Mode || line2_Mode || Record_Flag)
		return;
	
	ui_MenuToMainWnd();
	
	wnd = WndGrp[WndNum];
	
	if(!wnd)
	{
		ShowDesktopWnd();
	}
	else
	{
		if(WndUpdate)     
		{
			WndUpdate = 0;	
			
			Fun_GetCtrlValue(wnd);
			
			SystemKeyValue = 0xFF;
			
			if(RecordQueryWnd_Flag==0
				&& RecordDelWnd_Flag==0
				&& FileInputWnd_Flag==0
				&& DelAllFileWnd_Flag==0
				&& PowerAmpRegWnd_Flag==0
				&& CfgPowerAmpWnd_Flag==0
				&& CfgNativeAddressWnd_Flag==0
				&& UserPasswordWnd_Flag==0
				&& GovPasswordWnd_Flag==0
				&& PasswordSaveWnd_Flag==0
				&& CheckUserPasswordWnd_Flag == 0 
				&& CheckGovPasswordWnd_Flag == 0
				&& Time_Adjust_Flag == 0 
				&& FactoryCfgWnd_Flag == 0) 
				Fun_WndCtrlDeal(WndGrp[WndNum]);
		}
	}
}

void ui_RemoveAllWnd(void)
{
	WndNum=0;
	WndUpdate=1;

	CurWndFocus = 0;
	InputSum = 0;
	
	RecordQueryWnd_Flag=0;
	RecordDelWnd_Flag=0;
	FileInputWnd_Flag=0;
	DelAllFileWnd_Flag=0;
	PowerAmpRegWnd_Flag=0;
	
	CfgPowerAmpWnd_Flag=0;
	CfgNativeAddressWnd_Flag=0;
	UserPasswordWnd_Flag=0;
	GovPasswordWnd_Flag=0;
	PasswordSaveWnd_Flag = 0;
	CheckUserPasswordWnd_Flag = 0;
	CheckGovPasswordWnd_Flag = 0;
	Time_Adjust_Flag = 0;
	FactoryCfgWnd_Flag = 0;
}


void ui_ReDrawWnd(void)
{
	uint8_t wnd;
	
	WndUpdate=1;
	RecordQueryWnd_Flag=0;
	RecordDelWnd_Flag=0;
	FileInputWnd_Flag=0;
	DelAllFileWnd_Flag=0;
	PowerAmpRegWnd_Flag=0;
	
	CfgPowerAmpWnd_Flag=0;
	Cfg_PowAmp1to10_InitSta_OneEnter_Flag = 0;
	CfgNativeAddressWnd_Flag=0;
	UserPasswordWnd_Flag=0;
	GovPasswordWnd_Flag=0;
	PasswordSaveWnd_Flag = 0;
	CheckUserPasswordWnd_Flag = 0;
	CheckGovPasswordWnd_Flag = 0;
	Time_Adjust_Flag = 0;
	FactoryCfgWnd_Flag = 0;
	
	wnd = WndGrp[WndNum];
	Fun_WndExitDeal();
	Fun_WndPreDeal(wnd);
	Fun_WndCtrlDeal(WndGrp[WndNum]);
}

void ui_MenuToMainWnd(void)
{
	if(TimerBglcdFlag && WndNum)
	{
		if(DEF_MENUWND == WndGrp[WndNum] 
			|| DEF_FileOperate == WndGrp[WndNum] 
			|| DEF_SystemConfig == WndGrp[WndNum] 
			|| DEF_PasswordCfg == WndGrp[WndNum])
		{
			WndNum=0;
			WndUpdate=1;

			CurWndFocus = 0;
			InputSum = 0;
		}	
		else 
		{
			int bFlag = 0;
			
			if(DEF_RecordDel  == WndGrp[WndNum])
			{
				RecordDelWnd_Flag=0;
				Task_Flag=1;
				TX_Task_Command_Flag[12]=1;//发送 退出回放命令

				bFlag = 1;
			}
			else if(DEF_FileInput  == WndGrp[WndNum])
			{
				if(FileInputFinish_Flag==1)
				{
					FileInputFinish_Flag=0;
					FileInputWnd_Flag=0;
				}
				else
					return;

				bFlag = 1;
			}
			else if(DEF_DelAllFile == WndGrp[WndNum])
			{
				if(!DelAllFile_Flag)
					DelAllFileWnd_Flag=0;
				else
					return;

				bFlag = 1;
			}
			else if(DEF_PowerAmpCfg == WndGrp[WndNum])
			{
				Clear_PowAmpCfg_Var();
				bFlag = 1;
			}
			else if(DEF_TimeCfg  == WndGrp[WndNum])
			{
				Time_Adjust_Flag=0;
				bFlag = 1;
			}
			else if(DEF_AddressCfg  == WndGrp[WndNum])
			{
				CfgNativeAddressWnd_Flag=0;
				bFlag = 1;
			}
			else if(DEF_UserPasswordCfg  == WndGrp[WndNum])
			{
				UserPasswordWnd_Flag = 0;
				bFlag = 1;
			}
			else if(DEF_GovPasswordCfg  == WndGrp[WndNum])
			{
				GovPasswordWnd_Flag = 0;
				bFlag = 1;
			}
			else if(DEF_PasswordSave  == WndGrp[WndNum])
			{
				PasswordSaveWnd_Flag = 0;
				bFlag = 1;
			}
			else if(DEF_CheckUserPassword  == WndGrp[WndNum])
			{
				CheckUserPasswordWnd_Flag = 0;
				bFlag = 1;
			}
			else if(DEF_CheckGovPassword  == WndGrp[WndNum])
			{
				CheckGovPasswordWnd_Flag = 0;
				bFlag = 1;
			}
			else if(DEF_FactoryConfig  == WndGrp[WndNum])
			{
				if(!DelAllFile_Flag)
					FactoryCfgWnd_Flag=0;
				else
					return;
				bFlag = 1;
			}
			
			if(bFlag)
			{
				WndNum=0;
				WndUpdate=1;

				CurWndFocus = 0;
				InputSum = 0;
			}
		}
	}
}









