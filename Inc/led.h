
#include "main.h"

#define LED_UPDATE_TIMER TIM1
#define LED_PWM_TIMER    TIM2

typedef enum 
{
  RED,
  GREEN,
  BLUE,
  NUM_COLORS
} LEDColorChannel;

typedef struct 
{ 
  uint16_t          timChannel[3];
  uint16_t          channelBrightness[3];
  TIM_HandleTypeDef *pwmTimerHandle;
  TIM_HandleTypeDef *updateTimerHandle;
} LEDInstance;

void LED_Init(void);
void LED_Start(LEDInstance *led);
void LED_Update(void);
void LED_SetBrightness(LEDInstance *led, LEDColorChannel color, uint16_t brightness);

