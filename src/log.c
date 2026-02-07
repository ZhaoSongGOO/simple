#include "log.h"

#include "font.h"
#include "tft.h"
#include <stdint.h>

uint8_t col = 0, row = 0;

volatile uint8_t log_enable = 0;

void log_info(const char *msg) { log(msg, INFO); }
void log_debug(const char *msg) { log(msg, DEBUG); }
void log_warn(const char *msg) { log(msg, WARN); }
void log_error(const char *msg) { log(msg, ERROR); }

extern Log_Typedef Log = {
    .i = log_info, .d = log_debug, .e = log_error, .w = log_warn};

static uint32_t len(const char *str) {
  if (str == (void *)0) {
    return 0;
  }
  uint32_t result = 0;
  while (str[result++] != '\0')
    ;
  return result;
}

void printk(const char *str) {
  uint8_t x = 0, y = 0;
  uint32_t size = len(str);
  for (uint32_t i = 0; i < size; i++) {
    char ch = str[i];
    if (col >= TFT_COL) {
      col = 0;
      row = row + 1;
    }
    if (row >= TFT_ROW) {
      row = 0;
      col = 0;
    }
    x = col * FONT_WIDTH;
    y = row * FONT_HEIGHT;
    TFT_SetChar(x, y, ch, TFT_OLIVE, TFT_BLACK);
    col += 1;
  }
}

void printnk(const char *str, uint32_t size, uint16_t color) {
  uint8_t x = 0, y = 0;
  for (uint32_t i = 0; i < size; i++) {
    char ch = str[i];
    if (col >= TFT_COL) {
      col = 0;
      row = row + 1;
    }
    if (row >= TFT_ROW) {
      row = 0;
      col = 0;
    }
    x = col * FONT_WIDTH;
    y = row * FONT_HEIGHT;
    TFT_SetChar(x, y, ch, color, TFT_BLACK);
    col += 1;
  }
}

void log(const char *str, LOG_LEVEL level) {
  if (log_enable == 0) {
    return;
  }
  uint32_t size = len(str);
  if (size >= TFT_COL) {
    size = TFT_COL;
  }
  if (col != 0) {
    row += 1;
    col = 0;
  }
  uint16_t color;
  switch (level) {
  case INFO:
    color = TFT_WHITE;
    break;
  case DEBUG:
    color = TFT_BLUE;
    break;
  case WARN:
    color = TFT_ORANGE;
    break;
  case ERROR:
  default:
    color = TFT_RED;
    break;
  }
  printnk(str, size, color);
}

void Log_Init() { log_enable = 1; }
