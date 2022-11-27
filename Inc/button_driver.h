#include "main.h"
#include "stm32l4xx.h"

#define CNFG_BUTTON_DEBOUNCE_TIME_MS 3

// Sequence lengths in Ms
#define CNFG_BUTTON_SHORT_PRESS_MS 100
#define CNFG_BUTTON_LONG_PRESS_MS  800
#define CNFG_BUTTON_VLONG_PRESS_MS 3000
#define CNFG_BUTTON_VVLONG_PRESS_MS 10000

#define CNFG_BUTTON_PRESSED_STATE (GPIO_PIN_RESET)
#define CNFG_BUTTON_RELEASED_STATE (GPIO_PIN_SET)

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pin;
  GPIO_PinState debounceCheck;
  bool debouncePending; 
  bool pressed;
  uint32_t lastInputMs;
} ButtonContext_t;

typedef enum
{
  SHORT_RELEASE,
  LONG_RELEASE,
  VLONG_RELEASE,
  VVLONG_RELEASE,
} ButtonEvents_e;

void ButtonDriver_Init(void);
void ButtonDriver_ISR(void);
