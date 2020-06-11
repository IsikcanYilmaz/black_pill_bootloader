#include "main.h"
#include "addressable_led_manager.h"

typedef struct {
  uint8_t decrementRate;
  uint8_t waitTime;
  uint8_t redDecrementRate;
  uint8_t greenDecrementRate;
  uint8_t blueDecrementRate;
} RandomFadePixelData_t;

void Animation_RandomFade_Init(AddrLEDPanel_t *panels, uint8_t numPanels, RandomFadePixelData_t *pixelDataPtr);
