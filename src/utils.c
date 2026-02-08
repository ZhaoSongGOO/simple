#include "utils.h"

uint32_t len(const char *str) {
  if (str == (void *)0) {
    return 0;
  }
  uint32_t result = 0;
  while (str[result++] != '\0')
    ;
  return result;
}