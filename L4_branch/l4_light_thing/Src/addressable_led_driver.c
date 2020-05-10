
#include "addressable_led_driver.h"
#include "cmd_shell.h"
#include "main.h"
#include <string.h>
// Driver for the WS2812B based cascading, addressable LEDs

/*
 We have 5 different intervals, of which 4 must be followed accurately: T0H, T0L, T1H, T1L. Timings are reported on the datasheet as not strict (we have a margine of +-150ns) so that we can resume these intervals as:

 -Bit 0 1/3 of the period HIGH, 2/3 of the period LOW
 -Bit 1 2/3 of the period HIGH, 1/3 of the period LOW
 -Reset Stay LOW for at least 40 cycles (50us are about 40 periods of 1.25us)

Each WS2812B requires 24bits of data to reproduce a color. Each color is, in fact, composed of 3 groups of 8bits each that represent its RGB coding. This data must be sent following this order.

  GREEN[7:0] RED[7:0] BLUE[7:0]
*/

#define NAIVE_ADDR_LED_START_UPDATES() HAL_TIM_Base_Start_IT(&LED_PANEL_1_TIMER_HANDLE)
#define NAIVE_ADDR_LED_STOP_UPDATES()  HAL_TIM_Base_Stop_IT(&LED_PANEL_1_TIMER_HANDLE)

#define ADDR_LED_PWM_START()           HAL_TIM_PWM_Start(&LED_PANEL_1_PWM_TIMER_HANDLE, LED_PANEL_1_PWM_TIMER_CHANNEL);
#define ADDR_LED_PWM_STOP()            HAL_TIM_PWM_Stop(&LED_PANEL_1_PWM_TIMER_HANDLE, LED_PANEL_1_PWM_TIMER_CHANNEL);

#define ADDR_LED_PWM_SET_DUTY_CYCLE(d) __HAL_TIM_SET_COMPARE(&LED_PANEL_1_PWM_TIMER_HANDLE, LED_PANEL_1_PWM_TIMER_CHANNEL, d);

// PWM VALUES THAT MEET WS2812Bs SPECS
#define ADDR_LED_PWM_PSC 2
#define ADDR_LED_PWM_ARR 32

// PWM DUTY CYCLE VALUES FOR THE LOGIC HIGH AND LOW CODES
#define ADDR_LED_CODE_HIGH_COMPARE_VAL 23 
#define ADDR_LED_CODE_LOW_COMPARE_VAL  8

// PRIVATE VARIBLES -------------------------------------------------

uint32_t MICROSECOND_PRESCALER , MILLISECOND_PRESCALER;

// PRIVATE FUNCTIONS ------------------------------------------------

static void AddrLED_SetPWMPeriodUs(uint16_t ns)
{
  // Set/reset Symbol sending update timer to fire an interrupt
  // Stop and restart timer if it was already running
  bool tmrWasRunning = false;
  if (LED_PANEL_1_PWM_TIMER->CR1 ^ TIM_CR1_CEN)
  {
    tmrWasRunning = true;
    ADDR_LED_PWM_STOP();
  }

  // Update autoreload register
  // Timer will generate an IRQ every $ns nanoseconds
  LED_PANEL_1_PWM_TIMER->ARR = ns;

  // Generate update event to load new ARR immediately
  LED_PANEL_1_PWM_TIMER->EGR = TIM_EGR_UG;

  // Reset counter if timer was running before
  if (tmrWasRunning)
  {
    LED_PANEL_1_PWM_TIMER->CNT = 0x00;
    ADDR_LED_PWM_START();
  }
}

// WS2812B Related 

// Convert $byte into codes that WS2812B accepts, that we push out via PWM.
// 1 Byte converts into 8 bytes, so make sure $*codes pointer points to a block of memory that has 8 bytes allocated
static inline void ByteToCodes(uint8_t byte, uint8_t *codes)
{
  for (int b = 0; b < 8; b++)
  {
    *(codes + b) = ((byte & (0x1 << b)) > 0) ? ADDR_LED_CODE_HIGH_COMPARE_VAL : ADDR_LED_CODE_LOW_COMPARE_VAL;
  }
}

// Convert a Pixel_t object $p into codes that WS2812B accepts. 
// 1 Byte converts into 8 bytes, a Pixel_t is 3 bytes so make sure $*packet points to a block of memory that has 24 bytes allocated
static inline void PixelToPacket(Pixel_t *p, PixelPacket_t *packet)
{
  ByteToCodes(p->green, &(packet->greenRaw[0]));
  ByteToCodes(p->red, &(packet->redRaw[0]));
  ByteToCodes(p->blue, &(packet->blueRaw[0]));
}

// PUBLIC FUNCTIONS -------------------------------------------------

void AddrLED_Init(void)
{
  MICROSECOND_PRESCALER = ((HAL_RCC_GetSysClockFreq() / 1000000) - 1);    // 1000000 Hz
  MILLISECOND_PRESCALER = ((HAL_RCC_GetSysClockFreq() / 1000) - 1);       // 1000 Hz

  /* ~ Initialize PWM Timer ~
   *
   * OKAY SO:
   * PSC 0, ARR 25, CCR 8  Gives me an update time of ~1440 nanoseconds, HIGH time of ~445 nanoseconds
   *                CCR 7                                                HIGH time of ~390 nanoseconds
   *                CCR 15                                               HIGH time of ~823 nanoseconds
   * 
   * So seems like the Compare values i'll need are 7 and 15, for 0 CODE and 1 CODE respectively.
   *
   * Lets go with this, why not
   *
   */

  // Set update event flag so PSC and ARR are loaded
  LED_PANEL_1_PWM_TIMER->PSC = ADDR_LED_PWM_PSC;
  LED_PANEL_1_PWM_TIMER->ARR = ADDR_LED_PWM_ARR;
  LED_PANEL_1_PWM_TIMER->EGR = TIM_EGR_UG;
  
  //#define PWM_BASE_TEST
  #define PWM_DMA_TEST

  #ifdef PWM_BASE_TEST
  ADDR_LED_PWM_START();
  ADDR_LED_PWM_SET_DUTY_CYCLE(10);
  while(1){}
  #endif

  #ifdef PWM_DMA_TEST
  //
  uint8_t o = ADDR_LED_CODE_HIGH_COMPARE_VAL;
  uint8_t z = ADDR_LED_CODE_LOW_COMPARE_VAL;
  
  #if 1
  const uint8_t dmaTestPayload[] = {
                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, o, z,  

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, z, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, o, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, z, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, o, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, z, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, o, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, z, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, o, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, z, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, o, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, z, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, o, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, z, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, o, z, 

                                    z, z, z, z, z, z, z, o,
                                    z, z, z, z, z, z, z, z,
                                    z, z, z, z, z, z, z, z, 

                                    0};

  // 16 LEDS, 3 colors, 8 bytes per color, null character at the end
  // 16 * 3 * 8 + 1 = 385
  uint8_t payloadDupeTest[16 * 3 * 8 + 1];
  PixelPacket_t *payloadDupePP = (PixelPacket_t *) &payloadDupeTest;

  memset(&payloadDupeTest, 0x0, 16 * 3 * 8 + 1);
  Pixel_t color1 = {.red = 0x0, .green = 0x1, .blue = 0x2};
  Pixel_t color2 = {.red = 0x0, .green = 0x1, .blue = 0x0};
 
  for (int i = 0; i < (16 * 3 * 8) / sizeof(PixelPacket_t); i++)
  {
    PixelPacket_t *currPixel = payloadDupePP + i;
    if (i % 2)
    {
      PixelToPacket(&color1, currPixel);
    }
    else
    {
      PixelToPacket(&color2, currPixel);
    }
  }

  while(1){}

  #else
  const uint8_t dmaTestPayload[] = {o, z, 1, 1, 1, 1, 0};
  #endif
  //const uint8_t dmaTestPayload[] = {10, 1, 1, 1, 0};
  //HAL_TIM_PWM_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)
    HAL_TIM_PWM_Start_DMA(&LED_PANEL_1_PWM_TIMER_HANDLE, LED_PANEL_1_PWM_TIMER_CHANNEL, (uint32_t *) &dmaTestPayload, sizeof(dmaTestPayload));
  HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
  while(1){
  }
  #endif

  AddrLED_SanityTest();
  while(1){}
}

void AddrLED_SanityTest(void)
{
  // Initialize test Pixel array
  const uint8_t testSize = 4 * 4;
  Pixel_t test[testSize];
  memset(&test, 0x0, sizeof(test));
  for (int i = 0; i < testSize; i++)
  {
    if (i < 4)
      test[i] = (Pixel_t) {0, 0, 0xff};
    else if (i <= 8)
      test[i] = (Pixel_t) {0, 0xff, 0};
    else if (i <= 12)
      test[i] = (Pixel_t) {0xff, 0, 0};
    test[i] = (Pixel_t) {0, 0xff, 0};
  }
  
  // Initialize payload
  uint8_t test1Payload[3 * 8 * testSize + 1];
  uint8_t test1PayloadHead = 0;
  memset(&test1Payload, 0x0, sizeof(test1Payload));
  
  #define BREAKEARLY false
  // Go thru all Pixel_t objects
  for (int i = 0; i < testSize; i++)
  {
    // Go thru all bytes 
    for (int j = 0; j < sizeof(Pixel_t); j++)
    {
      uint8_t currSourceByte = *((uint8_t *) &test[i] + j);
      // Go thru all bits
      for (int b = 0; b < 8; b++)
      {
       *((uint8_t *) &test1Payload + test1PayloadHead) = ((currSourceByte) & 0x1 << b) > 0 ? ADDR_LED_CODE_HIGH_COMPARE_VAL : ADDR_LED_CODE_LOW_COMPARE_VAL;
       test1PayloadHead++;
      }
    }
    #if BREAKEARLY
    if (i == 3)
      break;
    #endif
  }
  
  HAL_TIM_PWM_Start_DMA(&LED_PANEL_1_PWM_TIMER_HANDLE, LED_PANEL_1_PWM_TIMER_CHANNEL, (uint32_t *) &test1Payload, sizeof(test1Payload));
  bool block = true;
  while (block) {}
}

void AddrLED_StartPWM(void)
{
  ADDR_LED_PWM_START();
}

void AddrLED_StopPWM(void)
{
  ADDR_LED_PWM_STOP();
}

void AddrLED_InitNaive(void)
{

}

void AddrLED_SendColor(uint8_t red, uint8_t green, uint8_t blue)
{

}

void AddrLED_SendReset(void)
{

}

// HMM this doesnt work. need to dma pwm this data
void AddrLED_NaiveISR(void)
{
  HAL_GPIO_TogglePin(DEBUG_PIN_GPIO_PORT, DEBUG_PIN_GPIO_PIN);
}


