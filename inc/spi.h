#ifndef SPI_H
#define SPI_H

#include <stdint.h>

typedef enum { SPI_PORT_1 = 1, SPI_PORT_2 = 2, SPI_PORT_3 = 3 } spi_port_t;

typedef struct {
  uint32_t baudrate; // 波特率
  uint8_t mode;      // SPI 模式 0-3
  uint8_t bits;      // 数据位
  uint8_t msb_first; // MSB 优先， 1 = MSB, 0 = LSB
} spi_config_t;

void SPI_Init(spi_port_t port, spi_config_t *config);

void SPI_SendByte(spi_port_t port, uint8_t data);

void SPI_SendData(spi_port_t port, uint8_t *data, uint32_t len);

uint8_t SPI_ReceiveByte(spi_port_t port);

uint8_t SPI_ReceiveData(spi_port_t port, uint8_t *data, uint32_t len);
#endif
