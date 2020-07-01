#include "main.h"
#include "stm32l4xx.h"

#define CNFG_BUTTON_DEBOUNCE_TIME_MS 3

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pin;
  GPIO_PinState debounceCheck;
  bool debouncePending; 
} ButtonContext_t;

void ButtonDriver_Init(void);
void ButtonDriver_ISR(void);
