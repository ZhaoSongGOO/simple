#ifndef TFT_H
#define TFT_H
#include "font.h"
#include <stdint.h>

#define TFT_BLACK 0x0000
#define TFT_WHITE 0xFFFF
#define TFT_RED 0xF800
#define TFT_GREEN 0x07E0
#define TFT_BLUE 0x001F
#define TFT_YELLOW 0xFFE0
#define TFT_MAGENTA 0xF81F
#define TFT_CYAN 0x07FF

#define TFT_NAVY 0x000F
#define TFT_DARKGREEN 0x03E0
#define TFT_DARKCYAN 0x03EF
#define TFT_MAROON 0x7800
#define TFT_PURPLE 0x780F
#define TFT_OLIVE 0x7BE0
#define TFT_LIGHTGREY 0xC618
#define TFT_DARKGREY 0x7BEF

#define TFT_ORANGE 0xFD20
#define TFT_GREENYELLOW 0xAFE5
#define TFT_PINK 0xF81F
#define TFT_SILVER 0xC618
#define TFT_SKYBLUE 0x867D

#define TFT_WIDTH 128
#define TFT_HEIGHT 160

#define TFT_ROW (TFT_HEIGHT / FONT_HEIGHT)
#define TFT_COL (TFT_WIDTH / FONT_WIDTH)

void TFT_Init(void);
void TFT_Clear(uint16_t color);
void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void TFT_DrawRect(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h,
                  uint16_t color);
void TFT_SetChar(uint16_t x, uint16_t y, char ch, uint16_t color,
                 uint16_t bgcolor);
#endif
