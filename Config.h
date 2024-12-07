#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

typedef struct
{
	volatile uint32_t CR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  volatile uint32_t APB2RSTR;
  volatile uint32_t APB1RSTR;
  volatile uint32_t AHBENR;
  volatile uint32_t APB2ENR;
  volatile uint32_t APB1ENR;
  volatile uint32_t BDCR;
  volatile uint32_t CSR;
}RCC_map;

typedef struct
{
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
}GPIO_map;

typedef struct
{
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;
	uint32_t RESERVED;
	volatile uint32_t MAPR2;
}AFIO_map;

typedef struct
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_map;

typedef struct
{
  volatile uint32_t ISER[3];               
  uint32_t RESERVED0[29];
  volatile uint32_t ICER[3];               
  uint32_t RESERVED1[29];
  volatile uint32_t ISPR[3];               
  uint32_t RESERVED2[29];
  volatile uint32_t ICPR[3];               
  uint32_t RESERVED3[29];
  volatile uint32_t IABR[3];               
  uint32_t RESERVED4[61];
  volatile uint8_t  IP[84];              
  uint32_t RESERVED5[695];
  volatile uint32_t STIR;           
} NVIC_map;

typedef struct
{
	volatile uint32_t CTRL;
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
	volatile const uint32_t CALIB;
}SysTick_map;

#define RCC ((RCC_map*)0x40021000)
#define GPIOA ((GPIO_map*) 0x40010800)
#define GPIOB ((GPIO_map*) 0x40010C00)
#define AFIO ((AFIO_map*) 0x40010000)
#define EXTI ((EXTI_map*) 0x40010400)
#define SysTick ((SysTick_map*) 0xE000E010)
#define NVIC ((NVIC_map*) 0xE000E100)

#ifdef __cplusplus

}
#endif

#endif /* __MISC_H */

