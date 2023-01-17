#include "main.h"
#include "addressable_led_manager.h"

typedef struct {
  uint8_t decrementRate;
  uint8_t waitTime;
  uint8_t backupR;
  uint8_t backupG;
  uint8_t backupB;
} LinesPixelData_t;

void Animation_Lines_Init(AddrLEDPanel_t *panels, uint8_t numPanels, LinesPixelData_t *pixelDataPtr);
void Animation_Lines_Update(void);
AnimationState_e Animation_Lines_GetState(void);
void Animation_Lines_SendMessage(AnimationMessage_t *message);
