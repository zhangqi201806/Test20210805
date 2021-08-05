#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "lcd.h"
#include "bitmap.h"
#include "timer.h"

#define DEF_HZK1NUM                 3755		/*һ���ֿ�16x16����	*/
#define DEF_ASCIINUM                95			/*ASCII8x16�ַ���	*/

void LCD_WriteByte(uint8_t TxData)	
{					  
	SPI_I2S_SendData(SPI2, TxData);   
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 
		{
				;
		}	
	SPI2->SR|=SPI_I2S_FLAG_TXE;	
}

void LCD_WriteCmd(uint8_t Command)
{	
	LCDCS_ENABLE();		
	LCDA0_COMMAND();	
	delayus(300);
	LCD_WriteByte(Command);	
	delayus(300);
	LCDCS_DISABLE();		
}

void Cfg_LCD_Com(void)
{
	LCDCS_ENABLE();		
	BACK_LIGHT_ON();	
//	//Һ�����ó�ʼ��
	LCDRES_ENABLE();
	delayus(7200);       
	LCDRES_DISABLE();
	delayus(7200); 		
	
	LCD_WriteCmd(0xe2);		//��λ
	LCD_WriteCmd(0xA2);		//BIAS����
	LCD_WriteCmd(0xA0);		//ADCѡ��,//ADC seg����ѡ�� 0xa0����,0xA1 ���Ҿ���
	LCD_WriteCmd(0xC8);		//COM���ģʽѡ��
	LCD_WriteCmd(0x2C);		//Power control set
	LCD_WriteCmd(0x2E);		//Power control set
	LCD_WriteCmd(0x2F);		//Power control set
	LCD_WriteCmd(0x81);		//Set electronic volume
	LCD_WriteCmd(0x18);		//Set electronic volume
	LCD_WriteCmd(0x24);		//
	LCD_WriteCmd(0xAF);		//��LCD��ʾ
	LCD_WriteCmd(0x40);		//�����п�ʼд	
}

void LCD_HardwareInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOB,&GPIO_InitStructure);     	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	 	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		  
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI2, &SPI_InitStructure);    	
	SPI_Cmd(SPI2, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;	 	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB,&GPIO_InitStructure);	  
	Cfg_LCD_Com();
	LcdFill(0x00);
}

void LCD_WriteData(uint8_t Data)
{
	LCDCS_ENABLE();		
	LCDA0_DATA();
	delayus(300);		
	LCD_WriteByte(Data);	
	delayus(300);
	LCDCS_DISABLE();			
}

void delayus(uint16_t us)
{
	while(us--); 
}

//��λLCD
void LCD_GotoXY(uint8_t x, uint8_t y)
{
    if(x == 0)
        x += 1;
	y = y + 4;
	if(y > 7)
		y -= 8;
	LCD_WriteCmd(0xB0 | y);         //set page address
	LCD_WriteCmd(0x10 | (x >> 4));  //set column address
	LCD_WriteCmd(x & 0x0F);
}

void LcdFill(uint8_t FillData)    //����
{
	uint8_t x,y;
	for (y=0; y<8; y++) 				
	{
		delayus(648);
		LCD_GotoXY(0,y);
		delayus(648);
		for (x=0; x<128; x++)
		{
			LCD_WriteData(FillData);
		}	
	}
	delayus(648);
}

void LcdFillRect(uint8_t x,uint8_t y,uint8_t dx,uint8_t dy,uint8_t Data)
{
	uint8_t nx,ny;
	for(ny=y;ny<y+dy;ny++)
	{
		delayus(648);
		LCD_GotoXY(x,ny);
		delayus(648);
		for (nx=x; nx<x+dx; nx++)
		{
			LCD_WriteData(Data);
		}	
	}
	delayus(648);
}

void LCD_Test(uint8_t udata)
{
	uint8_t i, j;

    for(j=0; j<8; j++)
    {
		delayus(648);
        LCD_GotoXY(0, j);
		delayus(648);
        for(i=0; i<128; i++)
        {
            if((i % 8) == 0)
                udata = ~udata;
            LCD_WriteData(udata);
        }
        udata = ~udata;
    }
}

void LCD_WriteLine(uint8_t y, uint8_t udata)    //write y line
{
	uint8_t i;

	delayus(648);
    LCD_GotoXY(0, y);
	delayus(648);
    for(i=0; i<128; i++)
        LCD_WriteData(udata);
}

void LCD_WriteAll(uint8_t udata)        //write all lines
{
	uint8_t j;

	for(j=0; j<8; j++) 
		LCD_WriteLine(j, udata);
}

//dot: 8-�ַ���ʾ��16-������ʾ
//x:��Ҫ��ʾ��������Һ���ĺ���λ��
//y:��Ҫ��ʾ��������Һ��������λ��
//*p:��Ҫ��ʾ�����������������ʼ��ַ
//type: 0-������ʾ��1-������ʾ
void Fun_WriteLCD(uint8_t dot, uint8_t x, uint8_t y, const uint8_t *p, uint8_t type)    //д�ַ�����LCD
{
	uint8_t i, j, k, m, n;

	k = dot / 8;
	if(k == 1)
	{
			m = 8;
	}
	else if(k == 2)
	{
			m = 15;
	}
	
	LCD_GotoXY(x, y);
	delayus(360);	
	
	for(i=0; i<2; i++)
	{
		for(j=0; j<m; j++)
		{
			n = *(p + i * m + j);
			if(type & 0x01)
			{
					n = ~n;
			}
			LCD_WriteData(n);
		}
		LCD_GotoXY(x, y+1);
		delayus(360);	
	}
}

//������ʾ�� 
//x:��Ҫ��ʾ��������Һ���ĺ���λ��
//y:��Ҫ��ʾ��������Һ��������λ��
//*p:��Ҫ��ʾ�����ݵ�HEX�� Ҳ�������ڵ�������׵�ַ         
//type:0-������ʾ��1-������ʾ
void Fun_WriteStr(uint8_t x, uint8_t y, const uint8_t *p, uint8_t type)     //д�ַ���
{
	uint8_t  chh, chl;
	uint32_t num;    
      
	while(*p != 0)
	{
		if(*p < 0x7f)          
		{
			if(x > (128-8))    
			{
				return;
				//x = 0;
				//y += 2;
			}
			num = *p - 0x20;  
			if(num < DEF_ASCIINUM)
				Fun_WriteLCD(8, x, y, &ASCII8x16[num][0], type);
			p++;
			x += 8; 
		}
		else       
		{
			if(x > (128-15))    
			{
				return;
				//x = 0;
				//y += 2;
			}
			chh = *p - 0xa0;       
			chl = *(p+1) - 0xa0;	
			num = (chh-16) * 94 + (chl-1);   
			if(num < DEF_HZK1NUM)
					Fun_WriteLCD(16, x, y, &Chinese[num][0], type);
			p += 2;
			x += 15;
		}
	}
}

void Fun_DisTandaBMP(void)      //��ʾTandaͼ��
{
	uint8_t x, y;
    
	for(y=0; y<8; y++)
	{
		LCD_GotoXY(0, y);
		for(x=0; x<128; x++)
			LCD_WriteData(TandaBMP[y][x]);
	}
	
	Fun_WriteStr(1,6,(uint8_t *)("  �����㲥ϵͳ  "), 0);
}

void Lcd_BackLightOn(void)
{
	if(BACK_LIGHT_ISOFF())
		BACK_LIGHT_ON();
	TimerBglcdCounter = 0;
	TimerBglcdFlag = 0;
}

