#include "gpio.h"
#include "log.h"
#include "system.h"
#include "tft.h"

int main() {
  Log.i("Enter Main INFO");
  GPIO_SetOutput(GPIO_PORT_A, 8, GPIO_SPEED_2MHZ);
  GPIO_WritePin(GPIO_PORT_A, 8, 1);
  while (1) {
    GPIO_TogglePin(GPIO_PORT_A, 8);
    Delay_ms(500);
  }
}