#include "gpio.h"
#include "stm32f103.h"
#include <stdint.h>

#define GPIO_MODE_INPUT 0x0
#define GPIO_MODE_OUTPUT_10MHZ 0x1 // 01
#define GPIO_MODE_OUTPUT_2MHZ 0x2  // 10
#define GPIO_MODE_OUTPUT_50MHZ 0x3 // 11

#define GPIO_CNF_INPUT_ANALOG 0x0
#define GPIO_CNF_INPUT_FLOAT 0x1
#define GPIO_CNF_INPUT_PULL 0x2
#define GPIO_CNF_OUTPUT_PP 0x0
#define GPIO_CNF_OUTPUT_OD 0x1
#define GPIO_CNF_AF_PP 0x2
#define GPIO_CNF_AF_OD 0x3

static GPIO_TypeDef *GetGPIOBase(gpio_port_t port) {
  switch (port) {
  case GPIO_PORT_A:
    return GPIOA;
  case GPIO_PORT_B:
    return GPIOB;
  case GPIO_PORT_C:
    return GPIOC;
  default:
    return (void *)0;
  }
}

void GPIO_EnableClock(gpio_port_t port) {
  switch (port) {
  case GPIO_PORT_A:
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    break;
  case GPIO_PORT_B:
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    break;
  case GPIO_PORT_C:
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    break;
  default:
    break;
  }
}

void GPIO_ConfigPin(gpio_port_t port, uint8_t pin, gpio_mode_t mode,
                    gpio_cnf_t cnf) {
  GPIO_TypeDef *gpio = GetGPIOBase(port);
  if (!gpio) {
    return;
  }

  uint32_t mode_val = (uint32_t)mode;
  uint32_t cnf_val = (uint32_t)cnf;
  uint32_t config = (cnf_val << 2) | mode_val;
  if (pin < 8) {
    uint32_t shift = pin * 4;
    gpio->CRL &= ~(0xFUL << shift);
    gpio->CRL |= (config << shift);
  } else {
    uint32_t shift = (pin - 8) * 4;
    gpio->CRH &= ~(0xFUL << shift);
    gpio->CRH |= (config << shift);
  }
}

void GPIO_SetOutput(gpio_port_t port, uint8_t pin, gpio_speed_t speed) {
  GPIO_EnableClock(port);
  uint32_t mode;
  switch (speed) {
  case GPIO_SPEED_10MHZ:
    mode = GPIO_MODE_OUTPUT_10MHZ;
    break;
  case GPIO_SPEED_2MHZ:
    mode = GPIO_MODE_OUTPUT_2MHZ;
    break;
  case GPIO_SPEED_50MHZ:
    mode = GPIO_MODE_OUTPUT_50MHZ;
    break;
  default:
    mode = GPIO_MODE_OUTPUT_2MHZ;
    break;
  }
  GPIO_ConfigPin(port, pin, mode, GPIO_CNF_OUTPUT_PP);
}

void GPIO_SetInput(gpio_port_t port, uint8_t pin, gpio_pull_t pull) {
  GPIO_EnableClock(port);
  uint32_t cnf;
  switch (pull) {
  case GPIO_PULL_NONE:
    cnf = GPIO_CNF_INPUT_FLOAT;
    break;
  case GPIO_PULL_UP:
  case GPIO_PULL_DOWN:
    cnf = GPIO_CNF_INPUT_PULL;
    break;
  default:
    cnf = GPIO_CNF_INPUT_FLOAT;
    break;
  }
  GPIO_ConfigPin(port, pin, GPIO_MODE_INPUT, cnf);
  if (pull == GPIO_PULL_UP) {
    GPIO_WritePin(port, pin, 1);
  } else if (pull == GPIO_PULL_DOWN) {
    GPIO_WritePin(port, pin, 0);
  }
}

void GPIO_WritePin(gpio_port_t port, uint8_t pin, uint8_t value) {
  GPIO_TypeDef *gpio = GetGPIOBase(port);
  if (!gpio) {
    return;
  }

  if (value) {
    gpio->BSRR = (1UL << pin);
  } else {
    gpio->BRR = (1UL << pin);
  }
}

uint8_t GPIO_ReadPin(gpio_port_t port, uint8_t pin) {
  GPIO_TypeDef *gpio = GetGPIOBase(port);
  if (!gpio) {
    return 0;
  }
  return (gpio->IDR >> pin) & 0x1;
}

void GPIO_TogglePin(gpio_port_t port, uint8_t pin) {
  GPIO_TypeDef *gpio = GetGPIOBase(port);
  if (!gpio) {
    return;
  }

  if (gpio->ODR & (1UL << pin)) {
    gpio->BRR = (1UL << pin);
  } else {
    gpio->BSRR = (1UL << pin);
  }
}

void GPIO_SetAlternateFunction(gpio_port_t port, uint8_t pin,
                               gpio_speed_t speed) {
  GPIO_EnableClock(port);
  uint32_t mode;
  switch (speed) {
  case GPIO_SPEED_10MHZ:
    mode = GPIO_MODE_OUTPUT_10MHZ;
    break;
  case GPIO_SPEED_2MHZ:
    mode = GPIO_MODE_OUTPUT_2MHZ;
    break;
  case GPIO_SPEED_50MHZ:
    mode = GPIO_MODE_OUTPUT_50MHZ;
    break;
  default:
    mode = GPIO_MODE_OUTPUT_2MHZ;
    break;
  }
  GPIO_ConfigPin(port, pin, mode, GPIO_CNF_AF_PP);
}
