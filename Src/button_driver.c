#include "button_driver.h"
#include "sw_timers.h"

/*
 * BUTTON DRUVER
 * How it works:
 * - Currently there's only one button on the light cube so this driver 
 *   is rather relaxed.
 * - GPIO edge triggers IRQ and has us call ButtonDriver_ISR. It starts the
 *   debounce timer. Debounce check is done upon the timeout of it.
 */

SwTimer_t buttonDebounceTimer = {.fn = NULL, .remainingMs = 0};
SwTimer_t buttonSequenceTimer = {.fn = NULL, .remainingMs = 0};

ButtonContext_t userButton1;

static uint32_t DebounceTimeout(void)
{
  GPIO_PinState state = HAL_GPIO_ReadPin(userButton1.port, userButton1.pin); 

  // Check if this is a succesfull button press
  if (state == userButton1.debounceCheck && userButton1.debouncePending)
  {
    // TODO //
    userButton1.debouncePending = false;
  }
  return 0;
}

static uint32_t SequenceTimeout(void)
{
  // TODO Input sequences
  return 0;
}

void ButtonDriver_Init(void)
{
  // Initialize our times
  buttonDebounceTimer.fn = DebounceTimeout;
  buttonDebounceTimer.remainingMs = CNFG_BUTTON_DEBOUNCE_TIME_MS;
  buttonSequenceTimer.fn = SequenceTimeout;

  // Initialize our button contexts
  userButton1.port = GPIOC;
  userButton1.pin  = GPIO_PIN_13;
}


void ButtonDriver_ISR(void)
{
  if (!userButton1.debouncePending)
  {
    CRITICAL_SECTION_BEGIN; // TODO // this may not be crit section worthy
    userButton1.debouncePending = true;
    GPIO_PinState state = HAL_GPIO_ReadPin(userButton1.port, userButton1.pin);
    userButton1.debounceCheck = state;
    SwTimer_Start(&buttonDebounceTimer);
    CRITICAL_SECTION_END;
  }
}
