#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "Config.h"
#include "lcd16x2.h"
/* lcd d4->d7 a8->a11
   rs rw e c13->c15
	 
	 red    south a7
	 yellow south	a6
	 green  south	a5
	 red    west	a4
	 yellow west	a3
	 green  west	a2
	 red    walk  a1
	 white  walk  a0
	 
	 south sensor b5
	 west sensor  b6
	 walk sensor  b7
*/
#define Stop 0
#define GoW 1
#define WaitW 2
#define GoS 3
#define WaitS 4
#define Walk 5
#define Warn1 6
#define Warn2 7
#define Warn3 8
#define Warn4 9
#define Warn5 10

static uint32_t input = 0;
void config(void);
void EXTI9_5_IRQHandler(void);
struct state
{
	uint8_t setup;
	uint16_t time;
	uint8_t next[8];
};
static const struct state FSM[11]={
{0x92,0,{Stop,Walk,GoW,GoW,GoS,GoS,GoS,GoS}},
{0x86,10,{WaitW,WaitW,GoW,WaitW,WaitW,WaitW,WaitW,WaitW}},
{0x8A,5,{Stop,Walk,GoW,Walk,GoS,GoS,GoS,Walk}},
{0x32,10,{WaitS,WaitS,WaitS,WaitS,GoS,WaitS,WaitS,WaitS}},
{0x52,5,{Stop,Walk,GoW,GoW,GoS,Walk,GoW,GoW}},
{0x91,10,{Warn1,Walk,Warn1,Warn1,Warn1,Warn1,Warn1,Warn1}},
{0x91,1,{Warn2,Warn2,Warn2,Warn2,Warn2,Warn2,Warn2,Warn2}},
{0x92,1,{Warn3,Warn3,Warn3,Warn3,Warn3,Warn3,Warn3,Warn3}},
{0x90,1,{Warn4,Warn4,Warn4,Warn4,Warn4,Warn4,Warn4,Warn4}},
{0x92,1,{Warn5,Warn5,Warn5,Warn5,Warn5,Warn5,Warn5,Warn5}},
{0x90,1,{Stop,Walk,GoW,GoW,GoS,GoS,GoS,GoS}},
};
int main(void)
{
	LCD_Init();
	config();
	input=0;
	uint8_t i=0;
	while(1){
		int time=1;
		int j;
		GPIOA->ODR=FSM[i].setup;
		if(i==Stop) 
		{		
			LCD_Gotoxy(6,0);
        	LCD_PutNum(0);			
			i=FSM[i].next[input];
			continue;
		}
		
		if(i==GoW||i==GoS||i==Walk||i==WaitW||i==WaitS) 
		{
  			time =	FSM[i].time;	
			for(j=1;j<=time;j++)
			{
				LCD_Gotoxy(6,0);
        		LCD_PutNum(j);
				Delays(1);
			}
			while(i==FSM[i].next[input])
			{
				LCD_Gotoxy(6,0);
    			LCD_PutNum(j++);
				Delays(1);
			}
			i=FSM[i].next[input];
			continue;
		}
		if(i== Warn1||i==Warn2||i==Warn3||i==Warn4||i==Warn5)
		{
			while(i== Warn1||i==Warn2||i==Warn3||i==Warn4||i==Warn5)
			{ 
				GPIOA->ODR=FSM[i].setup;
				LCD_Gotoxy(6,0);
            	LCD_PutNum(time++);
		  		Delays(1);
				i=FSM[i].next[input];
				continue;
			}
		}
	}
}

void config(void)
{
	RCC->APB2ENR |= 0xd;

	GPIOA->CRL = 0x22222222;
	GPIOB->CRL &= 0x000FFFFF;
	GPIOB->CRL |= 0x88800000;
	GPIOB->ODR |= 0x00;  //pull down
	
	AFIO->EXTICR1 |= 0x1110;
	EXTI->IMR |= 0xE0; // Interupt pin 5-6-7
	EXTI->RTSR|= 0xE0; // Interupt pin 5-6-7
	EXTI->FTSR|= 0xE0; // Interupt pin 5-6-7
	NVIC->ISER[0]= 1U<<23U;
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR & 0xE0)
	{
		input = (GPIOB->IDR >> 5) & 0x07;
	}
	EXTI->PR|=0xE0;
}
