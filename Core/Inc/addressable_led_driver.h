
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

// There are 5 kinds of symbols.
// To send a '0' code (logic low) one needs to send T0H, then T0L
// To send a '1' code (logic hi ) one needs to send T1H, then T1L
// The reset symbol finishes a transmission. 
// Use the lookup table defined in the c file to get their respective times
typedef enum {
  ADDR_LED_SYMBOL_T0H,
  ADDR_LED_SYMBOL_T1H,
  ADDR_LED_SYMBOL_T0L,
  ADDR_LED_SYMBOL_T1L,
  ADDR_LED_SYMBOL_RESET,
  ADDR_LED_SYMBOL_NONE,
} AddrLEDSymbol_e;

// Below enum just denotes logic high, logic low, and the reset codes. 
typedef enum {
  ADDR_LED_CODE_HIGH,
  ADDR_LED_CODE_LOW,
  ADDR_LED_CODE_RESET,
  ADDR_LED_CODE_NONE
} AddrLEDCode_e;

void AddrLED_Init(void);
void AddrLED_InitNaive(void);
void AddrLED_SendColor(uint8_t red, uint8_t green, uint8_t blue);
void AddrLED_SendReset(void);
void AddrLED_SendLowNaive(void);
void AddrLED_SendHighNaive(void);
void AddrLED_SendCodeNaive(AddrLEDCode_e code, bool blocking);
void AddrLED_NaiveISR(void);

void AddrLED_SanityTest(void);

