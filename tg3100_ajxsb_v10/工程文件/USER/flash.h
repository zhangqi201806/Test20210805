#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#include "stm32f10x.h"

//用户根据自己的需要设置
#define STM32_FLASH_SIZE 256 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)

//FLASH起始地址
#define STM32_FLASH_BASE 		0x08000000 	//STM32 FLASH的起始地址
#define STM32_Program_FLASH 	0x00037000  //STM32 给程序预留的FLASH大小
#define STM32_Page_FLASH 		0x00000800  //STM32 FLASH的页大小 2K

extern u16 STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
extern void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
extern u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据
extern void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
extern void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据
//测试写入
extern void Test_Write(u32 WriteAddr,u16 WriteData);	

#endif

















