#include "sw_timers.h"

volatile uint32_t tmrCounter_ms = 0;



















void HAL_SYSTICK_Callback(void)
{
  tmrCounter_ms++;  
}
