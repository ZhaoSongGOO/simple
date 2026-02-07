#include "gpio.h"
#include "log.h"
#include "system.h"
#include "tft.h"
#include "user.h"

int main() {
  print("Enter Main!");
  print("Enable LED!");
  GPIO_SetOutput(GPIO_PORT_A, 8, GPIO_SPEED_2MHZ);
  GPIO_WritePin(GPIO_PORT_A, 8, 1);
  while (1) {
    print("Change LED!");
    GPIO_TogglePin(GPIO_PORT_A, 8);
    Delay_ms(500);
  }
}