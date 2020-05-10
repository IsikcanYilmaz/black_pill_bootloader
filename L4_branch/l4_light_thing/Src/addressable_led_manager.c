#include "main.h"
#include "addressable_led_manager.h"
#include "tim.h"
#include <string.h>

const uint16_t ledStrip1Size = 16;
AddrLEDStrip_t ledStrip1;
Pixel_t ledStrip1Pixels[sizeof(PixelPacket_t) * 16];
uint8_t ledStrip1PacketBuffer[sizeof(PixelPacket_t) * 16 + 1]; // (3 * 8) * 16 + 1

void AddrLEDManager_Init(void)
{
  ledStrip1 = (AddrLEDStrip_t) {
    .numLeds               = ledStrip1Size,
    .pwmTimerHandle        = &LED_PANEL_1_PWM_TIMER_HANDLE,
    .pwmTimerHandleChannel = LED_PANEL_1_PWM_TIMER_CHANNEL,
    .pixels                = (Pixel_t *) &ledStrip1Pixels,
    .pixelPacketBuffer     = (uint8_t *) &ledStrip1PacketBuffer,
  };
  memset(&ledStrip1PacketBuffer, 0x0, sizeof(ledStrip1PacketBuffer));
  AddrLED_Init(&ledStrip1);
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

    for (int i = 0; i < ledStrip1.numLeds; i++)
    {
      Pixel_t color1, color2;
      switch(stage)
      {
#if 0
        case 0:
        color1 = (Pixel_t) {.red = c, .green = 0x0, .blue = c};
        color2 = (Pixel_t) {.red = 0x0, .green = c, .blue = top-c};
        break;

        case 1:
        color1 = (Pixel_t) {.red = top-c, .green = c, .blue = 0x0};
        color2 = (Pixel_t) {.red = 0x0, .green = c, .blue = 0x0};
        break;

        case 2:
        color1 = (Pixel_t) {.red = 0x0, .green = c, .blue = top-c};
        color2 = (Pixel_t) {.red = c, .green = c, .blue = 0x0};
        break;
#endif
        case 0:
        color1 = (Pixel_t) {.red = c, .green = 0x2, .blue = 0x0};
        color2 = (Pixel_t) {.red = 0x0, .green = c, .blue = c};
        break;

        case 1:
        color1 = (Pixel_t) {.red = 0x0, .green = c, .blue = c};
        color2 = (Pixel_t) {.red = c, .green = 0x2, .blue = 0x0};
        break;

        case 2:
        color1 = (Pixel_t) {.red = c, .green = 0x0, .blue = 0x0};
        color2 = (Pixel_t) {.red = c, .green = 0x0, .blue = c};
        break;
      }

      Pixel_t *currPixel = &(ledStrip1.pixels[i]);
      if (i == 5 || i == 6 || i == 9 || i == 10)
        *currPixel = (toggle) ? color2 : color1; // CENTER
      else
        *currPixel = (toggle) ? color1 : color2; // OUTER
    }
    //toggle = !toggle;

    AddrLED_SanityTest(&ledStrip1);

    HAL_Delay(50);

    if (c >= top)
    {
      addc = false;
    }
    if (c < 1)
    {
      addc = true;
      stage++;
      if (stage > 1)
        stage = 0;
    }

    c += (addc) ? +1 : -1;

    //__HAL_DMA_CLEAR_FLAG(&hdma_tim3_ch4_up, 
  }

}
