#include "lines_animation.h"
#include "addressable_led_manager.h"
#include "utils.h"
#include "color.h"
#include "dbg_uart.h"
#include <stdlib.h>

/*
 *  LINES ANIMATION MODULE 
 *
 */

typedef struct {
  uint8_t numPanels;
  uint8_t numLeds;
  AnimationState_e state;

  // This will be a ptr to an array of consecutive panel objects
  AddrLEDPanel_t *panels; 

  // Below is tobe used as storage for metadata about the Pixels that this animation module will keep
  LinesPixelData_t *pixelDataPtr;
  
  // Below are animation config variables
  // lower and upper limits for the random numbers
  uint8_t upperBrightness;
  uint8_t lowerBrightness;

  // Pointer to the strip. assuming there's only one
  AddrLEDStrip_t *stripBegin;
} AnimationContext_t; 

static AnimationContext_t context;

static void lines1();
static void rampUp();
static void rampDown();

void Animation_Lines_Init(AddrLEDPanel_t *panels, uint8_t numPanels, LinesPixelData_t *pixelDataPtr)
{
  memset(pixelDataPtr, 0x00, sizeof(LinesPixelData_t) * numPanels * panels->numLeds);

  context.panels = panels;
  context.pixelDataPtr = pixelDataPtr;
  context.stripBegin = panels->strip;
  context.numPanels = numPanels;
  context.numLeds = panels->numLeds * numPanels;

  context.lowerBrightness = 0;
  context.upperBrightness = 255;
}

// TODO // Change SendMessage to ProcessMessage 
void Animation_Lines_SendMessage(AnimationMessage_t *message)
{
  switch(message->signal)
  {
    case BEGIN:
      {
        context.state = RAMPING_UP;
        break;
      }
    case PAUSE:
    case STOP:
      {
        context.state = RAMPING_DOWN;

        // Keep a backup of where we leave off the LEDs
        for (int i = 0; i < context.numLeds; i++)
        {
          LinesPixelData_t *currData = (LinesPixelData_t *) &context.pixelDataPtr[i];
          Pixel_t *currPixel = (Pixel_t *) &context.stripBegin->pixels[i];
          currData->backupR = currPixel->red;
          currData->backupG = currPixel->green;
          currData->backupB = currPixel->blue;
        } 
        break;
      }
    case SET_PIXEL:
      {
        SET_PIXEL_ARGS *args = ((SET_PIXEL_ARGS *) message->payload);
        Position_e pos = args->pos;
        uint8_t x = args->x;
        uint8_t y = args->y;
        uint8_t r = args->r;
        uint8_t g = args->g;
        uint8_t b = args->b;
        logprint("SET PIXEL pos %d, x %d y %d, r%d g%d b%d\n", pos, x, y, r, g, b);
        AddrLEDManager_SetPixelRgb(args->pos, args->x, args->y, args->r, args->g, args->b);
        break;
      }
    default:
      break;
  }
}

AnimationState_e Animation_Lines_GetState(void)
{
  return context.state;
}

void Animation_Lines_Update(void)
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
        lines1();
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
  static uint8_t clockdivider = 0;

  clockdivider++;

  bool rampUpFinished = false;
  for (int i = 0; i < context.numLeds; i++)
  {
    LinesPixelData_t *currData = (LinesPixelData_t *) &context.pixelDataPtr[i];
    Pixel_t *currPixel = (Pixel_t *) &context.stripBegin->pixels[i];
    
    rampUpFinished = (currPixel->red == currData->backupR && currPixel->green == currData->backupG && currPixel->blue == currData->backupB);
   
    if (currPixel->red != currData->backupR)
      currPixel->red += (currPixel->red < currData->backupR) ? 1 : -1;
    
    if (currPixel->green != currData->backupG)
      currPixel->green += (currPixel->green < currData->backupG) ? 1 : -1;
    
    if (currPixel->blue != currData->backupB)
      currPixel->blue += (currPixel->blue < currData->backupB) ? 1 : -1;
  }

  if (rampUpFinished)
  {
    context.state = RUNNING;
  }
}

static void rampDown(void)
{
  static bool clockdivider = true;
  if (clockdivider == 0)
  {
    clockdivider = !clockdivider;
    return;
  }
  clockdivider = !clockdivider;

  bool rampDownFinished = true;
  for (int i = 0; i < context.numLeds; i++)
  {
    Pixel_t *currPixel = (Pixel_t *) &context.stripBegin->pixels[i];
    LinesPixelData_t *currData = (LinesPixelData_t *) &context.pixelDataPtr[i];
    
    rampDownFinished &= (!currPixel->red && !currPixel->green && !currPixel->blue);
   
    if (currPixel->red)
      currPixel->red--;
    if (currPixel->green)
      currPixel->green--;
    if (currPixel->blue)
      currPixel->blue--;
  }

  if (rampDownFinished)
  {
    context.state = STOPPED;
  }
}

static void lines1(void)
{
  /*static bool first = false;*/
  /*if (first) return;*/
  /*for (int panel = 0; panel < NUM_SIDES; panel++)*/
  /*{*/
    /*for (int y = 0; y < NUM_LEDS_PER_PANEL_SIDE; y++)*/
    /*{*/
      /*for (int x = 0; x < NUM_LEDS_PER_PANEL_SIDE; x++)*/
      /*{*/
        /*Pixel_t *currPixel = GetPixelByLocalCoordinate(panel, x, y);*/
        /*currPixel->red = 10;*/
        /*currPixel->green = 50;*/
        /*currPixel->blue = 0;*/
        /*break;*/
      /*}*/
      /*break;*/
    /*}*/
  /*}*/
  /*first = true;*/
}
