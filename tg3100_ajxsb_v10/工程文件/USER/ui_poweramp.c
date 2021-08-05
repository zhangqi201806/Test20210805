#include "led.h"
#include "lcd.h"
#include "config.h"
#include "txandre.h"  
#include "mre.h"
#include "ui_main.h"
#include "powamp.h" 

//功放注册界面
const struct StructHead_CtrlType Struct_CtrlPoweiAmpRegisWnd[] =
{
	{DEF_CTRLTEXT    ,0,0,0,0,(uint8_t*)"在线设备总数:",		0,0             }, 
	{DEF_CTRLTEXT    ,1,0,2,0,(uint8_t*)"在线功放地址:",		0,0             },
	{DEF_CTRLTEXT    ,2,0,4,0,(uint8_t*)"1.2.3.4.5.6.7.8.",		0,0             },
	{DEF_CTRLTEXT    ,3,0,6,0,(uint8_t*)"9.10.",				0,0             },
};

u8 CfgPowerAmpWnd_Flag;

static void CloseAllAreaLed(void)
{
	int nLedIndex;
	for(nLedIndex=6;nLedIndex<LED_SUM-1;nLedIndex++)
	{								
		led_CloseLed(nLedIndex); 
		if(nLedIndex%6==5)
			nLedIndex+=6;
	}	
}

void ShowAreaLed(void)
{
	int i;
	
	for(i=0;i<30;i++)
	{
		if(Area_Startup_Flag[i])
			led_LightLed((i/6)*12+i%6+6);
	
		if(Area_Fault_Flag[i])
			led_LightLed((i/6)*12+i%6+12);
	}
}

//功放配置初始化 点LED
void Clear_PowAmpCfg_Var(void)
{
	CfgPowerAmpWnd_Flag=0;
	Cfg_PowAmp1to10_InitSta_OneEnter_Flag=0;
	
	CloseAllAreaLed();
	ShowAreaLed();
}

void Cfg_PowAmp_AreaLed(void)
{
	u8 i,j;
	u8 pos,pos1;
	
	CloseAllAreaLed();
	
	//点亮功放对应的区LED
	for(i=0;i<4;i++)
	{
		if(i%4==3)
		{
			for(j=0;j<6;j++)
			{
				pos = ((uint8_t)0x01) << j;
				pos1=Edit_Cfg_PowAmp_Area_Buffer[i]&pos;
				if(pos1!=pos) 
				{
					led_LightLed(0x36+j);
				}				
			}
		}
		else
		{
			for(j=0;j<8;j++)
			{
				pos = ((uint8_t)0x01) << j;
				pos1=Edit_Cfg_PowAmp_Area_Buffer[i]&pos;
				if(pos1!=pos) 
				{
					if(i==0)
					{
						if(j==0||j==1||j==2||j==3||j==4||j==5)
						led_LightLed(0x06+j);
					}
					else if(i==1)
					{
						if(j==0||j==1||j==2||j==3)	led_LightLed(0x14+j);
						if(j==4)					led_LightLed(0x1e);
						if(j==5)					led_LightLed(0x1f);
					}	
					else if(i==2)
					{
						if(j==0)					led_LightLed(0x22);
						if(j==1)					led_LightLed(0x23);
						if(j==2||j==3||j==4||j==5) led_LightLed(0x28+j);
					}					
					if(j==6) 
						led_LightLed(0x12+0x0e*i);
					if(j==7)
						led_LightLed(0x13+0x0e*i);	
				}				
			}
		}			
	}	
}

void StrCpy(u8 *pDst,char *pSrc)
{
	while(*pSrc)
		*pDst++ = *pSrc++;
	*pDst = 0;
}

static u8 nCurShowPa = PA_NUM;
void Show_PowerAmp_Offline(int bReDraw)
{
	int nPa;
	u8 strBuf[13];
	
	nPa = PowerAmp_GetOfflinePa();
	
	if(nCurShowPa != nPa || bReDraw)
	{
		nCurShowPa = nPa;
		
		if(nPa == PA_NUM)
		{
			LCD_WriteLine(2,0);
			LCD_WriteLine(3,0);
		}
		else
		{
			LcdFillRect(0,2,32,2,0);
			Byte2Str(nPa+1,strBuf);
			StrCpy(&strBuf[2],"号功放掉线");
			strBuf[12] = 0;
			Fun_WriteStr(32,2,(uint8_t *)strBuf,0);
		}
	}
}

void ui_Register_Finshed(void)
{
	u8 a[3],i;
  u16 nCount;	
	
	LcdFill(0x00);

	Fun_WriteStr(0,0,(uint8_t *)("在线设备总数:"),0);
	nCount = PowerAmp_GetRegedCount();
	if(nCount < 10)
	{
		Fun_WriteStr(98,0,(uint8_t *)("0"),0);
		i=8;
	}
	else
		i=0;
	Com_itoa(nCount, a);
	Fun_WriteStr(98+i,0,a,0);	
	
	Fun_WriteStr(0,2,(uint8_t *)("在线功放地址:"),0);
	Fun_WriteStr(0,4,(uint8_t *)("1.") ,PowerAmp_IsReged(1));
	Fun_WriteStr(16,4,(uint8_t *)("2.") ,PowerAmp_IsReged(2));
	Fun_WriteStr(32,4,(uint8_t *)("3.") ,PowerAmp_IsReged(3));
	Fun_WriteStr(48,4,(uint8_t *)("4.") ,PowerAmp_IsReged(4));
	Fun_WriteStr(64,4,(uint8_t *)("5.") ,PowerAmp_IsReged(5));
	Fun_WriteStr(80,4,(uint8_t *)("6.") ,PowerAmp_IsReged(6));
	Fun_WriteStr(96,4,(uint8_t *)("7.") ,PowerAmp_IsReged(7));
	Fun_WriteStr(112,4,(uint8_t *)("8."),PowerAmp_IsReged(8));
	Fun_WriteStr(0,6,(uint8_t *)("9.")  ,PowerAmp_IsReged(9));
	Fun_WriteStr(16,6,(uint8_t *)("10."),PowerAmp_IsReged(10));		
}

void ui_Registering(void)
{
	LcdFill(0x00);
	Fun_WriteStr(12,3,(uint8_t *)("注册中 请稍后"),0);
}

