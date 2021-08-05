#ifndef __LCD_H
#define __LCD_H	 
#include "stm32f10x.h"

#define BACK_LIGHT_ON()			GPIO_ResetBits(GPIOC,GPIO_Pin_9)	                
#define BACK_LIGHT_OFF()		GPIO_SetBits(GPIOC,GPIO_Pin_9)	 	
#define BACK_LIGHT_ISOFF()	GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_9)

#define LCDCS_ENABLE()		GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define LCDCS_DISABLE()		GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define LCDRES_ENABLE()		GPIO_ResetBits(GPIOC,GPIO_Pin_7)	                     
#define LCDRES_DISABLE()	GPIO_SetBits(GPIOC,GPIO_Pin_7)	
#define LCDA0_COMMAND()		GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define LCDA0_DATA()		GPIO_SetBits(GPIOB,GPIO_Pin_12)

void LCD_HardwareInit(void);  
void LCD_WriteData(uint8_t Data);
void delayus(uint16_t us);
void LCD_GotoXY(uint8_t x, uint8_t y);
void LcdFill(uint8_t FillData);  
void LcdFillRect(uint8_t x,uint8_t y,uint8_t dx,uint8_t dy,uint8_t Data);
void LCD_WriteLine(uint8_t y, uint8_t udata);    
void LCD_WriteAll(uint8_t udata);       

void Fun_WriteStr(uint8_t x, uint8_t y, const uint8_t *p, uint8_t type);     //Ð´×Ö·û´®
void Fun_DisTandaBMP(void);
void Lcd_BackLightOn(void);
#endif
