#include "main.h"
#include "addressable_led_manager.h"
#include "tim.h"
#include <string.h>

#define NUM_PANELS              5
#define NUM_LEDS_PER_PANEL_SIDE 4
#define NUM_LEDS_PER_PANEL      (NUM_LEDS_PER_PANEL_SIDE * NUM_LEDS_PER_PANEL_SIDE)

// 

// PRIVATE VARIBLES -------------------------------------------------

const uint16_t ledStrip1Size = 32;
AddrLEDStrip_t ledStrip1;
Pixel_t ledStrip1Pixels[sizeof(PixelPacket_t) * 16 * 2];
uint8_t ledStrip1PacketBuffer[sizeof(PixelPacket_t) * 16 * 2 + 1]; // (3 * 8) * 16 + 1

AddrLEDPanel_t panels[5];


// PRIVATE FUNCTIONS -------------------------------------------------

static void InitPanel(AddrLEDPanel_t *p)
{
  // Set local coordinates of this panel
  Pixel_t *strip = p->strip->pixels;
  for (int i = 0; i < p->numLeds; i++)
  {
    Pixel_t *pixel = &strip[p->stripRange[0] + i];
    pixel->localX = i / NUM_LEDS_PER_PANEL_SIDE;
    pixel->localY = i % NUM_LEDS_PER_PANEL_SIDE;
    
    // TODO // HANDLE GLOBAL COORDINATES
  }

  // Set global coordinates of this panel

}

static Pixel_t* GetPixelByLocalCoordinate(Position_e pos, uint8_t x, uint8_t y)
{
  return NULL;
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
    .numLeds               = ledStrip1Size,
    .pwmTimerHandle        = &LED_PANEL_1_PWM_TIMER_HANDLE,
    .pwmTimerHandleChannel = LED_PANEL_1_PWM_TIMER_CHANNEL,
    .pixels                = (Pixel_t *) &ledStrip1Pixels,
    .pixelPacketBuffer     = (uint8_t *) &ledStrip1PacketBuffer,
  };
  memset(&ledStrip1PacketBuffer, 0x0, sizeof(ledStrip1PacketBuffer));
  AddrLED_Init(&ledStrip1);

  // Initialize the Panel(s)
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
    InitPanel(&p);
    panels[pos] = p;

    // TODO // Remove below when you get the full cube
    break;
  }
}

void AddrLEDManager_SanityTest(void)
{
  bool toggle = false;
  uint8_t c = 1;
  bool addc = true;
  uint8_t top = 100;
  uint8_t stage = 0;
  while(1){

    TOGGLE_ONBOARD_LED();

    for (int i = 0; i < ledStrip1.numLeds; i++)
    {
      Pixel_t color1, color2;
#define CURRENT_TEST 0
#if CURRENT_TEST
      uint8_t r = 1;
      uint8_t g = 73;
      uint8_t b = 1;
      color1 = (Pixel_t) {.red = r, .green = g, .blue = b};
      Pixel_t *currPixel = &(ledStrip1.pixels[i]);
      *currPixel = color1;
#else
      switch(stage)
      {
        case 0:
          color1 = (Pixel_t) {.red = c, .green = c, .blue = top-c};
          color2 = (Pixel_t) {.red = top-c, .green = top-c, .blue = c};
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

      if (/*i % 2*/ i == 5 || i == 6 ||i == 9 || i == 10 || i == 21 || i == 22|| i == 25 || i ==26)
      {
        *currPixel = color1;
      }
      else
      {
        *currPixel = color2;
      }
#endif

      //if (i == 15)
      //  break;
    }
    //toggle = !toggle;

    AddrLED_SanityTest(&ledStrip1);
    //IDLE_FOREVER(100);
    HAL_Delay(10);

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
    }
    c += (addc) ? +1 : -1;
  }
}
