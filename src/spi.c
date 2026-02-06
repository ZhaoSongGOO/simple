#include "spi.h"
#include "gpio.h"
#include "stm32f103.h"

static SPI_TypeDef *GetSPIBase(spi_port_t port) {
  switch (port) {
  case SPI_PORT_1:
    return SPI1;
  case SPI_PORT_2:
    return SPI2;
  case SPI_PORT_3:
    return SPI3;
  default:
    return (void *)0;
  }
}

static void SPI_EnableClock(spi_port_t port) {
  if (port == SPI_PORT_1) {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  } else if (port == SPI_PORT_2) {
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  } else if (port == SPI_PORT_3) {
    RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
  }
}

/*
在我的这个板子上,
SPI3_MISO -> PB4
SPI3_MOSI -> PB5
SPI3_SCK  -> PB3
*/
static void SPI_ConfigPins(spi_port_t port) {
  if (port == SPI_PORT_3) {
    RCC->APB2ENR |= (1UL << 0);   // 开启 AFIO 时钟
    AFIO->MAPR &= ~(0x7UL << 24); // 清除 SWJ_CFG
    AFIO->MAPR |= (0x2UL << 24);  // 设置为 010：禁用 JTAG-DP，使能 SW-DP
    GPIO_SetAlternateFunction(GPIO_PORT_B, 3, GPIO_SPEED_50MHZ);
    GPIO_SetInput(GPIO_PORT_B, 4, GPIO_PULL_NONE);
    GPIO_SetAlternateFunction(GPIO_PORT_B, 5, GPIO_SPEED_50MHZ);
  }
}

static uint8_t SPI_CalculateBaudRatePrescaler(uint32_t baudrate,
                                              uint32_t clock_freq) {
  uint32_t prescaler = clock_freq / baudrate;
  if (prescaler <= 2)
    return 0;
  else if (prescaler <= 4)
    return 1;
  else if (prescaler <= 8)
    return 2;
  else if (prescaler <= 16)
    return 3;
  else if (prescaler <= 32)
    return 4;
  else if (prescaler <= 64)
    return 5;
  else if (prescaler <= 128)
    return 6;
  else
    return 7;
}

void SPI_Init(spi_port_t port, spi_config_t *config) {
  SPI_TypeDef *spi = GetSPIBase(port);
  if (!spi) {
    return;
  }

  SPI_EnableClock(port);

  SPI_ConfigPins(port);

  uint32_t clock_freq;
  if (port == SPI_PORT_1) {
    clock_freq = APB2_CLOCK_FREQ;
  } else {
    clock_freq = APB1_CLOCK_FREQ;
  }

  uint8_t br = SPI_CalculateBaudRatePrescaler(config->baudrate, clock_freq);

  uint32_t cr1 = 0;

  cr1 |= SPI_CR1_MSTR; // master mode

  cr1 |= (br << 3);

  if (config->bits == 16) {
    cr1 |= SPI_CR1_DFF;
  }

  if (!config->msb_first) {
    cr1 |= SPI_CR1_LSBFIRST;
  }

  if (config->mode & 0x1) {
    cr1 |= SPI_CR1_CPHA;
  }

  if (config->mode & 0x2) {
    cr1 |= SPI_CR1_CPOL;
  }

  cr1 |= SPI_CR1_SSM;
  cr1 |= SPI_CR1_SSI;

  spi->CR1 = cr1;

  spi->CR1 |= SPI_CR1_SPE;
}

void SPI_SendByte(spi_port_t port, uint8_t data) {
  SPI_TypeDef *spi = GetSPIBase(port);

  if (!spi) {
    return;
  }

  while (!(spi->SR & SPI_SR_TXE))
    ;

  if (spi->CR1 & SPI_CR1_DFF) {
    spi->DR = (uint16_t)data;
  } else {
    *((volatile uint8_t *)&spi->DR) = data;
  }
  while (spi->SR & SPI_SR_BSY)
    ;
}

void SPI_SendData(spi_port_t port, uint8_t *data, uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    SPI_SendByte(port, *(data + i));
  }
}

uint8_t SPI_ReceiveByte(spi_port_t port) { return 0; }

uint8_t SPI_ReceiveData(spi_port_t port, uint8_t *data, uint32_t len) {
  return 0;
}
