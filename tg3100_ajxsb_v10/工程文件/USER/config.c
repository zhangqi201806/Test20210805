#include "flash.h"
#include "config.h"

#define PA_ADDR(PA)	(STM32_FLASH_BASE+STM32_Program_FLASH+STM32_Page_FLASH*(PA))
#define MAIN_ADDR	PA_ADDR(10)
#define PASS_ADDR	PA_ADDR(11)

static uint8_t Cfg_PowAmp_Flag[10],Cfg_PowAmp_Num;
static uint8_t Cfg_Area_Flag[270],Cfg_Area_Num;
static uint8_t Cfg_PowAmp_ReadFlash_Buffer[10][40]={0};
static uint8_t Password_Buffer[2][4]={0};

uint8_t Edit_Cfg_PowAmp_Area_Buffer[40]={0};
uint8_t Main_Adress_ReadFlash_Buffer[4]={0};
uint8_t Edit_Password_Buffer[4]={0};

//读写 主板地址  信息
void Read_Main_Adress(void)
{
	STMFLASH_Read(MAIN_ADDR,(u16*)Main_Adress_ReadFlash_Buffer,2);

	if((Main_Adress_ReadFlash_Buffer[0]>9)||(Main_Adress_ReadFlash_Buffer[1]>9)||(Main_Adress_ReadFlash_Buffer[2]>9))
	{
		Main_Adress_ReadFlash_Buffer[0] = 1;
		Main_Adress_ReadFlash_Buffer[1] = 1;
		Main_Adress_ReadFlash_Buffer[2] = 8;
	}
}

static uint8_t CurAddr[4];
void Write_Main_Adress(void)
{
	STMFLASH_Read(MAIN_ADDR,(u16*)CurAddr,2);

	if((CurAddr[0]>9)||(CurAddr[1]>9)||(CurAddr[2]>9))
	{
		CurAddr[0] = 1;
		CurAddr[1] = 1;
		CurAddr[2] = 8;
	}
	
	if((*(u32 *)CurAddr) != (*(u32 *)Main_Adress_ReadFlash_Buffer))
		STMFLASH_Write(MAIN_ADDR,(u16*)Main_Adress_ReadFlash_Buffer,2);
}

static u8 DefPassword[2][4] = {{1,2,3,4},{1,3,5,7}};

//读写 主板地址  信息
void Read_Password(void)
{
	int nIndex,nSubIndex;
	
	STMFLASH_Read(PASS_ADDR,(u16*)Password_Buffer,4);
	for(nIndex=0;nIndex<2;nIndex++)
	{
		if(Password_Buffer[nIndex][0]>9 || Password_Buffer[nIndex][1]>9 || Password_Buffer[nIndex][2]>9 || Password_Buffer[nIndex][3]>9)
		{
			for(nSubIndex=0;nSubIndex<4;nSubIndex++)
				Password_Buffer[nIndex][nSubIndex] = DefPassword[nIndex][nSubIndex];
		}
	}
}

int Password_IsUpdated(int nType)
{
	int nIndex;
	
	for(nIndex=0;nIndex<4;nIndex++)
		if(Password_Buffer[nType][nIndex] != Edit_Password_Buffer[nIndex])
		{
			return 1;
		}
	return 0;
}

void Write_Password(int nType)
{
	int nIndex,bFlag;

	bFlag = 0;
	for(nIndex=0;nIndex<4;nIndex++)
		if(Password_Buffer[nType][nIndex] != Edit_Password_Buffer[nIndex])
		{
			Password_Buffer[nType][nIndex] = Edit_Password_Buffer[nIndex];
			bFlag = 1;
		}
		
	if(bFlag)
		STMFLASH_Write(PASS_ADDR,(u16*)Password_Buffer,4);
}

void Load_PasswordBuf(int nType)
{
	int nIndex;
	if(nType < 2)
	{
		for(nIndex=0;nIndex<4;nIndex++)
			Edit_Password_Buffer[nIndex] = Password_Buffer[nType][nIndex];
	}
	else
	{
		for(nIndex=0;nIndex<4;nIndex++)
			Edit_Password_Buffer[nIndex] = 0;
	}
}

int Check_Password(int nType)
{
	int nIndex;
	
	for(nIndex=0;nIndex<4;nIndex++)
		if(Edit_Password_Buffer[nIndex] != Password_Buffer[nType][nIndex])
			break;
	if(nIndex<4)
		return 0;
	return 1;
}
static u8 SysPassword[4] = {0x09,0x05,0x02,0x07};
static u8 SysFlag;
void Set_SysPassword(int nPos)
{
	int nIndex;
	
	for(nIndex=0;nIndex<4;nIndex++)
	{
		if(nIndex <= nPos)
		{
			if(Edit_Password_Buffer[nIndex] != SysPassword[nIndex])
				break;
		}
		else
		{
			if(Edit_Password_Buffer[nIndex])
				break;
		}
	}
	if(nIndex == 4)
		SysFlag |= (1<<nPos);
	else
		SysFlag = 0;
}

int Check_SysPassword(void)
{
	if((*(u32 *)Edit_Password_Buffer) == (*(u32 *)SysPassword) && SysFlag == 0x07)
	{
		SysFlag = 0;
		return 1;
	}
	return 0;
}

void UpdatePowAmpFlag(int nPa)
{
	int nIndex;
	
	for(nIndex=0;nIndex<36;nIndex+=4)
		if(*(u32 *)(&Cfg_PowAmp_ReadFlash_Buffer[nPa][nIndex]) != 0xFFFFFFFF)
			break;
		
	if(nIndex<36)
	{
		if(!Cfg_PowAmp_Flag[nPa])
		{
			Cfg_PowAmp_Flag[nPa] = 1;
			Cfg_PowAmp_Num++;
		}
	}
	else
	{
		if(Cfg_PowAmp_Flag[nPa])
		{
			Cfg_PowAmp_Flag[nPa] = 0;
			Cfg_PowAmp_Num--;
		}
	}
}

void InitAllAreaFlag(void)
{
	int nArea,nPa,nPos;
	u8 nMask;
	
	Cfg_Area_Num = 0;
	for(nArea=0;nArea<270;nArea++)
	{
		nPos = (nArea/30)*4+(nArea%30)/8;
		nMask = 1<<(nArea%30%8);
		
		for(nPa=0;nPa<10;nPa++)
		{
			if(!(Cfg_PowAmp_ReadFlash_Buffer[nPa][nPos] & nMask))
				break;
		}
		if(nPa<10)
		{
			Cfg_Area_Flag[nArea] = 1;
			Cfg_Area_Num++;
		}
		else
			Cfg_Area_Flag[nArea] = 0;
	}
}

void Init_AllPowAmpCfg(void)
{
	int nPa;
	
	Cfg_PowAmp_Num = 0;
	for(nPa = 0;nPa<10;nPa++)
	{
		STMFLASH_Read(PA_ADDR(nPa),(u16*)Cfg_PowAmp_ReadFlash_Buffer[nPa],20);
		
		UpdatePowAmpFlag(nPa);
	}
	InitAllAreaFlag();
}

//编辑功放区配置
void Edit_PowAmpCfg(int nPa)
{
	int nIndex;
	
	if(nPa<0 || nPa>9) return;
	
	for(nIndex=0;nIndex<36;nIndex+=4)
		*(u32 *)(&Edit_Cfg_PowAmp_Area_Buffer[nIndex]) = *(u32 *)(&Cfg_PowAmp_ReadFlash_Buffer[nPa][nIndex]);
}

void config_ToggleAreaFlag(int nArea)
{
	int nIndex;
	u8 nMask;
	
	if(nArea<0 || nArea>=270) return;
	
	nIndex = (nArea/30)*4+((nArea%30)/8);
	nMask = 1<<(nArea%30%8);
	
	Edit_Cfg_PowAmp_Area_Buffer[nIndex] = Edit_Cfg_PowAmp_Area_Buffer[nIndex]^nMask;
}

void Write_PowAmpCfg(int nPa)
{
	int nIndex;
	
	if(nPa<0 || nPa>9) return;
	
	for(nIndex=0;nIndex<36;nIndex+=4)
	{
		if(*(u32 *)(&Cfg_PowAmp_ReadFlash_Buffer[nPa][nIndex]) != *(u32 *)(&Edit_Cfg_PowAmp_Area_Buffer[nIndex]))
			break;
	}
	if(nIndex == 36)
		return;
	
	for(nIndex=0;nIndex<36;nIndex+=4)
		*(u32 *)(&Cfg_PowAmp_ReadFlash_Buffer[nPa][nIndex]) = *(u32 *)(&Edit_Cfg_PowAmp_Area_Buffer[nIndex]);
	
	STMFLASH_Write(PA_ADDR(nPa),(u16*)Cfg_PowAmp_ReadFlash_Buffer[nPa],20);
	
	UpdatePowAmpFlag(nPa);
	
	InitAllAreaFlag();
}

int config_IsPowAmpCfged(int nPa)
{
	if(nPa<0 || nPa>9) return 0;
	
	return Cfg_PowAmp_Flag[nPa];
}

int config_GetPowAmpCfgedNum(void)
{
	return Cfg_PowAmp_Num;
}

int config_IsAreaCfged(int nPa,int nArea)
{
	int nIndex;
	u8 nMask;
	
	if(nPa<0 || nPa>9) return 0;
	if(nArea<0 || nArea>=270) return 0;
	
	nIndex = (nArea/30)*4+((nArea%30)/8);
	nMask = 1<<(nArea%30%8);
	
	return (!(Cfg_PowAmp_ReadFlash_Buffer[nPa][nIndex] & nMask)?1:0);
}

int config_IsAreaCfgedAny(int nArea)
{
	if(nArea<0 || nArea>=270) return 0;
	return Cfg_Area_Flag[nArea];
}

void config_ReadAllConfig(void)
{
	Init_AllPowAmpCfg();
	
	Read_Main_Adress();

	Read_Password();
}

void config_RemoveAllCfg(void)
{
	int nPa,nIndex;
	
	for(nPa=0;nPa<10;nPa++)
	{
		for(nIndex=0;nIndex<40;nIndex+=4)
			*(u32 *)(&Cfg_PowAmp_ReadFlash_Buffer[nPa][nIndex]) = 0xFFFFFFFF;
		
		STMFLASH_Write(PA_ADDR(nPa),(u16*)Cfg_PowAmp_ReadFlash_Buffer[nPa],20);
	}
	
	*(u32 *)Main_Adress_ReadFlash_Buffer = 0xFFFFFFFF;
	STMFLASH_Write(MAIN_ADDR,(u16*)Main_Adress_ReadFlash_Buffer,2);	
	
	*(u32 *)(Password_Buffer[0]) = 0xFFFFFFFF;
	*(u32 *)(Password_Buffer[1]) = 0xFFFFFFFF;
	STMFLASH_Write(PASS_ADDR,(u16*)Password_Buffer,4);
	
	config_ReadAllConfig();
}

