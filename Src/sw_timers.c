#include "sw_timers.h"
#include "stm32l4xx.h"
#include "stm32l4xx_hal_cortex.h"
#include "stm32l4xx_hal.h"

/*
 * SOFTWARE TIMERS
 * Api that allows for the execution of arbitrary functions at specified time
 * A user is to create an instance of SwTimer_t, and populate the fn field 
 * with the desired callback function. 
 * How it works:
 * - $counter is incremented every millisecond.
 * - We have a "timer" structure that has a function pointer that gets called
 *   upon the structure's timeout_ms field reaching 0. This structure is 
 *   "link listable". 
 */

volatile uint32_t counterMs = 0;
SwTimer_t *timerListHead = NULL; 
SwTimer_t *timerListTail = NULL;


// Upon timeout or stop, remove $timer from the linked list.
static void SwTimer_Unlist(SwTimer_t *timer)
{
  SwTimer_t *originalPrev = timer->prev;
  SwTimer_t *originalNext = timer->next;
  originalPrev->next = originalNext;
  if (originalNext)
    originalNext->prev = originalPrev;
}

// Go through all registered timers and call their functions if they timeout
static void SwTimer_ProcessTimers(void)
{
  if (timerListHead == NULL)
  {
    return;
  }
  
  // Traverse thru the linked list of sw timer objects
  // Decrement their remaining times
  SwTimer_t *t = timerListHead;
  SwTimer_t *tNext = timerListHead->next;
  while(true)
  {
    if (t->running)
    {
      t->remainingMs--;

      // Check if this timer timed out
      if (t->remainingMs == 0)
      {
        // This timer timed out. Call its function and set the timers 
        // new timeout based on what the function returns.
        // If the function returns 0, remove this timer from the linked list.
        uint32_t newTimeout = t->fn(); 
        if (newTimeout)
        {
          t->remainingMs = newTimeout;
        }
        else
        {
          SwTimer_Unlist(t);
        }
      }
    }

    t = tNext;
    tNext = t->next;

    if (t == NULL)
    {
      break;
    }
  }

}

// Starting the timer means linking it to the overall timer linked list
// and or flipping its $running field to true. Disable IRQs while doing so.
void SwTimer_Start(SwTimer_t *timer)
{
  CRITICAL_SECTION_BEGIN;

  if (timerListHead == NULL) // this is the first timer that's registered
  {
    timerListHead = timer;
    timerListTail = timer;
    timer->next = NULL;
    timer->prev = NULL;
  }
  else // not the first timer. add it to the end of the list
  {
    timer->prev = timerListTail;
    timerListTail->next = timer;
    timerListTail = timer;
  }
  timer->running = true;

  CRITICAL_SECTION_END;
}

void SwTimer_Stop(SwTimer_t *timer)
{
  timer->running = false;
  SwTimer_Unlist(timer);
}

// This is called upon the SysTick IRQ
void SwTimer_ISR(void)
{
  counterMs++;
  SwTimer_ProcessTimers();
}
