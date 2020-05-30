#include "main.h"
#include "addressable_led_manager.h"
#include "tim.h"
#include "utils.h"
#include "random_fade_animation.h"
#include <string.h>
#include <stdlib.h>

#define NUM_PANELS              5
#define NUM_LEDS_PER_PANEL_SIDE 4
#define NUM_LEDS_PER_PANEL      (NUM_LEDS_PER_PANEL_SIDE * NUM_LEDS_PER_PANEL_SIDE)
#define NUM_LEDS_TOTAL          (NUM_PANELS * NUM_LEDS_PER_PANEL)

#define LEDS_BEGIN_AT_BOTTOM    1

// 

// PRIVATE VARIBLES -------------------------------------------------

const uint16_t ledStrip1Size = NUM_LEDS_TOTAL;
AddrLEDStrip_t ledStrip1;
Pixel_t ledStrip1Pixels[sizeof(PixelPacket_t) * NUM_LEDS_PER_PANEL * NUM_PANELS];
uint8_t ledStrip1PacketBuffer[sizeof(PixelPacket_t) * NUM_LEDS_PER_PANEL * NUM_PANELS + 1]; // (3 * 8) * 16 + 1

AddrLEDPanel_t panels[5];

// PixelData_t arrays are for animation modules to hold misc data about every pixel.
// They are to be instantiated here and a pointer of them to be passed to aniation
// modules during their inits.
RandomFadePixelData_t randomFadePixelData[NUM_LEDS_TOTAL];


// PRIVATE FUNCTIONS -------------------------------------------------

static void InitPanel(AddrLEDPanel_t *p)
{
  // Set local coordinates of this panel
  Pixel_t *strip = p->strip->pixels;
  for (int i = 0; i < p->numLeds; i++)
  {
    Pixel_t *pixel = &strip[p->stripRange[0] + i];
    pixel->localX = NUM_LEDS_PER_PANEL_SIDE - (i / NUM_LEDS_PER_PANEL_SIDE);
    pixel->localY = NUM_LEDS_PER_PANEL_SIDE - (i % NUM_LEDS_PER_PANEL_SIDE);
    
    // TODO // HANDLE GLOBAL COORDINATES
  }

  // Set global coordinates of this panel

}

static Pixel_t* GetPixelByLocalCoordinate(Position_e pos, uint8_t x, uint8_t y)
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

static Pixel_t* GetPixelByGlobalCoordinate(uint8_t x, uint8_t y, uint8_t z)
{
  return NULL;
} 

inline static AddrLEDPanel_t* GetPanelByLocation(Position_e pos)
{
  return NULL;
}

// PUBLIC FUNCTIONS -------------------------------------------------

void AddrLEDManager_Init(void)
{
  // Initialize the strip(s). This initialize one continuous strip. 
  // If multiple panels are daisychained, that counts as one strip.
  ledStrip1 = (AddrLEDStrip_t) {
    .numLeds                 = ledStrip1Size,
      .pwmTimerHandle        = &LED_PANEL_1_PWM_TIMER_HANDLE,
      .pwmTimerHandleChannel = LED_PANEL_1_PWM_TIMER_CHANNEL,
      .pixels                = (Pixel_t *) &ledStrip1Pixels,
      .pixelPacketBuffer     = (uint8_t *) &ledStrip1PacketBuffer,
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

  // Initialize our animation
  Animation_RandomFade_Init((AddrLEDPanel_t *) &panels, NUM_PANELS, (RandomFadePixelData_t *) &randomFadePixelData);
}

void AddrLEDManager_SanityTest(void)
{
  bool toggle = false;
  uint8_t c = 1;
  bool addc = true;
  uint8_t top = 10;
  uint8_t stage = 0;
  while(1){

    TOGGLE_ONBOARD_LED();

#define TEST 3
#if (TEST == 0)   // CURRENT DRAW TEST
    for (int i = 0; i < ledStrip1.numLeds; i++)
    {
      continue;
      Pixel_t color1, color2;
      uint8_t r = 0;
      uint8_t g = 10;
      uint8_t b = 10;
      color1 = (Pixel_t) {.red = r, .green = g, .blue = b};
      Pixel_t *currPixel = &(ledStrip1.pixels[i]);
      *currPixel = color1;
    }
    
    static uint8_t testx = 0;
    static uint8_t testy = 0;
    uint8_t upper = 10;
    uint8_t lower = 1;
    static Pixel_t col1 = {0, 10, 10};
    static Pixel_t col2 = {10, 0, 10};
    static Pixel_t col3 = {10, 10, 0};
    static uint32_t count = 0;

    for (int side = 0; side < NUM_SIDES; side++)
    {
      for (int j = 0; j < 4; j++)
      {
        if (testx >= j)
        {
          Pixel_t *p1 = GetPixelByLocalCoordinate(side, testx-j, testy+j);
          *p1 = (side % 2) ? col1 : col2;
        }
      }
    }

    for (int side = 0; side < NUM_SIDES; side++)
    {
      for (int j = 0; j < 4; j++)
      {
        if (testx >= j)
        {
          Pixel_t *p1 = GetPixelByLocalCoordinate(side, 3-(testx-j), 3-(testy+j));
          *p1 = (side % 2) ? col2 : col1;

          if (testx == 3)
            *p1 = col3;
        }
      }
    }


    testx++;
    if (testx > 3)
    {
      testx = 0;
      count++;
      if (count % 2 == 0)
      {
        col1 = (Pixel_t) {RAND_IN_RANGE(lower, upper), RAND_IN_RANGE(lower, upper), RAND_IN_RANGE(lower, upper)};
        col2 = (Pixel_t) {RAND_IN_RANGE(lower, upper), RAND_IN_RANGE(lower, upper), RAND_IN_RANGE(lower, upper)};
        col3 = (Pixel_t) {RAND_IN_RANGE(lower, upper), RAND_IN_RANGE(lower, upper), RAND_IN_RANGE(lower, upper)};
      }
      else
      {
        col1 = (Pixel_t) {0,0,0};
        col2 = (Pixel_t) {0,0,0};
        col3 = (Pixel_t) {0,0,0};
      }
    }

#elif (TEST == 1) // REGULAR TEST
    for (int i = 0; i < ledStrip1.numLeds; i++)
    {
      Pixel_t color1, color2;
      switch(stage)
      {
        case 0:
          color1 = (Pixel_t) {.red = c, .green = 0, .blue = top-c};
          color2 = (Pixel_t) {.red = 0, .green = top-c, .blue = c};
          break;

        case 1:
          color1 = (Pixel_t) {.red = c, .green = c, .blue = 0x0};
          color2 = (Pixel_t) {.red = c, .green = top-c, .blue = 0x0};
          break;

        case 2:
          color1 = (Pixel_t) {.red = c, .green = 0x0, .blue = c};
          color2 = (Pixel_t) {.red = c, .green = 0x0, .blue = top-c};
          break;
      }

      Pixel_t *currPixel = &(ledStrip1.pixels[i]);

      if (i % 2)
      {
        *currPixel = (toggle) ? color1 : color2;
      }
      else
      {
        *currPixel = (toggle) ? color2 : color1;
      }
    }
#elif (TEST == 3)
    Animation_RandomFade_Update();
#endif
    //if (i == 15)
    //  break;
    //toggle = !toggle;

    AddrLED_SanityTest(&ledStrip1);

    // TODO THERES A MEMORY ERROR SOMEWHERE THAT AFTER THE ABOVE FUNCTION, panels[0].strip GETS OVERWRITTEN
    //panels[0].strip = &ledStrip1; 

    //IDLE_FOREVER(100);
    HAL_Delay(100);

    // We need to stop the pwm timer after our payload is sent and start it back up again
    AddrLED_StopPWM();

    if (c >= top)
    {
      addc = false;
    }
    if (c < 1)
    {
      addc = true;
      //stage++;
      if (stage > 2)
        stage = 0;
      //toggle = !toggle;
    }
    c += (addc) ? +1 : -1;
  }
}
