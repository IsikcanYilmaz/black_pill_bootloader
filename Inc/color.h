#include <stdint.h>

#define H_MAX 360
#define S_MAX 100
#define V_MAX 100
#define RGB_MAX 255

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} RgbColor_t;

typedef struct {
  uint16_t h;
  uint8_t s;
  uint8_t v;
} HsvColor_t;

void HsvToRgb(double h, double s, double v, double *r, double *g, double *b);
void RgbToHsvStruct(RgbColor_t *rgb, HsvColor_t *hsv);
void HsvToRgbStruct(HsvColor_t *hsv, RgbColor_t *rgb);
