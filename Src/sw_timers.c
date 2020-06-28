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
  // One of three things may happen at unlisting a timer
  // - the timer at the head is to be unlisted
  // - the timer at the tail
  // - a timer in between

  CRITICAL_SECTION_BEGIN;

  SwTimer_t *originalPrev = timer->prev;
  SwTimer_t *originalNext = timer->next;

  // this is the head of the list update our head
  if (timer == timerListHead)
  {
    if (timer->next)
      timerListHead = timer->next;
    else
      timerListHead = NULL;
  }

  // this is the tail of the list update our tail
  if (timer == timerListTail)
  {
    if (timer->prev)
      timerListTail = timer->prev;
    else
      timerListTail = NULL;
  }

  // this is a timer in the middle
  if (originalPrev)
  {
    if (originalNext)
      originalPrev->next = originalNext;
  }

  if (originalNext)
  {
    if (originalPrev)
      originalNext->prev = originalPrev;
  }

  CRITICAL_SECTION_END;
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
      // Check if this timer timed out. if not decrement its remaining ms
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
      else
      {
        t->remainingMs--;
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
  timer->remainingMs = timer->Ms;

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
