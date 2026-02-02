/**
 * @file gpio.h
 * @brief GPIO Driver abstraction for STM32F103 (Cortex-M3).
 * * This file contains definitions for configuring and manipulating
 * General Purpose Input/Output (GPIO) pins, specifically targeting
 * the Register Map of the STM32F103 series.
 */

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/** * @brief Available GPIO ports on the STM32F103.
 */
typedef enum { GPIO_PORT_A = 0, GPIO_PORT_B = 1, GPIO_PORT_C = 2 } gpio_port_t;

/** * @brief GPIO Mode Configuration (maps to the 'MODE' bits in CRL/CRH
 * registers).
 */
typedef enum {
  GPIO_MODE_INPUT = 0, // 00: Input mode (reset state)
  GPIO_MODE_OUTPUT =
      1 // Note: STM32F103 has multiple output speeds (see gpio_speed_t)
} gpio_mode_t;

/** * @brief Configuration bits (maps to the 'CNF' bits in CRL/CRH registers).
 * The meaning changes depending on whether the pin is Input or Output.
 */
typedef enum {
  /* Output configurations */
  GPIO_CNF_OUTPUT_PP = 0, // General purpose output push-pull
  GPIO_CNF_OUTPUT_OD = 1, // General purpose output Open-drain

  /* Input configurations */
  GPIO_CNF_INPUT_ANALOG = 0, // Analog mode
  GPIO_CNF_INPUT_FLOAT = 1,  // Floating input (reset state)
  GPIO_CNF_INPUT_PULL = 2    // Input with pull-up / pull-down
} gpio_cnf_t;

/** * @brief Output Speed settings (Max frequency).
 */
typedef enum {
  GPIO_SPEED_10MHZ = 0, // Mode bits 01
  GPIO_SPEED_2MHZ = 1,  // Mode bits 10
  GPIO_SPEED_50MHZ = 2  // Mode bits 11
} gpio_speed_t;

/** * @brief Internal Pull-up/Pull-down selection.
 */
typedef enum {
  GPIO_PULL_NONE = 0,
  GPIO_PULL_UP = 1,
  GPIO_PULL_DOWN = 2
} gpio_pull_t;

/* --- Function Prototypes --- */

/**
 * @brief Enables the APB2 peripheral clock for the specified GPIO port.
 */
void GPIO_EnableClock(gpio_port_t port);

/**
 * @brief Basic pin configuration for Mode and CNF bits.
 */
void GPIO_ConfigPin(gpio_port_t port, uint8_t pin, gpio_mode_t mode,
                    gpio_cnf_t cnf);

/**
 * @brief Configures a pin as a General Purpose Output with a specific speed.
 */
void GPIO_SetOutput(gpio_port_t port, uint8_t pin, gpio_speed_t speed);

/**
 * @brief Configures a pin as an Input with Pull-up, Pull-down, or Floating.
 */
void GPIO_SetInput(gpio_port_t port, uint8_t pin, gpio_pull_t pull);

/**
 * @brief Writes a high (1) or low (0) value to the specified pin via the ODR
 * register.
 */
void GPIO_WritePin(gpio_port_t port, uint8_t pin, uint8_t value);

/**
 * @brief Reads the current logic level of the pin from the IDR register.
 */
uint8_t GPIO_ReadPin(gpio_port_t port, uint8_t pin);

/**
 * @brief Inverts the current state of the output pin.
 */
void GPIO_TogglePin(gpio_port_t port, uint8_t pin);

/**
 * @brief Configures the pin for Alternate Function (e.g., UART, SPI, PWM).
 */
void GPIO_SetAlternateFunction(gpio_port_t port, uint8_t pin,
                               gpio_speed_t speed);

#endif /* GPIO_H */