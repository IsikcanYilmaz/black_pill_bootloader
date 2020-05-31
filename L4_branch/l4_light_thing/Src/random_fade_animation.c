#include "random_fade_animation.h"
#include "addressable_led_manager.h"
#include "utils.h"
#include <stdlib.h>

/*
 *  RANDOM FADE ANIMATION MODULE 
 *
 *  This animation module will pick pixels that are not lit, randomly assign them
 *  some color and a decrementRate, and decrements them every update by that value.
 *
 *  The random colors are picked between the $lower and $upper variables that currently
 *  reside in the update function. 
 *
 *  There's also a WAIT_TIME that has the pixel wait at 0 brightness for that many update
 *  cycles. looks neat
 *
 *  Average current consumptions between ranges of $lower-$upper values with 1-10 $waitTime, 1-1 $decrementRate, $cutoff 0
 *  (Note that this animation uses 2 color channels at one time. so this survey is done with 2 channels)
 *  
 *  [l]   [u]   [Avg Current]  
 *  0     5   : 30mA           
 *  5     5   : 30-40mA
 *  5    10   : 40mA
 *  10   10   : 40-50mA
 *  20   20   : 70-90mA
 *  30   30   : 100-120mA
 *  40   40   : 150-170mA
 *  50   50   : 90-120mA  (Increased decrement rate to 1-10, wait time to 1-30 at this point and on)
 *  70   70   : 150-200mA 
 *  0    70   : 70-100mA
 *  100 100   : 230-300mA
 *  50  100   : 150-190mA
 *  50  100   : 90-110mA  (Increased decrement rate to 10-30 at this point and on)
 *  100 100   : 120-150mA
 */

typedef struct {
  uint8_t numPanels;
  uint8_t numLeds;

  // This will be a ptr to an array of consecutive panel objects
  AddrLEDPanel_t *panels; 

  // Below is tobe used as storage for metadata about the Pixels that this animation module will keep
  RandomFadePixelData_t *pixelDataPtr;
  
  // Below are animation config variables
  // lower and upper limits for the random numbers
  uint8_t lowerBrightness;
  uint8_t upperBrightness;
  uint8_t lowerDecrementRate;
  uint8_t upperDecrementRate;
  uint8_t lowerWaitTime;
  uint8_t upperWaitTime;
  uint8_t cutoff; // < decrement pixel until this value
  bool singleValForAllChannels; // < same value for all of the rgb channels, or not

  // Pointer to the strip. assuming there's only one
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
  
  context.lowerBrightness    = 10;
  context.upperBrightness    = 10;
  context.lowerDecrementRate = 1;
  context.upperDecrementRate = 2;
  context.lowerWaitTime      = 1;
  context.upperWaitTime      = 30;
  context.cutoff             = 0;
  context.singleValForAllChannels = false;

  // Start pixels with a wait time so all of them dont go full brightness at the same time at the beginning
  for (int i = 0; i < context.numLeds; i++)
  {
    RandomFadePixelData_t *currData = (RandomFadePixelData_t *) &context.pixelDataPtr[i];
    currData->waitTime = RAND_IN_RANGE(context.lowerWaitTime, context.upperWaitTime);
    currData->decrementRate = RAND_IN_RANGE(context.lowerDecrementRate, context.upperDecrementRate);
  }
}


void Animation_RandomFade_Update(void)
{
  static uint32_t count = 0;
  for (int i = 0; i < context.numLeds; i++)
  {
    Pixel_t *currPixel = (Pixel_t *) &context.stripBegin->pixels[i];
    RandomFadePixelData_t *currData = (RandomFadePixelData_t *) &context.pixelDataPtr[i];
    uint8_t decrementRate = currData->decrementRate;
    
    // If all colors reached 0, check for wait time, decrement it, and if it too 
    // has reached zero, find a new random value.
    // If all colors havent reached 0, decrement them
    if (currPixel->red <= context.cutoff && currPixel->green <= context.cutoff && currPixel->blue <= context.cutoff)
    {
      if (currData->waitTime > 0)
      {
        currData->waitTime--;
        continue;
      }
      currData->decrementRate = RAND_IN_RANGE(context.lowerDecrementRate, context.upperDecrementRate);

      if (count % 200 < 100)
      {
        uint8_t r1 = RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness);
        uint8_t r2 = (context.singleValForAllChannels) ? r1 : RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness);
        currPixel->red = 0; 
        currPixel->green = r1;
        currPixel->blue = r2;
      }
      else if (count % 200 < 200)
      {
        uint8_t r1 = RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness);
        uint8_t r2 = (context.singleValForAllChannels) ? r1 : RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness);
        currPixel->red = r1;
        currPixel->green = 0; 
        currPixel->blue = r2;
      }
      else
      {
        uint8_t r1 = RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness);
        uint8_t r2 = (context.singleValForAllChannels) ? r1 : RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness);
        currPixel->red = r1;
        currPixel->green = r1;
        currPixel->blue = 0;
      }
      
      currData->waitTime = RAND_IN_RANGE(context.lowerWaitTime, context.upperWaitTime);
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
