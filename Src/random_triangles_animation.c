#include "random_fade_animation.h"
#include "addressable_led_manager.h"
#include "utils.h"
#include "color.h"
#include <stdlib.h>

#define HSV_TEST true

/*
 *  RANDOM TRIANGLES ANIMATION MODULE 
 *

 */

typedef struct {
  uint8_t numPanels;
  uint8_t numLeds;
  AnimationState_e state;

  // This will be a ptr to an array of consecutive panel objects
  AddrLEDPanel_t *panels; 

  // Below is tobe used as storage for metadata about the Pixels that this animation module will keep
  RandomFadePixelData_t *pixelDataPtr;
  
  // Below are animation config variables
  // lower and upper limits for the random numbers
  uint8_t upperBrightness;
  uint8_t lowerBrightness;

  // Pointer to the strip. assuming there's only one
  AddrLEDStrip_t *stripBegin;
} AnimationContext_t; 

static AnimationContext_t context;

static void randomTriangles1();
static void randomTriangles2();
static void rampUp();
static void rampDown();

void Animation_RandomTriangles_Init(AddrLEDPanel_t *panels, uint8_t numPanels, RandomFadePixelData_t *pixelDataPtr)
{
  context.panels = panels;
  context.pixelDataPtr = pixelDataPtr;
  context.stripBegin = panels->strip;
  context.numPanels = numPanels;
  context.numLeds = panels->numLeds * numPanels;

  context.lowerBrightness = 0;
  context.upperBrightness = 225;
}

void Animation_RandomTriangles_SendMessage(AnimationMessage_t *message)
{
  // TODO
}

AnimationState_e Animation_RandomTriangles_GetState(void)
{
  return context.state;
}

void Animation_RandomTriangles_Update(void)
{
  switch(context.state)
  {
    case(RAMPING_UP):
      {
        rampUp();
        break;
      }
    case(RUNNING):
      {
        randomTriangles1();
        break;
      }
    case(RAMPING_DOWN):
      {
        rampDown();
        break;
      }
    case(STOPPED):
      {
        //
        break;
      }
    default:
      break;
  }
}

static void rampUp(void)
{

}

static void rampDown(void)
{

}

static void randomTriangles1(void)
{
  static double r = 0.0;
  static double g = 0.0;
  static double b = 0.0;

  static uint8_t rscaled, gscaled, bscaled;

  static double rowHueOffset = 0;
  static bool rowHueOffsetAdd = true;
  static double rowHueOffsetRate = 0.01;

  static double h = 0.0;
  static double s = 0.999;
  static double v = 0.100;

  static double hrate = 0.5;
  static double srate = 0.0;
  static double vrate = 0;

  static bool hadd = true;
  static bool sadd = true;
  static bool vadd = true;

  // Now color all our leds with this color
  for (int panel = 0; panel < NUM_SIDES; panel++)
  { 
    // TODO // Theres heavy inefficient code below. i'm just impatient now and i know future jon will take care of it
    if (panel == TOP)
    {
      for (int y = 0; y < NUM_LEDS_PER_PANEL_SIDE; y++)
      {  
        for (int x = 0; x < NUM_LEDS_PER_PANEL_SIDE; x++)
        {
          // If top panel, outer square, or not
          if (y == 0 || y == 3 || (y == 1 && (x == 0 || x == 3)) || (y == 2 && (x == 0 || x == 3)))
          {
            HsvToRgb(h + (rowHueOffset * 2), s, v, &r, &g, &b);
          }
          else
          {
            HsvToRgb(h + (rowHueOffset * 1), s, v, &r, &g, &b);
          }
          rscaled = (uint8_t) (r * context.upperBrightness);
          gscaled = (uint8_t) (g * context.upperBrightness);
          bscaled = (uint8_t) (b * context.upperBrightness);
          Pixel_t * currPixel = GetPixelByLocalCoordinate(panel, x, y);
          currPixel->red = rscaled;
          currPixel->green = gscaled;
          currPixel->blue = bscaled;
        }
      }
    }
    else
    {
      for (int y = 0; y < NUM_LEDS_PER_PANEL_SIDE; y++)
      {  
        HsvToRgb(h + (rowHueOffset * (6 - y)), s, v, &r, &g, &b);
        rscaled = (uint8_t) (r * context.upperBrightness);
        gscaled = (uint8_t) (g * context.upperBrightness);
        bscaled = (uint8_t) (b * context.upperBrightness);
        for (int x = 0; x < NUM_LEDS_PER_PANEL_SIDE; x++)
        {
          Pixel_t * currPixel = GetPixelByLocalCoordinate(panel, x, y);
          currPixel->red = rscaled;
          currPixel->green = gscaled;
          currPixel->blue = bscaled;
        }
      }
    }

  }

  // Update our hsv values
  rowHueOffset += (rowHueOffsetAdd) ? rowHueOffsetRate : -rowHueOffsetRate;
  if (rowHueOffset > 100)
  {
    //rowHueOffsetAdd = false;
  }
  if (rowHueOffset <= 0)
  {
    rowHueOffsetAdd = true;
  }
  
  h += hrate;

  s += (sadd) ? srate : -srate;
  if (s < 0.50 && !sadd)
  {
    sadd = true;
    s = 0.50;
  }
  if (s >= 1 && sadd)
  {
    sadd = false;
    s = 0.99;
  }

  v += (vadd) ? vrate : -vrate;
  if (v < 0 && !vadd)
  {
    vadd = true;
    v = 0;
  }
  if (v >= 1 && vadd)
  {
    vadd = false;
    v = 0.99;
  }
}

static void randomTriangles2(void)
{
  static uint8_t testx = 0;
  static uint8_t testy = 0;
  static Pixel_t col1 = {0, 10, 10};
  static uint32_t count = 0;

  // DO ALL SIDES BESIDES THE TOP
  for (int side = 0; side < TOP; side++)
  {
    if (count % 2 == 0)
    {
      for (int j = 0; j < 4; j++)
      {
        if (testx >= j)
        {
          Pixel_t *p1 = GetPixelByLocalCoordinate(side, testx-j, testy+j);
          *p1 = col1;
        }
      }
    }
    else
    {
      for (int j = 0; j < 4; j++)
      {
        if (3-testx >= j)
        {
          Pixel_t *p1 = GetPixelByLocalCoordinate(side, 3-(2-testx-j), 3-(testy+j));
          *p1 = col1;
        }
      }
    }
  }

  // DO TOP DIFFERENTLY
  for (int j = 0; j < 4; j++)
  {
    for (int k = 0; k < 4; k++)
    {
      Pixel_t *p;
      switch(count % 4)
      {
        case 0:
          p = GetPixelByLocalCoordinate(TOP, testx, k);
          break;
        case 1:
          p = GetPixelByLocalCoordinate(TOP, testx, 3-k);
          break;
        case 2:
          p = GetPixelByLocalCoordinate(TOP, 3-testx, k);
          break;
        case 3:
          p = GetPixelByLocalCoordinate(TOP, 3-testx, 3-k);
          break;
      }
      *p = col1;
    }
  }

  testx++;
  if (testx > 3)
  {
    testx = 0;
    count++;
    if (count % 2 == 0)
    {
      col1 = (Pixel_t) {
        RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness), \
          RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness), \
          RAND_IN_RANGE(context.lowerBrightness, context.upperBrightness)
      };
    }
  }
}

