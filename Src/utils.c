
#include "utils.h"
#include <stdlib.h>

Pixel_t getRandomColor(uint8_t lower, uint8_t upper)
{
  return (Pixel_t) {.red = RAND_IN_RANGE(lower, upper), .green = RAND_IN_RANGE(lower, upper), .blue = RAND_IN_RANGE(lower, upper)};
}

bool stringsSame(char *str1, char *str2)
{
  bool same = true;
  char *p1 = str1;
  char *p2 = str2;
  while (*p1 != '\0' && *p2 != '\0')
  {
    if (*str1 != *str2)
    {
      return false;
    }
    p1++;
    p2++;
  }
  return true;
}
