#include "system.h"
#include "stm32f103.h"
#include "tft.h"
#include <stdint.h>

extern uint32_t _estack;

uint8_t x = 0, y = 0;

volatile uint32_t msTicks = 0;

void SysTick_Handler(void) { msTicks++; }

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
  /*
  现象：当你的 PLL 成功起振并准备将系统频率提升到 72MHz 时，如果 Flash
  的读取速度跟不上 CPU 的速度， CPU 取不到指令，就会立即触发 BusFault 继而转为
  HardFault。 解决：在切换到 PLL 之前，必须设置 Flash Latency。对于
  72MHz，通常需要 2 个等待周期（WS）。
  */
  FLASH_ACR |= FLASH_ACR_LATENCY_2;
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  SCB->VTOR = 0x08000000UL;
  while ((RCC->CFGR & (0x3UL << 2)) != RCC_CFGR_SWS_PLL)
    ;

  SysTick_Config(SYSTEM_CLOCK_FREQ / 1000);
  TFT_Init();
  TFT_Clear(TFT_BLACK);
  printk("SystemInit Success!", 19);
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
  uint32_t start = msTicks;
  while ((msTicks - start) < ms) {
  }
}

void printk(const char *str, uint32_t size) {
  for (uint32_t i = 0; i < size; i++) {
    char ch = str[i];
    if (x + 8 > TFT_WIDTH) {
      x = 0;
      y = y + 16;
    }
    if (y + 16 > TFT_HEIGHT) {
      x = 0;
      y = 0;
    }
    TFT_SetChar(x, y, ch, TFT_RED, TFT_BLACK);
    x += 8;
  }
}
