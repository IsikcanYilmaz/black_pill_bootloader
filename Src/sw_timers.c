#include "sw_timers.h"
#include "stm32l4xx.h"
#include "stm32l4xx_hal_cortex.h"

volatile uint32_t tmrCounter_ms = 0;















void HAL_SYSTICK_Callback(void)
{
  tmrCounter_ms++;  
}
