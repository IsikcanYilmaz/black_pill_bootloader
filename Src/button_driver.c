#include "button_driver.h"
#include "sw_timers.h"
#include "addressable_led_manager.h"

/*
 * BUTTON DRIVER
 * How it works:
 * - Currently there's only one button on the light cube so this driver 
 *   is rather relaxed. We'll assume there's only one button.
 * - GPIO edge triggers IRQ and has us call ButtonDriver_ISR. It starts the
 *   debounce timer. Debounce check is done upon the timeout of it.
 * - This driver is responsible of figuring out the sequence of button inputs.
 * - Sequences can be made up of button presses and a press&hold that can end
 *   a sequence. for example 2 short releases and 1 long press, tap tap hold.
 *   Can be a single hold. can be a single tap.
 * - Holds will have four time lengths: normal, long, very long, very very long
 *
 */

SwTimer_t buttonDebounceTimer = {.fn = NULL, .Ms = 0};
SwTimer_t buttonSequenceTimer = {.fn = NULL, .Ms = 0};
SwTimer_t buttonHoldTimer     = {.fn = NULL, .Ms = 0};

ButtonContext_t userButton1;

static uint32_t DebounceTimeout(void)
{
  GPIO_PinState state = HAL_GPIO_ReadPin(userButton1.port, userButton1.pin); 
  // Check if this is a succesfull button press
  if (state == userButton1.debounceCheck && userButton1.debouncePending)
  {
    uint32_t deltaT = SwTimer_GetCountMs() - userButton1.lastInputMs;
    if (state == GPIO_PIN_SET) // PRESS 
    {
      userButton1.pressed = true;
    }
    else // state == GPIO_PIN_RESET // RELEASE
    {
      if (userButton1.pressed == true)
      {
        AddrLEDManager_PlayNextAnimation();
        if (deltaT < CNFG_BUTTON_SHORT_PRESS_MS)
        {
          // Short press happened
        }
        else if (deltaT < CNFG_BUTTON_LONG_PRESS_MS)
        {
          // Long press happened
        }
        else if (deltaT < CNFG_BUTTON_VLONG_PRESS_MS)
        {
          // Very long press happened (3second)
        }
        else if (deltaT < CNFG_BUTTON_VVLONG_PRESS_MS)
        {
          // Very very long press happened (10 second)
        }
        else
        {
          // Longer than 10seconds. not defined currently
        }
      }
      userButton1.pressed = false;
    }

    userButton1.lastInputMs = SwTimer_GetCountMs();
  }
  userButton1.debouncePending = false;
  return 0;
}

static uint32_t SequenceTimeout(void)
{
  // TODO Input sequences
  return 0;
}

static uint32_t HoldTimeout(void)
{
  // TODO Hold sequences
  return 0;
}

void ButtonDriver_Init(void)
{
  // Initialize our times
  buttonDebounceTimer.fn = DebounceTimeout;
  buttonDebounceTimer.Ms = CNFG_BUTTON_DEBOUNCE_TIME_MS;
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
