#include "gpio.h"
#include "system.h"

int main() {
  GPIO_SetOutput(GPIO_PORT_A, 8, GPIO_SPEED_2MHZ);
  while (1) {
    GPIO_TogglePin(GPIO_PORT_A, 8);
    Delay_ms(500);
  }
}