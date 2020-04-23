
#include <stdint.h>
#include <stdbool.h>
#include "stm32f3xx_hal.h"
//#include "cmsis_armcc.h" // for __set_PRIMASK , __set_MSP
//#include "stm32f3xx_hal_flash.h" 

typedef void (*pFunction)(void);
static pFunction AppStart;

int main(void)
{
  __set_PRIMASK(1);
  volatile uint32_t appAddr;  // Beginning of the application code
  volatile uint32_t jumpAddr; // Reset_Handler location

  appAddr  = 0x0; //APP_VECTOR_ADDRESS;
  jumpAddr = * (volatile uint32_t *) (appAddr + 4); 
  AppStart = (pFunction) jumpAddr;

  volatile bool block = true;
  while(block) {}

  __set_MSP(*(volatile uint32_t *) appAddr);
  AppStart();


  // NEVER REACH HERE
  while(1) {}
  
  return 0;
}
