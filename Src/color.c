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


// Convert HSV values to RGB equivalents. 
// Parameters: double h, a number between 0 and 360 (degrees)
//             double s, a number between 0 and 100 (percent)
//             double v, a number between 0 and 100 (percent)
//             double *r, *g, *b pointers that get populated with rgb values
void HsvToRgb(double h, double s, double v, double *r, double *g, double *b)
{
  int i;
  double f, p, q, t;

  if (h > 360)
    h = fmod(h, 360);

  if (h == 360)
    h = 0;
  else
    h = h / 60.0;

  i = (int) trunc(h);
  f = h - i;
  p = v * (1.0 - s);
  q = v * (1.0 - (s * f));
  t = v * (1.0 - (s * (1.0 - f)));

  switch (i)
  {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;

		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;

		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;

		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;

		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;

		default:
			*r = v;
			*g = p;
			*b = q;
			break;
  }
}

void RgbToHsvStruct(RgbColor_t *rgb, HsvColor_t *hsv)
{

}

void HsvToRgbStruct(HsvColor_t *hsv, RgbColor_t *rgb)
{
  int i;
  double f, p, q, t;
  double h = hsv->h;
  double s = hsv->s;
  double v = hsv->v;

  if (h > 360)
    h = fmod(h, 360);

  if (h == 360)
    h = 0;
  else
    h = h / 60.0;

  i = (int) trunc(h);
  f = h - i;
  p = v * (1.0 - s);
  q = v * (1.0 - (s * f));
  t = v * (1.0 - (s * (1.0 - f)));

  switch (i)
  {
		case 0:
			rgb->r = v;
			rgb->g = t;
			rgb->b = p;
			break;

		case 1:
			rgb->r = q;
			rgb->g = v;
			rgb->b = p;
			break;

		case 2:
			rgb->r = p;
			rgb->g = v;
			rgb->b = t;
			break;

		case 3:
			rgb->r = p;
			rgb->g = q;
			rgb->b = v;
			break;

		case 4:
			rgb->r = t;
			rgb->g = p;
			rgb->b = v;
			break;

		default:
			rgb->r = v;
			rgb->g = p;
			rgb->b = q;
			break;
  }
}
