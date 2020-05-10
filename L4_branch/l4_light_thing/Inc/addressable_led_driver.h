
#ifndef ADDRLEDDRIVER_H
#define ADDRLEDDRIVER_H

#include "main.h"
#include "tim.h"

#define LED_PANEL_1_GPIO_PORT GPIOB
#define LED_PANEL_1_GPIO_PIN  GPIO_PIN_1

#define DEBUG_PIN_GPIO_PORT GPIOB
#define DEBUG_PIN_GPIO_PIN  GPIO_PIN_0

#define LED_PANEL_1_TIMER         TIM4
#define LED_PANEL_1_TIMER_CHANNEL TIM_CHANNEL_4
#define LED_PANEL_1_TIMER_HANDLE  htim4

#define LED_PANEL_1_PWM_TIMER         TIM3
#define LED_PANEL_1_PWM_TIMER_CHANNEL TIM_CHANNEL_4
#define LED_PANEL_1_PWM_TIMER_HANDLE  htim3

#define NAIVE false

// Below type denotes a color. 3 bytes, one for each of RGB
typedef struct {
  uint8_t green;
  uint8_t red;
  uint8_t blue;
} __attribute__((packed)) Pixel_t;

// Below denotes the raw datastructure that will be sent to the the WS2812B via DMA.
// Each uint8_t represents the duty cycle (CCR value) the PWM will be set to.
typedef struct {
  uint8_t greenRaw[8];
  uint8_t redRaw[8];
  uint8_t blueRaw[8];
} __attribute__((packed)) PixelPacket_t;

// Below structure denotes the encapsulation of one continuous LED strip (can be in any form. location of pixels handled by upper layer)
typedef struct {
  uint16_t numLeds; // Number of LEDs in the strip
  Pixel_t *pixels; // The 1 dimensional array of Pixel_t objects. an upper layer manages the locations of said pixels
  uint8_t *pixelPacketBuffer; // This is the bufer that will hold the raw data bytes to be transmitted that corresponds to the $*pixels array 
  TIM_HandleTypeDef *pwmTimerHandle;
  uint32_t pwmTimerHandleChannel;
} AddrLEDStrip_t;
  

void AddrLED_Init(AddrLEDStrip_t *l);
void AddrLED_InitNaive(void);
void AddrLED_StartPWM(void);
void AddrLED_StopPWM(void);
void AddrLED_SendReset(void);
void AddrLED_NaiveISR(void);

void AddrLED_SanityTest(AddrLEDStrip_t *l);

#endif
