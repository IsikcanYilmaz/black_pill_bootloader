#include "main.h"
#include "addressable_led_manager.h"

typedef struct {
  uint8_t decrementRate;
  uint8_t waitTime;
  uint8_t backupR;
  uint8_t backupG;
  uint8_t backupB;
} RandomTrianglesPixelData_t;

void Animation_RandomTriangles_Init(AddrLEDPanel_t *panels, uint8_t numPanels, RandomTrianglesPixelData_t *pixelDataPtr);
void Animation_RandomTriangles_Update(void);
AnimationState_e Animation_RandomTriangles_GetState(void);
void Animation_RandomTriangles_SendMessage(AnimationMessage_t *message);
