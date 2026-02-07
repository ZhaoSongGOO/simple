#ifndef SYSTEM_H
#define SYSTEM_H

#include "root.h"
#include <stdint.h>
void SystemInit(void);
uint32_t SysTick_Config(uint32_t ticks);
void Delay_ms(uint32_t ms);
void Trigger_SVC(SysCall_t sc_id);
void Trigger_SVC_Print();
#endif