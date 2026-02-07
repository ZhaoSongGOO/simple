#include "tft.h"
#include "gpio.h"
#include "spi.h"
#include "stm32f103.h"
#include "system.h"

#define TFT_CS_PORT GPIO_PORT_B
#define TFT_CS_PIN 8
#define TFT_DC_PORT GPIO_PORT_B
#define TFT_DC_PIN 7
#define TFT_RESET_PORT GPIO_PORT_B
#define TFT_RESET_PIN 6
#define TFT_BL_PORT GPIO_PORT_B
#define TFT_BL_PIN 9

#define TFT_SPI_PORT SPI_PORT_3

#define TFT_CMD 0
#define TFT_DATA 1

static void TFT_WriteCommand(uint8_t cmd) {
  GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, TFT_CMD);
  GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, 0);
  SPI_SendByte(TFT_SPI_PORT, cmd);
  GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, 1);
}

static void TFT_WriteData(uint8_t data) {
  GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, TFT_DATA);
  GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, 0);
  SPI_SendByte(TFT_SPI_PORT, data);
  GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, 1);
}

static void TFT_WriteData16(uint16_t data) {
  TFT_WriteData(data >> 8);
  TFT_WriteData(data & 0xFF);
}

static void TFT_Reset(void) {
  GPIO_WritePin(TFT_RESET_PORT, TFT_RESET_PIN, 0);
  Delay_ms(10);
  GPIO_WritePin(TFT_RESET_PORT, TFT_RESET_PIN, 1);
  Delay_ms(10);
}
void TFT_Init(void) {
  GPIO_EnableClock(TFT_CS_PORT);
  // GPIO_EnableClock(TFT_DC_PORT);
  // GPIO_EnableClock(TFT_RESET_PORT);
  // GPIO_EnableClock(TFT_BL_PORT);

  GPIO_SetOutput(TFT_CS_PORT, TFT_CS_PIN, GPIO_SPEED_50MHZ);
  GPIO_SetOutput(TFT_DC_PORT, TFT_DC_PIN, GPIO_SPEED_50MHZ);
  GPIO_SetOutput(TFT_RESET_PORT, TFT_RESET_PIN, GPIO_SPEED_50MHZ);
  GPIO_SetOutput(TFT_BL_PORT, TFT_BL_PIN, GPIO_SPEED_50MHZ);

  GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, 1);
  GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, TFT_CMD);
  GPIO_WritePin(TFT_BL_PORT, TFT_BL_PIN, 1);

  TFT_Reset();

  spi_config_t spi_config = {
      .baudrate = 10000000, .mode = 0, .bits = 8, .msb_first = 1};

  SPI_Init(TFT_SPI_PORT, &spi_config);

  TFT_WriteCommand(0x01);
  Delay_ms(120);

  TFT_WriteCommand(0x11);
  Delay_ms(120);

  TFT_WriteCommand(0x3A);
  TFT_WriteData(0x05);

  TFT_WriteCommand(0x29);
  Delay_ms(10);
}
void TFT_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  TFT_WriteCommand(0x2A);
  TFT_WriteData16(x1);
  TFT_WriteData16(x2);

  TFT_WriteCommand(0x2B);
  TFT_WriteData16(y1);
  TFT_WriteData16(y2);

  TFT_WriteCommand(0x2C);
}

void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
  if (x >= TFT_WHITE || y >= TFT_HEIGHT) {
    return;
  }

  TFT_SetWindow(x, y, x, y);
  TFT_WriteData16(color);
}

void TFT_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                  uint16_t color) {
  int dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
  int dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);

  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  int x = x1, y = y1;

  while (1) {
    TFT_DrawPixel(x, y, color);

    if (x == x2 && y == y2) {
      break;
    }

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x += sx;
    }

    if (e2 < dx) {
      err += dx;
      y += sy;
    }
  }
}

void TFT_DrawRect(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h,
                  uint16_t color) {
  TFT_DrawLine(x1, y1, x1 + w - 1, y1, color);
  TFT_DrawLine(x1, y1, x1, y1 + h - 1, color);
  TFT_DrawLine(x1 + w - 1, y1, x1 + w - 1, y1 + h - 1, color);
  TFT_DrawLine(x1, y1 + h - 1, x1 + w - 1, y1 + h - 1, color);
}

void TFT_Clear(uint16_t color) {
  TFT_SetWindow(0, 0, TFT_WIDTH, TFT_HEIGHT); // 1.8寸屏分辨率通常是 128x160
  for (uint32_t i = 0; i < TFT_WIDTH * TFT_HEIGHT; i++) {
    TFT_WriteData16(color);
  }
}

void TFT_SetChar(uint16_t x, uint16_t y, char ch, uint16_t color,
                 uint16_t bgcolor) {
  uint8_t i, j, temp;
  if (ch < ' ' || ch > 'z')
    ch = ' ';

  // 偏移量计算：每个字符16字节
  uint32_t offset = (uint32_t)(ch - ' ') * 16;

  // 2. 划定一个 8x16 的矩形窗口
  TFT_SetWindow(x, y, x + 8 - 1, y + 16 - 1);

  // 3. 连续写入 128 个像素的颜色
  for (i = 0; i < 16; i++) {      // 遍历 16 行
    temp = FONT_8x16[offset + i]; // 取出一行的点阵 (8位)
    for (j = 0; j < 8; j++) {     // 遍历这一行的 8 个像素
      if (temp & 0x80) {          // 如果最高位是 1，画字体颜色
        TFT_WriteData16(color);
      } else { // 如果是 0，画背景颜色
        TFT_WriteData16(bgcolor);
      }
      temp <<= 1; // 左移处理下一个像素
    }
  }
}
