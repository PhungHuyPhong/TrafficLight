#include "lcd16x2.h"
#include <stdio.h>
#include "Config.h"
#define LCD_RS      GPIO_Pin_13
#define LCD_RW      GPIO_Pin_14
#define LCD_EN      GPIO_Pin_15

#define LCD_D4      GPIO_Pin_8
#define LCD_D5      GPIO_Pin_9
#define LCD_D6      GPIO_Pin_10
#define LCD_D7      GPIO_Pin_11



void GPIO_LCD_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef        GPIO_LCD_InitStruction;
    
    /*Configure GPIO pin Output Level */
    GPIO_WriteBit((GPIO_TypeDef*)GPIOA, LCD_D4|LCD_D5|LCD_D5|LCD_D7, 0); 
    GPIO_WriteBit(GPIOC, LCD_RS|LCD_RW|LCD_EN, 0); 
    
    GPIO_LCD_InitStruction.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_LCD_InitStruction.GPIO_Pin     = LCD_D4|LCD_D5|LCD_D6|LCD_D7;      
    GPIO_LCD_InitStruction.GPIO_Speed   = GPIO_Speed_2MHz;
    GPIO_Init((GPIO_TypeDef*)GPIOA, &GPIO_LCD_InitStruction);


    GPIO_LCD_InitStruction.GPIO_Pin =  LCD_EN|LCD_RS|LCD_RW;
    GPIO_Init(GPIOC, &GPIO_LCD_InitStruction);
}

void LCD_Enable(void)
{
    GPIO_SetBits(GPIOC, LCD_EN);
    DelayMs(1);
    GPIO_ResetBits(GPIOC, LCD_EN);
    DelayMs(1);
}

void LCD_Send4Bit(unsigned char Data)
{
  GPIO_WriteBit((GPIO_TypeDef*)GPIOA, LCD_D4, Data & 0x01);
  GPIO_WriteBit((GPIO_TypeDef*)GPIOA, LCD_D5, (Data>>1)&1);
  GPIO_WriteBit((GPIO_TypeDef*)GPIOA, LCD_D6, (Data>>2)&1);
  GPIO_WriteBit((GPIO_TypeDef*)GPIOA, LCD_D7, (Data>>3)&1);
}

void LCD_SendCommand(unsigned char command)
{
  LCD_Send4Bit(command >> 4);
  LCD_Enable();
  LCD_Send4Bit(command);
  LCD_Enable();

}

void LCD_Clear(void)
{
  LCD_SendCommand(0x01);
  DelayUs(10);
}

void LCD_Init(void)
{
  GPIO_LCD_Config();
// Delay initialization
  DelayInit();
  
  LCD_SendCommand(0x28); // giao thuc 4 bit, hien thi 2 hang, ki tu 5x8
  LCD_SendCommand(0x0C); // cho phep hien thi man hinh
  LCD_SendCommand(0x06); // tang ID, khong dich khung hinh
  LCD_SendCommand(0x01); // xoa toan bo khung hinh
}

void LCD_Gotoxy(unsigned char x, unsigned char y)
{
  unsigned char address;
  if(y == 0)address=(0x80 + x);
  else if(y == 1) address=(0xc0 + x);
  LCD_SendCommand(address);
}

void LCD_PutChar(unsigned char Data)
{
  GPIO_SetBits(GPIOC, LCD_RS);
  LCD_SendCommand(Data);
  GPIO_ResetBits(GPIOC, LCD_RS);
}

void LCD_Puts(char *s)
{
  while (*s)
  {
    LCD_PutChar(*s);
    s++;
  }
}
void LCD_PutNum(int x)
{
	int x1,a[16],i=0;
	if(x<10) {
	LCD_PutChar( 48 );
	LCD_PutChar( x+48 );}
		else
		{
			while (x!=0){
			x1=x%10;
			a[i++]=x1;
			x=x/10;				
		}
	}
		for( int j=i-1;j>=0;j--) LCD_PutChar( a[j]+48 );
}
