#define RAND_IN_RANGE(min, max) (rand() % (max - min + 1) + min)
#define TOGGLE_ONBOARD_LED() HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
#define IDLE_FOREVER(x)      do {TOGGLE_ONBOARD_LED(); HAL_Delay(x);} while(1)
