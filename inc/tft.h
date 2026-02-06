#ifndef TFT_H
#define TFT_H
#include <stdint.h>

#define TFT_BLACK 0x0000
#define TFT_WHITE 0xFFFF
#define TFT_RED 0xF800

#define TFT_WIDTH 128
#define TFT_HEIGHT 160

void TFT_Init(void);
void TFT_Clear(uint16_t color);
void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void TFT_DrawRect(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h,
                  uint16_t color);
void TFT_SetChar(uint16_t x, uint16_t y, char ch, uint16_t color,
                 uint16_t bgcolor);
#endif
