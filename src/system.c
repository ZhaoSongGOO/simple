#include "system.h"
#include "stm32f103.h"

extern uint32_t _estack;

void SystemInit(void) {
  // enable HSE clock (8MHZ)
  RCC->CR |= RCC_CR_HSEON;

  while (!(RCC->CR & RCC_CR_HSERDY))
    ;
}
