
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

void DelayInit(void);
void SysTick_Handler(void);
void DelayUs(uint32_t us);
void DelayMs(uint32_t ms);
void Delays(uint32_t ms);
#ifdef __cplusplus
}
#endif

#endif

