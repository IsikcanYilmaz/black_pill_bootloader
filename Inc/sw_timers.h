#include "main.h"

typedef struct SwTimer_t
{
  bool running;
  uint32_t Ms;
  uint32_t remainingMs;
  uint32_t (*fn) (void);
  struct SwTimer_t *next;
  struct SwTimer_t *prev;
} SwTimer_t;

extern volatile uint32_t counterMs;

void SwTimer_ISR(void);
void SwTimer_InitTimer(SwTimer_t *timer, uint32_t ms);
void SwTimer_Start(SwTimer_t *timer);
void SwTimer_Stop(SwTimer_t *timer);
