
#include "led.h"
#include "tim.h"

LEDInstance led1;


void LED_Init(void)
{
  // Initialize LED 1
  led1 = (LEDInstance) {
    .channelBrightness = {0, 0, 0},
    .pwmTimerHandle = &htim2,
    .updateTimerHandle = &htim1,
    .timChannel = {
      [RED]   = TIM_CHANNEL_1, 
      [GREEN] = TIM_CHANNEL_2, 
      [BLUE]  = TIM_CHANNEL_3
    },
  };

  // Start LED 1s timers
  MX_TIM1_Init(); // update timer 
  MX_TIM2_Init(); // pwm timer

  // Start LED 1
  LED_Start(&led1);
}

void LED_Start(LEDInstance *led)
{
  // Start pwms
  HAL_TIM_PWM_Start(led->pwmTimerHandle, led->timChannel[RED]);
  HAL_TIM_PWM_Start(led->pwmTimerHandle, led->timChannel[GREEN]);
  HAL_TIM_PWM_Start(led->pwmTimerHandle, led->timChannel[BLUE]);

  // Start update timer
  led->updateTimerHandle->Instance->ARR = 10; 
  HAL_TIM_GenerateEvent(led->updateTimerHandle, TIM_EVENTSOURCE_UPDATE);
  HAL_TIM_Base_Start_IT(led->updateTimerHandle);
}

void LED_SetBrightness(LEDInstance *led, LEDColorChannel color, uint16_t brightness)
{
  __HAL_TIM_SET_COMPARE(led->pwmTimerHandle, 
                        led->timChannel[color],
                        brightness);
  led->channelBrightness[color] = brightness;
}

void LED_Update(void)
{
  static int i = 100;
  static bool add = true;

  static LEDColorChannel goingUp   = RED;
  static LEDColorChannel goingDown = GREEN;

  LED_SetBrightness(&led1, BLUE, i);
  //LED_SetBrightness(&led1, GREEN, i);
  LED_SetBrightness(&led1, RED, 999-i);
  
  i += (add) ? 1 : -1;
  
  if (i > 999)
  {
    add = false;
  }
  if (i < 100)
  {
    add = true;

    // ROTATE COLORS
    //goingUp   = (goingUp + 1) % (NUM_COLORS);
  }

  
}

