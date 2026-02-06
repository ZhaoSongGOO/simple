
#ifndef STM32F103_H
#define STM32F103_H

#include <stdint.h>

#define PERIPH_BASE 0x40000000UL
#define APB1PERIPH_BASE PERIPH_BASE
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000UL)
#define AHBPERIPH_BASE (PERIPH_BASE + 0x20000UL)

#define RCC_BASE (AHBPERIPH_BASE + 0x1000UL)
#define RCC ((RCC_TypeDef *)RCC_BASE)

typedef struct {
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
} RCC_TypeDef;

#define RCC_CR_HSEON (1UL << 16)
#define RCC_CR_HSERDY (1UL << 17)
#define RCC_CR_PLLON (1UL << 24)
#define RCC_CR_PLLRDY (1UL << 25)

#define RCC_CFGR_SW_PLL (2UL << 0)
#define RCC_CFGR_SWS_PLL (2UL << 2)
#define RCC_CFGR_PLLSRC (1UL << 16)
#define RCC_CFGR_PLLMUL_9 (7UL << 18)

#define RCC_APB2ENR_IOPAEN (1UL << 2)
#define RCC_APB2ENR_IOPBEN (1UL << 3)
#define RCC_APB2ENR_IOPCEN (1UL << 4)
#define RCC_APB2ENR_USART1EN (1UL << 14)
#define RCC_APB2ENR_SPI1EN (1UL << 12)

#define RCC_APB1ENR_USART2EN (1UL << 17)
#define RCC_APB1ENR_USART3EN (1UL << 18)
#define RCC_APB1ENR_SPI2EN (1UL << 14)
#define RCC_APB1ENR_SPI3EN (1UL << 15)

#define GPIOA_BASE (APB2PERIPH_BASE + 0x0800UL)
#define GPIOB_BASE (APB2PERIPH_BASE + 0x0C00UL)
#define GPIOC_BASE (APB2PERIPH_BASE + 0x1000UL)

#define GPIOA ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef *)GPIOC_BASE)

typedef struct {
  volatile uint32_t CRL;
  volatile uint32_t CRH;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t BRR;
  volatile uint32_t LCKR;
} GPIO_TypeDef;

#define SYSTICK_BASE 0xE000E010UL
#define SysTick ((SysTick_TypeDef *)SYSTICK_BASE)

typedef struct {
  volatile uint32_t CTRL;
  volatile uint32_t LOAD;
  volatile uint32_t VAL;
  volatile uint32_t CALTB;
} SysTick_TypeDef;

#define SysTick_CTRL_ENABLE (1UL << 0)
#define SysTick_CTRL_TICKINT (1UL << 1)
#define SysTick_CTRL_CLKSOURCE (1UL << 2)
#define SysTick_CTRL_COUNTFLAGS (1UL << 16)

#define SCB_BASE 0xE000ED00UL
#define SCB ((SCB_TypeDef *)SCB_BASE)

typedef struct {
  volatile uint32_t CPUIO;
  volatile uint32_t ISCR;
  volatile uint32_t VTOR;
  volatile uint32_t AIRCR;
  volatile uint32_t SCR;
  volatile uint32_t CCR;
  volatile uint32_t SHPR[3];
  volatile uint32_t SHCSR;
  volatile uint32_t CFSR;
  volatile uint32_t HFSR;
  volatile uint32_t DFSR;
  volatile uint32_t MMFAR;
  volatile uint32_t BFAR;
  volatile uint32_t AFSR;
} SCB_TypeDef;

#define SCB_VTOR_TBLOFF (0x1FFFFFF80UL)

#define NVIC_BASE 0xE000E100UL
#define NVIC ((NVIC_TypeDef *)NVIC_BASE)

typedef struct {
  volatile uint32_t ISER[8];
  uint32_t RESERVED0[24];
  volatile uint32_t ICER[8];
  uint32_t RESERVED1[24];
  volatile uint32_t ISPR[8];
  uint32_t RESERVED2[24];
  volatile uint32_t ICPR[8];
  uint32_t RESERVED3[24];
  volatile uint32_t IABR[8];
  uint32_t RESERVED4[56];
  volatile uint32_t IP[240];
  uint32_t RESERVED5[644];
  volatile uint32_t STIR;
} NVIC_TypeDef;

#define SYSTEM_CLOCK_FREQ 72000000UL
#define APB1_CLOCK_FREQ (SYSTEM_CLOCK_FREQ / 2)
#define APB2_CLOCK_FREQ SYSTEM_CLOCK_FREQ

#define FLASH_R_BASE 0x40022000UL
#define FLASH_ACR (*(volatile uint32_t *)(FLASH_R_BASE + 0x00))

#define FLASH_ACR_LATENCY_2                                                    \
  (0x2UL << 0) // 2个等待周期，适用于 48MHz < SYSCLK <= 72MHz
#define FLASH_ACR_PRFTBE (0x1UL << 4) // 开启预取缓冲区 (建议开启以提升性能)

#define SPI1_BASE (APB2PERIPH_BASE + 0x3000UL)
#define SPI2_BASE (APB1PERIPH_BASE + 0x3800UL)
#define SPI3_BASE (APB1PERIPH_BASE + 0x3C00UL)

#define SPI1 ((SPI_TypeDef *)SPI1_BASE)
#define SPI2 ((SPI_TypeDef *)SPI2_BASE)
#define SPI3 ((SPI_TypeDef *)SPI3_BASE)

typedef struct {
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t SR;
  volatile uint32_t DR;
  volatile uint32_t CRCPR;
  volatile uint32_t RXCRCR;
  volatile uint32_t TXCRCR;
  volatile uint32_t I2SCFGR;
  volatile uint32_t I2SPR;
} SPI_TypeDef;

#define SPI_CR1_CPHA (1UL << 0)
#define SPI_CR1_CPOL (1UL << 1)
#define SPI_CR1_MSTR (1UL << 2)
#define SPI_CR1_BR (0x7UL << 3)
#define SPI_CR1_SPE (1UL << 6)
#define SPI_CR1_LSBFIRST (1UL << 7)
#define SPI_CR1_SSI (1UL << 8)
#define SPI_CR1_SSM (1UL << 9)
#define SPI_CR1_DFF (1UL << 11)

#define SPI_SR_RXNE (1UL << 0)
#define SPI_SR_TXE (1UL << 1)
#define SPI_SR_BSY (1UL << 7)

#define AFIO_BASE (APB2PERIPH_BASE + 0x0000UL)

typedef struct {
  volatile uint32_t EVCR;      // 事件控制寄存器
  volatile uint32_t MAPR;      // 复用重映射寄存器 (最关键)
  volatile uint32_t EXTICR[4]; // 外部中断配置寄存器
  uint32_t RESERVED0;          // 保留
  volatile uint32_t MAPR2;     // 复用重映射寄存器 2
} AFIO_TypeDef;

#define AFIO ((AFIO_TypeDef *)AFIO_BASE)

// 相关宏定义
#define RCC_APB2ENR_AFIOEN (1UL << 0)

#endif
