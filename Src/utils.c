
#include "utils.h"
#include <stdlib.h>

Pixel_t getRandomColor(uint8_t lower, uint8_t upper)
{
  return (Pixel_t) {.red = RAND_IN_RANGE(lower, upper), .green = RAND_IN_RANGE(lower, upper), .blue = RAND_IN_RANGE(lower, upper)};
}
