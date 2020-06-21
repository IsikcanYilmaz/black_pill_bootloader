#include "addressable_led_driver.h"
#include "color.h"
#include <math.h>
#include <stdlib.h>


//https://en.wikipedia.org/wiki/HSL_and_HSV
/*
 * COLOR
 *
 * This file is to provide functionality to handle HSL (Hue Saturation Lightness)
 * and HSV (Hue Saturation Value) representations of RGB pixels. 
 *
 */

void HsvToRgb(double h, double s, double v, double *r, double *g, double *b)
{
  double c = v * s;
  double m = v - c;
  double x = c * (1 - abs(fmod((h / 60), 2) - 1));
  if (h >= 0 && h < 60)
  {
    *r = c + m;
    *g = x + m;
    *b = m;
  }
  else if (h >= 60 && h < 120)
  {
    *r = x + m;
    *g = c + m;
    *b = m;
  }
  else if (h >= 120 && h < 180)
  {
    *r = m;
    *g = c + m;
    *b = x + m;
  }
  else if (h >= 180 && h < 240)
  {
    *r = m;
    *g = x + m;
    *b = c + m;
  }
  else if (h >= 240 && h < 300)
  {
    *r = x + m;
    *g = m;
    *b = c + m;
  }
  else if (h >= 300 && h < 360)
  {
    *r = c + m;
    *g = m;
    *b = x + m;
  }
  else
  {
    *r = m;
    *g = m;
    *b = m;
  }
}
