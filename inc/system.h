#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

void SystemInit(void);
uint32_t SysTick_Config(uint32_t ticks);
void Delay_ms(uint32_t ms);
void printk(const char *str, uint32_t size);
#endif