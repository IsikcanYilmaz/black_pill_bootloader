#include "main.h"
#include "addressable_led_manager.h"
#include "tim.h"
#include "utils.h"
#include "random_fade_animation.h"
#include "random_triangles_animation.h"
#include "sw_timers.h"
#include "dbg_uart.h"
#include <string.h>
#include <stdlib.h>

#define LEDS_BEGIN_AT_BOTTOM    1

/*
 * ADDRESSABLE LED MANAGER
 *
 * This module is to handle abstract neopixel usage. It will handle the initialization of the AddrLEDPanel_t and 
 * the AddrLEDStrip_t abstractions, initialize Pixel_ts with their coordinate data, etc. The neopixel driver code is 
 * not aware of the overall shape and structure of the strips. The manger module has awareness of this, and one of  its 
 * purposes is to provide functions to make addressing the pixels easier.
 *
 * The manager module also initializes and plays "Animation" modules. 
 *
 */

// PRIVATE VARIBLES -------------------------------------------------

const uint16_t ledStrip1Size = NUM_LEDS_TOTAL;
AddrLEDStrip_t ledStrip1;
Pixel_t ledStrip1Pixels[sizeof(PixelPacket_t) * NUM_LEDS_PER_PANEL * NUM_PANELS];
uint8_t ledStrip1PacketBuffer[sizeof(PixelPacket_t) * NUM_LEDS_PER_PANEL * NUM_PANELS + 1]; // (3 * 8) * 16 + 1
SwTimer_t refreshTimer;

volatile bool animationSkipInProgress = false;
volatile uint8_t animationIndex = 0; // TODO // this is a temporary gimmick. 

AddrLEDPanel_t panels[5];

// PixelData_t arrays are for animation modules to hold misc data about every pixel.
// They are to be instantiated here and a pointer of them to be passed to aniation
// modules during their inits.
RandomFadePixelData_t randomFadePixelData[NUM_LEDS_TOTAL];
RandomTrianglesPixelData_t randomTrianglesPixelData[NUM_LEDS_TOTAL];

AnimationInterface_t animations[NUM_ANIMATIONS] = 
{
  [ANIMATION_RANDOMFADE] = {
                            .init = Animation_RandomFade_Init, 
                            .update = Animation_RandomFade_Update,  
                            .getState = Animation_RandomFade_GetState,
                            .sendMessage = Animation_RandomFade_SendMessage,
                            .animationStr = "Random Fade"
                           },
  [ANIMATION_RANDOMTRIANGLES] = {
                            .init = Animation_RandomTriangles_Init, 
                            .update = Animation_RandomTriangles_Update,  
                            .getState = Animation_RandomTriangles_GetState,
                            .sendMessage = Animation_RandomTriangles_SendMessage,
                            .animationStr = "Random Triangles"
                           },

};

// PRIVATE FUNCTIONS -------------------------------------------------

static void InitPanel(AddrLEDPanel_t *p)
{
  // Set local coordinates of this panel
  Pixel_t *pixels = p->strip->pixels;
  for (int i = 0; i < p->numLeds; i++)
  {
    Pixel_t *pixel = &pixels[p->stripRange[0] + i];
    pixel->localX = NUM_LEDS_PER_PANEL_SIDE - (i / NUM_LEDS_PER_PANEL_SIDE);
    pixel->localY = NUM_LEDS_PER_PANEL_SIDE - (i % NUM_LEDS_PER_PANEL_SIDE);
    
    // TODO // HANDLE GLOBAL COORDINATES
  }

  // Set global coordinates of this panel

}

static uint32_t AddrLEDManager_RefreshCallback(void)
{
  TOGGLE_ONBOARD_LED();
  switch (animationIndex % 2)
  {
    case 0:
      {
        Animation_RandomTriangles_Update();
        break;
      }
    case 1:
      {
        Animation_RandomFade_Update();
        break;
      }
  }
  HAL_Delay(1);
  //AddrLED_DisplayStrip(&ledStrip1);
  return 100;
}

// PUBLIC FUNCTIONS -------------------------------------------------

void AddrLEDManager_Init(void)
{
  // Initialize the strip(s). This initializes one continuous strip. 
  // If multiple panels are daisychained, that counts as one strip.
  ledStrip1 = (AddrLEDStrip_t) {
    .numLeds                 = ledStrip1Size,
    .pwmTimerHandle          = &LED_PANEL_1_PWM_TIMER_HANDLE,
    .pwmTimerHandleChannel   = LED_PANEL_1_PWM_TIMER_CHANNEL,
    .pixels                  = (Pixel_t *) &ledStrip1Pixels,
    .pixelPacketBuffer       = (uint8_t *) &ledStrip1PacketBuffer,
  };
  memset(&ledStrip1PacketBuffer, 0x0, sizeof(ledStrip1PacketBuffer));
  AddrLED_Init(&ledStrip1);

  // Initialize the panel structures
  for (int panelIdx = 0; panelIdx < NUM_PANELS; panelIdx++)
  {
    Position_e pos = (Position_e) panelIdx;
    AddrLEDPanel_t p = {
      .strip = &ledStrip1,
      .numLeds = NUM_LEDS_PER_PANEL,
      .stripRange = {(panelIdx * NUM_LEDS_PER_PANEL), ((panelIdx + 1)* NUM_LEDS_PER_PANEL - 1)},
      .position = pos,
      .neighborPanels = {NULL, NULL, NULL, NULL},
    };
    p.stripFirstPixel = &(p.strip->pixels[p.stripRange[0]]);
    InitPanel(&p);
    panels[pos] = p;
  }

  // Initialize our animations
  Animation_RandomFade_Init((AddrLEDPanel_t *) &panels, NUM_PANELS, (RandomFadePixelData_t *) &randomFadePixelData);
  Animation_RandomTriangles_Init((AddrLEDPanel_t *) &panels, NUM_PANELS, (RandomTrianglesPixelData_t *) &randomTrianglesPixelData);

  // Initialize refresh timer
  refreshTimer.fn = AddrLEDManager_RefreshCallback;
  refreshTimer.Ms = 100;

  logprint("Animations initialized\n");
  logprint("Current Animation %s\n", animations[animationIndex].animationStr);
}

Pixel_t* GetPixelByLocalCoordinate(Position_e pos, uint8_t x, uint8_t y)
{
  AddrLEDPanel_t *panel = &panels[pos];
  AddrLEDStrip_t *strip = panel->strip;
  uint8_t ledIdx;
#if LEDS_BEGIN_AT_BOTTOM
  y = NUM_LEDS_PER_PANEL_SIDE - y - 1;
#endif
  if (y % 2 == 0)
  {
    ledIdx = x + (NUM_LEDS_PER_PANEL_SIDE * y);
  }
  else
  {
    ledIdx = (NUM_LEDS_PER_PANEL_SIDE - 1 - x) + (NUM_LEDS_PER_PANEL_SIDE * y);
  }
  return &(panel->stripFirstPixel[ledIdx]);
}

Pixel_t* GetPixelByGlobalCoordinate(uint8_t x, uint8_t y, uint8_t z)
{
  return NULL;
} 

inline AddrLEDPanel_t* GetPanelByLocation(Position_e pos)
{
  return NULL;
}

void AddrLEDManager_RefreshTimerStart(void)
{
  SwTimer_Start(&refreshTimer);
}

void AddrLEDManager_PlayNextAnimation(void)
{
  // Tell current animation to start ramping down
  AnimationMessage_t pauseMsg = {PAUSE, NULL};
  animations[animationIndex % NUM_ANIMATIONS].sendMessage(&pauseMsg);

  // Set our state machine to "transitioning from animation to animation"
  animationSkipInProgress = true;

  logprint("Skipping animation. Current animation %s\n", animations[animationIndex].animationStr);
}

void AddrLEDManager_SanityTest(void)
{}

void AddrLEDManager_Workloop(void)
{
  while(1){
    /*TOGGLE_ONBOARD_LED();*/
    
    // If we're in process of skipping, wait for current animation to ramp down
    if (animationSkipInProgress)
    {
      AnimationState_e animState = animations[animationIndex % NUM_ANIMATIONS].getState();
      if (animState == STOPPED)
      {
        animationIndex++;
        AnimationMessage_t start = {BEGIN, NULL};
        animations[animationIndex % NUM_ANIMATIONS].sendMessage(&start);
        animationSkipInProgress = false;
      }
    }
    animations[animationIndex % NUM_ANIMATIONS].update();
    AddrLED_DisplayStrip(&ledStrip1);
    HAL_Delay(10);
  }
}
