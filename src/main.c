#include "gpio.h"
#include "system.h"
#include "tft.h"

int main() {
  GPIO_SetOutput(GPIO_PORT_A, 8, GPIO_SPEED_2MHZ);
  GPIO_WritePin(GPIO_PORT_A, 8, 1);

  TFT_Init();
  TFT_Clear(TFT_BLACK);
  TFT_DrawRect(10, 10, 50, 100, TFT_RED);
  TFT_DrawRect(0, 0, TFT_WIDTH, TFT_HEIGHT, TFT_WHITE);
  TFT_SetChar(20, 20, 'A', TFT_RED, TFT_BLACK);
  TFT_SetChar(28, 20, 'B', TFT_RED, TFT_BLACK);
  TFT_SetChar(36, 20, 'Z', TFT_RED, TFT_BLACK);
  TFT_SetChar(44, 20, '[', TFT_RED, TFT_BLACK);
  TFT_SetChar(52, 20, '\\', TFT_RED, TFT_BLACK);
  TFT_SetChar(60, 20, 'a', TFT_RED, TFT_BLACK);
  while (1) {
    GPIO_TogglePin(GPIO_PORT_A, 8);
    Delay_ms(500);
  }
}