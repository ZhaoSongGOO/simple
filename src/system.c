#include "system.h"
#include "stm32f103.h"
#include <stdint.h>

extern uint32_t _estack;

void SystemInit(void) {
  // enable HSE clock (8MHZ)
  RCC->CR |= RCC_CR_HSEON;

  while (!(RCC->CR & RCC_CR_HSERDY))
    ;

  RCC->CFGR &= ~(0xFUL << 18);
  RCC->CFGR |= RCC_CFGR_PLLMUL_9;
  RCC->CFGR |= RCC_CFGR_PLLSRC;

  RCC->CR |= RCC_CR_PLLON;

  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;

  RCC->CFGR &= ~(0xFUL << 4);
  RCC->CFGR |= (0x4UL << 4);
  RCC->CFGR &= ~(0x7UL << 8);

  RCC->CFGR &= ~(0x3UL << 0);
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  while ((RCC->CFGR & (0x3UL << 2)) != RCC_CFGR_SWS_PLL)
    ;

  SysTick_Config(SYSTEM_CLOCK_FREQ);

  SCB->VTOR = 0x08000000UL;
}

uint32_t SysTick_Config(uint32_t ticks) {
  if (ticks > 0x00FFFFFFUL) {
    return 1;
  }
  SysTick->LOAD = ticks - 1;
  SysTick->VAL = 0;
  SysTick->CTRL =
      SysTick_CTRL_CLKSOURCE | SysTick_CTRL_TICKINT | SysTick_CTRL_ENABLE;
  return 0;
}

void Delay_ms(uint32_t ms) {
  volatile uint32_t count = ms * (SYSTEM_CLOCK_FREQ);
  while (count--)
    ;
}

void Delay_us(uint32_t us) {
  volatile uint32_t count = us * (SYSTEM_CLOCK_FREQ / 1000000);
  while (count--)
    ;
}
