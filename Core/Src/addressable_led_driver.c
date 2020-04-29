
#include "addressable_led_driver.h"
#include "cmd_shell.h"

// Driver for the WS2812B based cascading, addressable LEDs

/*
 We have 5 different intervals, of which 4 must be followed accurately: T0H, T0L, T1H, T1L. Timings are reported on the datasheet as not strict (we have a margine of +-150ns) so that we can resume these intervals as:

 -Bit 0 1/3 of the period HIGH, 2/3 of the period LOW
 -Bit 1 2/3 of the period HIGH, 1/3 of the period LOW
 -Reset Stay LOW for at least 40 cycles (50us are about 40 periods of 1.25us)

Each WS2812B requires 24bits of data to reproduce a color. Each color is, in fact, composed of 3 groups of 8bits each that represent its RGB coding. This data must be sent following this order.

  GREEN[7:0] RED[7:0] BLUE[7:0]
*/

/*
  NAIVE  MODE: Send symbols via regular gpio.
  NORMAL MODE: Send symbols via DMAd PWM
*/

#define NAIVE_ADDR_LED_START_UPDATES() HAL_TIM_Base_Start_IT(&LED_PANEL_1_TIMER_HANDLE)
#define NAIVE_ADDR_LED_STOP_UPDATES()  HAL_TIM_Base_Stop_IT(&LED_PANEL_1_TIMER_HANDLE)

uint32_t POINT_ONE_TWO_MICROSECOND_PRESCALER , MICROSECOND_PRESCALER , MILLISECOND_PRESCALER;

const uint16_t AddrLEDSymbolTimes[] = // x * 0.125 time units
{
  [ADDR_LED_SYMBOL_T0H]   = 400/125,
  [ADDR_LED_SYMBOL_T0L]   = 850/125,
  [ADDR_LED_SYMBOL_T1H]   = 800/125,
  [ADDR_LED_SYMBOL_T1L]   = 450/125,
  [ADDR_LED_SYMBOL_RESET] = 50000*0.125 // <- Will fire once
};


volatile bool naiveSendingInProgress = false;
volatile AddrLEDCode_e   naiveCurrentCode   = ADDR_LED_CODE_NONE;
volatile AddrLEDSymbol_e naiveCurrentSymbol = ADDR_LED_SYMBOL_NONE;

static void AddrLED_NaiveSetUpdatePeriodUs(uint16_t ns)
{
  // Set/reset Symbol sending update timer to fire an interrupt
  // Stop and restart timer if it was already running
  bool tmrWasRunning = false;
  if (LED_PANEL_1_TIMER->CR1 ^ TIM_CR1_CEN)
  {
    tmrWasRunning = true;
    NAIVE_ADDR_LED_STOP_UPDATES();
  }

  // Update autoreload register
  // Timer will generate an IRQ every $ns nanoseconds
  LED_PANEL_1_TIMER->ARR = ns;

  // Generate update event to load new ARR immediately
  LED_PANEL_1_TIMER->EGR = TIM_EGR_UG;

  // Reset counter if timer was running before
  if (tmrWasRunning)
  {
    LED_PANEL_1_TIMER->CNT = 0x00;
    NAIVE_ADDR_LED_START_UPDATES();
  }
}

void AddrLED_Init(void)
{
  POINT_ONE_TWO_MICROSECOND_PRESCALER = ((HAL_RCC_GetSysClockFreq() / 8000000) - 1); // 8000000 Hz, update irq every 0.125 us
  MICROSECOND_PRESCALER = ((HAL_RCC_GetSysClockFreq() / 1000000) - 1);    // 1000000 Hz
  MILLISECOND_PRESCALER = ((HAL_RCC_GetSysClockFreq() / 1000) - 1);       // 1000 Hz

#if NAIVE
  LED_PANEL_1_TIMER->PSC = POINT_ONE_TWO_MICROSECOND_PRESCALER;
  AddrLED_InitNaive();
#else
  // TODO
#endif
}

void AddrLED_InitNaive(void)
{
  HAL_GPIO_DeInit(LED_PANEL_1_GPIO_PORT, LED_PANEL_1_GPIO_PIN);
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = LED_PANEL_1_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_PANEL_1_GPIO_PORT, &GPIO_InitStruct);
}

void AddrLED_SendColor(uint8_t red, uint8_t green, uint8_t blue)
{
#if NAIVE
  uint8_t *colors[] = {&red, &green, &blue};
  for (int i = 0; i < 3; i++)
  {
    for (int b = 0; b < 8; b++)
    {
      AddrLEDCode_e code;
      code = ((*colors[i] & (0x1 << b)) > 0) ? ADDR_LED_CODE_HIGH : ADDR_LED_CODE_LOW;
      if (code == ADDR_LED_CODE_HIGH)
      AddrLED_SendCodeNaive(code, true);
    }
  }
#else

#endif
}

void AddrLED_SendReset(void)
{
#if NAIVE
  AddrLED_SendCodeNaive(ADDR_LED_CODE_RESET, true);
#else

#endif
}

void AddrLED_SendCodeNaive(AddrLEDCode_e code, bool blocking)
{
  // Begin sending code. 
  naiveSendingInProgress = true;
  naiveCurrentCode = code;
  uint16_t holdTime;

  // All this function has to pretty much is to find how long to hold the first
  // symbol of the code. 
  // Unless the code is RESET, it is pulling the serial line high. 
  switch(code)
  {
    case ADDR_LED_CODE_HIGH:
    {
      naiveCurrentSymbol = ADDR_LED_SYMBOL_T1H;
      break;
    }
    case ADDR_LED_CODE_LOW:
    {
      naiveCurrentSymbol = ADDR_LED_SYMBOL_T0H;
      break;
    }
    case ADDR_LED_CODE_RESET:
    {
      naiveCurrentSymbol = ADDR_LED_SYMBOL_RESET;
      break;
    }
    default:
    {
      // This shouldnt happen really
      return;
    }
  }
  holdTime = AddrLEDSymbolTimes[naiveCurrentSymbol];

  // If code we're sending is a 1 or a 0, the first half of the code is
  // the serial line being pulled high. So, do that.
  if (code < ADDR_LED_CODE_RESET)
  {
    HAL_GPIO_WritePin(LED_PANEL_1_GPIO_PORT, LED_PANEL_1_GPIO_PIN, SET);
  }
  else
  {
    HAL_GPIO_WritePin(LED_PANEL_1_GPIO_PORT, LED_PANEL_1_GPIO_PIN, RESET);
  }

  // Set and start the IRQ timer. If $blocking, wait until all bits are sent
  AddrLED_NaiveSetUpdatePeriodUs(holdTime);
  NAIVE_ADDR_LED_START_UPDATES();
  if (blocking)
  {
    while(naiveSendingInProgress) {}
  }
}

void AddrLED_NaiveISR(void)
{
  // If needed, begin sending the second half of the code
  naiveSendingInProgress = true;
  uint16_t holdTime;

  // If we were just playing the RESET code, this interrupt is the 
  // end of this code. 
  // If we were playing a T1H/T0H code, this interrupt needs to start playing 
  // the second part of this code
  // If we were playing a T1L/T0L code, this interrupt is the end of this
  // code.

  switch(naiveCurrentSymbol)
  {
    case ADDR_LED_SYMBOL_T1H:
    {
      naiveCurrentSymbol = ADDR_LED_SYMBOL_T1L;
      break;
    }
    case ADDR_LED_SYMBOL_T0H:
    {
      naiveCurrentSymbol = ADDR_LED_SYMBOL_T0L;
      break;
    }
    case ADDR_LED_SYMBOL_RESET:
    case ADDR_LED_SYMBOL_T0L:
    case ADDR_LED_SYMBOL_T1L:
    {
      // End of a sequence. stop the irq timer and return
      naiveCurrentSymbol = ADDR_LED_SYMBOL_NONE;
      naiveCurrentCode   = ADDR_LED_CODE_NONE;
      NAIVE_ADDR_LED_STOP_UPDATES();
      naiveSendingInProgress = false;
      return;
    }
    default:
    {
      // This shouldnt happen
      break;
    }
  }

  holdTime = AddrLEDSymbolTimes[naiveCurrentSymbol];
  if (naiveCurrentCode != ADDR_LED_CODE_RESET)
  {   
    HAL_GPIO_WritePin(LED_PANEL_1_GPIO_PORT, LED_PANEL_1_GPIO_PIN, RESET);
  }
  AddrLED_NaiveSetUpdatePeriodUs(holdTime);

}




