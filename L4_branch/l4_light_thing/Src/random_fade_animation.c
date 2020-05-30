#include "random_fade_animation.h"
#include "addressable_led_manager.h"
#include "utils.h"
#include <stdlib.h>

typedef struct {
  uint8_t numPanels;
  uint8_t numLeds;

  // This will be a ptr to an array of consecutive panel objects
  AddrLEDPanel_t *panels; 

  // Below is tobe used as storage for metadata about the Pixels that this animation module will keep
  RandomFadePixelData_t *pixelDataPtr;

  AddrLEDStrip_t *stripBegin;
} AnimationContext_t; 



static AnimationContext_t context;


void Animation_RandomFade_Init(AddrLEDPanel_t *panels, uint8_t numPanels, RandomFadePixelData_t *pixelDataPtr)
{
  context.panels = panels;
  context.pixelDataPtr = pixelDataPtr;
  context.stripBegin = panels->strip;
  context.numPanels = numPanels;
  context.numLeds = panels->numLeds * numPanels;
}


void Animation_RandomFade_Update(void)
{
  const uint8_t lower = 0;
  const uint8_t upper = 10;

  static uint32_t count = 0;
  for (int i = 0; i < context.numLeds; i++)
  {
    Pixel_t *currPixel = (Pixel_t *) &context.stripBegin->pixels[i];
    RandomFadePixelData_t *currData = (RandomFadePixelData_t *) &context.pixelDataPtr[i];
    uint8_t decrementRate = currData->decrementRate;

    if (currPixel->red <= lower && currPixel->green <= lower && currPixel->blue <= lower)
    {
      if (currData->waitTime > 0)
      {
        currData->waitTime--;
        continue;
      }
      currData->decrementRate = RAND_IN_RANGE(1, 1);

      if (count % 200 < 100)
      {
        uint8_t r1 = RAND_IN_RANGE(lower, upper);
        uint8_t r2 = RAND_IN_RANGE(lower, upper);
        currPixel->red = 0; 
        currPixel->green = r1;
        currPixel->blue = r2;
      }
      else if (count % 200 < 200)
      {
        uint8_t r1 = RAND_IN_RANGE(lower, upper);
        uint8_t r2 = RAND_IN_RANGE(lower, upper);
        currPixel->red = r1;
        currPixel->green = 0; 
        currPixel->blue = r2;
      }
      else
      {
        uint8_t r1 = RAND_IN_RANGE(lower, upper);
        uint8_t r2 = RAND_IN_RANGE(lower, upper);
        currPixel->red = r1;
        currPixel->green = r1;
        currPixel->blue = 0;
      }

      currData->waitTime = RAND_IN_RANGE(1, 5);
    }
    else
    {
      currPixel->red = (currPixel->red > decrementRate) ? currPixel->red - decrementRate : 0;
      currPixel->green = (currPixel->green > decrementRate) ? currPixel->green - decrementRate : 0;
      currPixel->blue = (currPixel->blue > decrementRate) ? currPixel->blue - decrementRate : 0;
    }
  }
  count++;
}
