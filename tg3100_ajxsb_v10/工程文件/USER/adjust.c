#include "main.h"
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
#include "config.h"
#include "ui_key.h"

//时间配置
uint8_t Time_Storage[14]={1,7,1,1,2,7,2,3,5,9,5,9,2,0};
uint8_t Time_Adjust_Flag;
uint8_t year_qian,year_bai,year_ten,year_ge,month_ten,month_ge,day_ten,day_ge,hour_ten,hour_ge,min_ten,min_ge,sec_ten,sec_ge,year_H,year_L;
uint16_t year;
uint8_t yue,ri,hour,min,sec;

//功放配置
u8 CfgPowerAmp_shi,CfgPowerAmp_ge=1;
u8 CfgPowerAmpNum=1;
/*********************************************************时间配置************************************************/

//x1，y1是字符上半部分的地址  x2，y2是字符下半部分的地址
//char_position是要显示的字符在数组中的位置
//chardisplaybuffer[]  要显示的字符所在的数组
void char_display(u8 x,u8 y,u16 char_position,unsigned char char_display_buffer[][16],u8 type)
{				
	u8 a=0;
	LCD_GotoXY(x,y);	
	if(type==0)
	{
		for(a=0;a<8;a++)
		{
			LCD_WriteData(char_display_buffer[char_position][a]);
		}
	}
	else
	{
		for(a=0;a<8;a++)
		{
			LCD_WriteData(~char_display_buffer[char_position][a]);
		}
	}
	
	LCD_GotoXY(x,y+1);	
	if(type==0)
	{
		for(a=8;a<16;a++)
		{	
			LCD_WriteData(char_display_buffer[char_position][a]);
		}	
	}
	else
	{
		for(a=8;a<16;a++)
		{	
			LCD_WriteData(~char_display_buffer[char_position][a]);
		}	
	}
	
}

void LoadTimeStore(void)
{
	if(mTimeBuffer[0]>99)
		mTimeBuffer[0] = 0;
	if(mTimeBuffer[1]>12)
		mTimeBuffer[1] = 1;
	if(mTimeBuffer[2]>31)
		mTimeBuffer[2] = 1;
	if(mTimeBuffer[3]>23)
		mTimeBuffer[3] = 0;
	if(mTimeBuffer[4]>59)
		mTimeBuffer[4] = 0;
	if(mTimeBuffer[5]>59)
		mTimeBuffer[5] = 0;
	
	//年 低两位
	Time_Storage[0] = mTimeBuffer[0]/10;
	Time_Storage[1] = mTimeBuffer[0]%10;
	
	//月
	Time_Storage[2] = mTimeBuffer[1]/10;
	Time_Storage[3] = mTimeBuffer[1]%10;
	
	//日
	Time_Storage[4] = mTimeBuffer[2]/10;
	Time_Storage[5] = mTimeBuffer[2]%10;
	
	//时	
	Time_Storage[6] = mTimeBuffer[3]/10;
	Time_Storage[7] = mTimeBuffer[3]%10;
	
	//分
	Time_Storage[8] = mTimeBuffer[4]/10;
	Time_Storage[9] = mTimeBuffer[4]%10;
	
	//秒
	Time_Storage[10] = mTimeBuffer[5]/10;
	Time_Storage[11] = mTimeBuffer[5]%10;
}

void StoreTimeStore(void)
{
	year 	= Time_Storage[0]*10+Time_Storage[1];
	yue		=	Time_Storage[2]*10+Time_Storage[3];
	ri		=	Time_Storage[4]*10+Time_Storage[5];
	hour	=	Time_Storage[6]*10+Time_Storage[7];
	min		=	Time_Storage[8]*10+Time_Storage[9];
	sec		=	Time_Storage[10]*10+Time_Storage[11];
}

void DateTime_Display(void)
{
	if(Time_Adjust_Flag == 1)
	{
		char_display(24,4,Time_Storage[0],Ascii_char_display_buffer,1);	//显示年十位
		char_display(32,4,Time_Storage[1],Ascii_char_display_buffer,1);	//显示年个位
	}
	else
	{
		char_display(24,4,Time_Storage[0],Ascii_char_display_buffer,0);	//显示年十位
		char_display(32,4,Time_Storage[1],Ascii_char_display_buffer,0);	//显示年个位
	}
	
	if(Time_Adjust_Flag == 2)
	{
		char_display(56,4,Time_Storage[2],Ascii_char_display_buffer,1);	//显示月十位
		char_display(64,4,Time_Storage[3],Ascii_char_display_buffer,1);	//显示月个位
	}
	else
	{
		char_display(56,4,Time_Storage[2],Ascii_char_display_buffer,0);	//显示月十位
		char_display(64,4,Time_Storage[3],Ascii_char_display_buffer,0);	//显示月个位
	}
		
	if(Time_Adjust_Flag == 3)
	{
		char_display(88,4,Time_Storage[4],Ascii_char_display_buffer,1);	//显示日十位
		char_display(96,4,Time_Storage[5],Ascii_char_display_buffer,1);	//显示日个位
	}
	else
	{
		char_display(88,4,Time_Storage[4],Ascii_char_display_buffer,0);	//显示日十位
		char_display(96,4,Time_Storage[5],Ascii_char_display_buffer,0);	//显示日个位
	}

	if(Time_Adjust_Flag == 4)
	{
		char_display(16,6,Time_Storage[6],Ascii_char_display_buffer,1);	//显示时十位
		char_display(24,6,Time_Storage[7],Ascii_char_display_buffer,1);	//显示时个位
	}
	else
	{
		char_display(16,6,Time_Storage[6],Ascii_char_display_buffer,0);	//显示时十位
		char_display(24,6,Time_Storage[7],Ascii_char_display_buffer,0);	//显示时个位
	}
		
	if(Time_Adjust_Flag == 5)
	{
		char_display(48,6,Time_Storage[8],Ascii_char_display_buffer,1);	//显示分十位
		char_display(56,6,Time_Storage[9],Ascii_char_display_buffer,1);	//显示分个位
	}
	else
	{
		char_display(48,6,Time_Storage[8],Ascii_char_display_buffer,0);	//显示分十位
		char_display(56,6,Time_Storage[9],Ascii_char_display_buffer,0);	//显示分个位
	}
		
	if(Time_Adjust_Flag == 6)
	{
		char_display(80,6,Time_Storage[10],Ascii_char_display_buffer,1);	//显示秒十位
		char_display(88,6,Time_Storage[11],Ascii_char_display_buffer,1);	//显示秒个位
	}
	else
	{
		char_display(80,6,Time_Storage[10],Ascii_char_display_buffer,0);	//显示秒十位
		char_display(88,6,Time_Storage[11],Ascii_char_display_buffer,0);	//显示秒个位
	}
}

void Add_Time_Adjust_Function(void)
{
	int nYear,nMon;
	
	if(Time_Adjust_Flag == 1)//加年低两位	
	{
		Time_Storage[1]++;
		if(Time_Storage[1]==10)
		{
			Time_Storage[1]=0;
			Time_Storage[0]++;
			if(Time_Storage[0]==10)
			{
				Time_Storage[0]=0;
			}
		}
		nYear=10*Time_Storage[0]+Time_Storage[1];
	} 
	else if(Time_Adjust_Flag == 2)//加月	
	{
		Time_Storage[3]++;					  
		if(Time_Storage[2]==1&&Time_Storage[3]==3)
		{
			Time_Storage[3]=1;
			Time_Storage[2]=0;
		}
		if(Time_Storage[3]==10)
		{
			Time_Storage[3]=0;
			Time_Storage[2]++;
		}
		nMon=10*Time_Storage[2]+Time_Storage[3];	
	} 		
	else if(Time_Adjust_Flag == 3)//加日	
	{
		Time_Storage[5]++;
		switch(nMon)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
					if(Time_Storage[4]==3&&Time_Storage[5]==2)
					{
						Time_Storage[5]=1;
						Time_Storage[4]=0;
					}
					if(Time_Storage[5]==10)
					{
						Time_Storage[5]=0;
						Time_Storage[4]++;
					}
			break;
			case 4:
			case 6:
			case 9:
			case 11:
					if(Time_Storage[4]==3&&Time_Storage[5]==1)
					{
						Time_Storage[5]=1;
						Time_Storage[4]=0;
					}
					if(Time_Storage[5]==10)
					{
						Time_Storage[5]=0;
						Time_Storage[4]++;
					}
			break;
			case 2:
					if(nYear%4) //平年
					{
						if(Time_Storage[4]==2 && Time_Storage[5]==9)
						{
							Time_Storage[5]=1;
							Time_Storage[4]=0;
						}
						if(Time_Storage[5]==10)
						{
							Time_Storage[5]=0;
							Time_Storage[4]++;
						}
					}
					else //闰年
					{
						if(Time_Storage[5]==10)
						{
							Time_Storage[5]=0;
							Time_Storage[4]++;
						}
						if(Time_Storage[4]==3)
						{
							Time_Storage[5]=1;
							Time_Storage[4]=0;
						}
					}	
			break;
		}					  
	}
	else if(Time_Adjust_Flag == 4)//加时
	{
		Time_Storage[7]++;					  
		if(Time_Storage[6]==2 && Time_Storage[7]==4)
		{
			Time_Storage[7]=0;
			Time_Storage[6]=0;
		}
			if(Time_Storage[7]==10)
			{
				Time_Storage[7]=0;
				Time_Storage[6]++;
			}	
	}	
	else if(Time_Adjust_Flag == 5)//加分
	{
		Time_Storage[9]++;					  
		if(Time_Storage[9]==10)
		{
			Time_Storage[9]=0;
			Time_Storage[8]++;
		}
		if(Time_Storage[8]==6)
		{
			Time_Storage[8]=0;
		}	
	}	 
	else if(Time_Adjust_Flag == 6)//加秒
	{
		Time_Storage[11]++;					  
		if(Time_Storage[11]==10)
		{
			Time_Storage[11]=0;
			Time_Storage[10]++;
		}
		if(Time_Storage[10]==6)
		{
			Time_Storage[10]=0;
		}	
	}	 
}

void Reduce_Time_Adjust_Function(void)
{
	int nYear,nMon;
	
	if(Time_Adjust_Flag == 1) //减年低两位	
	{	
		if(Time_Storage[1]==0)
		{
			Time_Storage[1]=10;
			if(Time_Storage[0]==0)
			{
				Time_Storage[0]=10;
			}	
			Time_Storage[0]--;			
		}
		Time_Storage[1]--;
		nYear=10*Time_Storage[0]+Time_Storage[1];
	} 
	else if(Time_Adjust_Flag == 2) //减月	
	{	
		if((Time_Storage[3]==1)&&(Time_Storage[2]==0))
		{
			Time_Storage[3]=3;
			Time_Storage[2]=1;
		}
		if((Time_Storage[3]==0)&&(Time_Storage[2]==1))
		{
			Time_Storage[3]=10;
			Time_Storage[2]--;
		}
		Time_Storage[3]--;
		nMon=10*Time_Storage[2]+Time_Storage[3];
	} 
	else if(Time_Adjust_Flag == 3) //减日
	{	
		switch(nMon)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
					if((Time_Storage[5]==1)&&(Time_Storage[4]==0))
					{
						Time_Storage[5]=2;
						Time_Storage[4]=3;
					}
					if(((Time_Storage[5]==0)&&(Time_Storage[4]==1))||((Time_Storage[5]==0)&&(Time_Storage[4]==2))||((Time_Storage[5]==0)&&(Time_Storage[4]==3)))
					{
						Time_Storage[5]=10;
						Time_Storage[4]--;
					}
						Time_Storage[5]--;
			break;
			case 4:
			case 6:
			case 9:
			case 11:
					if((Time_Storage[5]==1)&&(Time_Storage[4]==0))
					{
						Time_Storage[5]=1;
						Time_Storage[4]=3;
					}
					if(((Time_Storage[5]==0)&&(Time_Storage[4]==1))||((Time_Storage[5]==0)&&(Time_Storage[4]==2))||((Time_Storage[5]==0)&&(Time_Storage[4]==3)))
					{
						Time_Storage[5]=10;
						Time_Storage[4]--;
					}
					Time_Storage[5]--;
			break;
			case 2:
					if(nYear%4) //平年
					{
						if((Time_Storage[5]==1)&&(Time_Storage[4]==0))
						{
							Time_Storage[5]=9;
							Time_Storage[4]=2;
						}
						if(((Time_Storage[5]==0)&&(Time_Storage[4]==1))||((Time_Storage[5]==0)&&(Time_Storage[4]==2)))
						{
							Time_Storage[5]=10;
							Time_Storage[4]--;
						}
						Time_Storage[5]--;
					}
					else	//闰年
					{
						if((Time_Storage[5]==1)&&(Time_Storage[4]==0))
						{
							Time_Storage[5]=0;
							Time_Storage[4]=3;
						}
						if(((Time_Storage[5]==0)&&(Time_Storage[4]==1))||((Time_Storage[5]==0)&&(Time_Storage[4]==2))||((Time_Storage[5]==0)&&(Time_Storage[4]==3)))
						{
							Time_Storage[5]=10;
							Time_Storage[4]--;
						}
						Time_Storage[5]--;
					}	
			break;
		}
	}	
	else if(Time_Adjust_Flag == 4) //减时
	{	
		if((Time_Storage[7]==0)&&(Time_Storage[6]==0))
		{
			Time_Storage[7]=4;
			Time_Storage[6]=2;
		}
		if(((Time_Storage[7]==0)&&(Time_Storage[6]==1))||((Time_Storage[7]==0)&&(Time_Storage[6]==2)))
		{
			Time_Storage[7]=10;
			Time_Storage[6]--;
		}
		Time_Storage[7]--;
	}	
	else if(Time_Adjust_Flag == 5) //减分
	{	
		if(Time_Storage[9]==0)
		{
			Time_Storage[9]=10;
			if(Time_Storage[8]==0)
			{
				Time_Storage[8]=6;
			}
			Time_Storage[8]--;
		}
		Time_Storage[9]--;	
	}
	else if(Time_Adjust_Flag == 6) //减秒
	{	
		if(Time_Storage[11]==0)
		{
			Time_Storage[11]=10;
			if(Time_Storage[10]==0)
			{
				Time_Storage[10]=6;
			}
			Time_Storage[10]--;
		}
		Time_Storage[11]--;	
	}
}

static u8 NativeAddress[3];
void LoadNativeAddress(void)
{
	int nIndex;
	for(nIndex=0;nIndex<3;nIndex++)
		NativeAddress[nIndex] = Main_Adress_ReadFlash_Buffer[nIndex];
}

int StoreNativeAddress(void)
{
	int nIndex,bFlag;
	bFlag = 0;
	for(nIndex=0;nIndex<3;nIndex++)
	{
		if(Main_Adress_ReadFlash_Buffer[nIndex] != NativeAddress[nIndex])
		{
			Main_Adress_ReadFlash_Buffer[nIndex] = NativeAddress[nIndex];
			bFlag = 1;
		}
	}	
	return bFlag;
}

void CfgNativeAddress_Display(void)
{
	if(CfgNativeAddressWnd_Flag==1)
		char_display(68,3,NativeAddress[0],Ascii_char_display_buffer,1);//显示百位
	else
		char_display(68,3,NativeAddress[0],Ascii_char_display_buffer,0);	//显示百位
	
	if(CfgNativeAddressWnd_Flag==2)
		char_display(76,3,NativeAddress[1],Ascii_char_display_buffer,1);	//显示十位
	else
		char_display(76,3,NativeAddress[1],Ascii_char_display_buffer,0);	//显示十位
	
	if(CfgNativeAddressWnd_Flag==3)
		char_display(84,3,NativeAddress[2],Ascii_char_display_buffer,1);	//个位
	else
		char_display(84,3,NativeAddress[2],Ascii_char_display_buffer,0);	//个位
}

void Add_CfgNativeAddress(void)
{
	int nPos;
	if(CfgNativeAddressWnd_Flag == 1)//加百位
		nPos = 0;
	else if(CfgNativeAddressWnd_Flag==2)//加十位
		nPos = 1;
	else if(CfgNativeAddressWnd_Flag==3)//加个位
		nPos = 2;	
	else if(CfgNativeAddressWnd_Flag==4)//
		return;
	
	NativeAddress[nPos]++;
	if(NativeAddress[nPos]>9)
		NativeAddress[nPos]=0;
	
	CfgNativeAddress_Display();
}

void Reduce_CfgNativeAddress(void)
{	
	int nPos;
	if(CfgNativeAddressWnd_Flag == 1)//加百位
		nPos = 0;
	else if(CfgNativeAddressWnd_Flag==2)//加十位
		nPos = 1;
	else if(CfgNativeAddressWnd_Flag==3)//加个位
		nPos = 2;	
	else if(CfgNativeAddressWnd_Flag==4)//
		return;
	
	if(NativeAddress[nPos])
		NativeAddress[nPos]--;
	else
		NativeAddress[nPos]=9;
	
	CfgNativeAddress_Display();
}	

void Password_Display(void)
{
	if(UserPasswordWnd_Flag == 1 || GovPasswordWnd_Flag == 1 || CheckGovPasswordWnd_Flag ==1 || CheckUserPasswordWnd_Flag ==1)
		char_display(48,4,Edit_Password_Buffer[0],Ascii_char_display_buffer,1);
	else
		char_display(48,4,10,Ascii_char_display_buffer,0);
	
	if(UserPasswordWnd_Flag == 2 || GovPasswordWnd_Flag == 2 || CheckGovPasswordWnd_Flag ==2 || CheckUserPasswordWnd_Flag ==2)
		char_display(56,4,Edit_Password_Buffer[1],Ascii_char_display_buffer,1);	//显示十位
	else
		char_display(56,4,10,Ascii_char_display_buffer,0);	//显示十位
	
	if(UserPasswordWnd_Flag == 3 || GovPasswordWnd_Flag == 3 || CheckGovPasswordWnd_Flag ==3 || CheckUserPasswordWnd_Flag ==3)
		char_display(64,4,Edit_Password_Buffer[2],Ascii_char_display_buffer,1);	//个位
	else
		char_display(64,4,10,Ascii_char_display_buffer,0);	//个位
	
	if(UserPasswordWnd_Flag == 4 || GovPasswordWnd_Flag == 4 || CheckGovPasswordWnd_Flag ==4 || CheckUserPasswordWnd_Flag ==4)
		char_display(72,4,Edit_Password_Buffer[3],Ascii_char_display_buffer,1);	//个位
	else
		char_display(72,4,10,Ascii_char_display_buffer,0);	//个位
}

void Password_Inc_Display(void)
{
	int nPos;
	
	if(UserPasswordWnd_Flag == 1 || GovPasswordWnd_Flag == 1 || CheckGovPasswordWnd_Flag ==1 || CheckUserPasswordWnd_Flag ==1)
		nPos = 0;
	else if(UserPasswordWnd_Flag == 2 || GovPasswordWnd_Flag == 2 || CheckGovPasswordWnd_Flag ==2 || CheckUserPasswordWnd_Flag ==2)
		nPos = 1;
	else if(UserPasswordWnd_Flag == 3 || GovPasswordWnd_Flag == 3 || CheckGovPasswordWnd_Flag ==3 || CheckUserPasswordWnd_Flag ==3)
		nPos = 2;
	else if(UserPasswordWnd_Flag == 4 || GovPasswordWnd_Flag == 4 || CheckGovPasswordWnd_Flag ==4 || CheckUserPasswordWnd_Flag ==4)
		nPos = 3;
	else 
		return;
		
	Edit_Password_Buffer[nPos]++;
	if(Edit_Password_Buffer[nPos] > 9)
		Edit_Password_Buffer[nPos] = 0;
	
	Password_Display();
}

void Password_Dec_Display(void)
{
	int nPos;
	
	if(UserPasswordWnd_Flag == 1 || GovPasswordWnd_Flag == 1 || CheckGovPasswordWnd_Flag ==1 || CheckUserPasswordWnd_Flag ==1)
		nPos = 0;
	else if(UserPasswordWnd_Flag == 2 || GovPasswordWnd_Flag == 2 || CheckGovPasswordWnd_Flag ==2 || CheckUserPasswordWnd_Flag ==2)
		nPos = 1;
	else if(UserPasswordWnd_Flag == 3 || GovPasswordWnd_Flag == 3 || CheckGovPasswordWnd_Flag ==3 || CheckUserPasswordWnd_Flag ==3)
		nPos = 2;
	else if(UserPasswordWnd_Flag == 4 || GovPasswordWnd_Flag == 4 || CheckGovPasswordWnd_Flag ==4 || CheckUserPasswordWnd_Flag ==4)
		nPos = 3;
	else 
		return;
		
	if(Edit_Password_Buffer[nPos])
		Edit_Password_Buffer[nPos]--;
	else
		Edit_Password_Buffer[nPos] = 9;
	
	Password_Display();
}

/*********************************************************功放配置************************************************/
void Add_CfgPowerAmp_Function(void)
{
	CfgPowerAmp_ge++;					  
	if(CfgPowerAmp_ge==10)
	{
		CfgPowerAmp_ge=0;
		CfgPowerAmp_shi++;
	}
	if(CfgPowerAmp_shi==1&&CfgPowerAmp_ge==1)
	{
		CfgPowerAmp_shi=0;
		CfgPowerAmp_ge=1;
	}

	CfgPowerAmp_Dispaly_Function();
	
	CfgPowerAmpNum=10*CfgPowerAmp_shi+CfgPowerAmp_ge;
}		

void Reduce_CfgPowerAmp_Function(void)
{		
	if((CfgPowerAmp_ge==1)&&(CfgPowerAmp_shi==0))
	{
		CfgPowerAmp_ge=1;
		CfgPowerAmp_shi=1;
	}
	if(((CfgPowerAmp_ge==0)&&(CfgPowerAmp_shi==1)))
	{
		CfgPowerAmp_ge=10;
		CfgPowerAmp_shi--;
	}
	CfgPowerAmp_ge--;

	CfgPowerAmp_Dispaly_Function();

	CfgPowerAmpNum=10*CfgPowerAmp_shi+CfgPowerAmp_ge;
}

void CfgPowerAmp_Dispaly_Function(void)
{
	char_display(30,0,CfgPowerAmp_shi,Ascii_char_display_buffer,1);//功放地址 十位
	char_display(38,0,CfgPowerAmp_ge,Ascii_char_display_buffer,1);//功放地址 个位
	char_display(30,4,CfgPowerAmp_shi,Ascii_char_display_buffer,1);//功放地址 十位
	char_display(38,4,CfgPowerAmp_ge,Ascii_char_display_buffer,1);//功放地址 个位	
}

