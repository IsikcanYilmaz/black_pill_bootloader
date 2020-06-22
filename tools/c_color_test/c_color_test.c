#include "color.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct {
  uint8_t upperBrightness;
  uint8_t loweBrightness;
} TestContext_t;

TestContext_t context = {255, 0};

static void normalizeHsv(double h, double s, double v, double *hnorm, double *snorm, double *vnorm)
{
  // H is a number between 0 and 360 (angles)
  // Hnorm will be between 0 and 1
  *hnorm = h / 360.0;

  // S is a number between 0 and 100 (percent)
  // Snorm will be between 0 and 1
  *snorm = s / 100.0;

  // V is a number between 0 and 100 (percent)
  // Vnorm will be between 0 and 1
  *vnorm = v / 100.0;
}

static void normalizeRgb(double r, double g, double b, uint8_t *rnorm, uint8_t *gnorm, uint8_t *bnorm)
{
  // r, g, b are numbers between 0 and 1.
  // rnorm, gnorm, bnorm will be numbers between 0 and upperBrightness
  *rnorm = r * context.upperBrightness;
  *gnorm = g * context.upperBrightness;
  *bnorm = b * context.upperBrightness;
}

int main(void)
{
  double r = 0.0;
  double g = 0.0;
  double b = 0.0;

  double h = 85;
  double s = 0.79;
  double v = 1;

  printf("%f %f %f \n", h, s, v);

  double hnorm = 0.0;
  double snorm = 0.0;
  double vnorm = 0.0;

  normalizeHsv(h, s, v, &hnorm, &snorm, &vnorm);

  uint8_t rscaled = 0;
  uint8_t gscaled = 0;
  uint8_t bscaled = 0;

  HsvToRgb(h, s, v, &r, &g, &b);
 
  normalizeRgb(r, g, b, &rscaled, &gscaled, &bscaled);

  printf("h:%f s:%f v:%f \nhnorm:%f snorm:%f vnorm:%f \nr:%f g:%f b:%f \nrscaled:%d gscaled:%d bscaled:%d\n", h, s, v, hnorm, snorm, vnorm, r, g, b, rscaled, gscaled, bscaled);

  return 0;
}
