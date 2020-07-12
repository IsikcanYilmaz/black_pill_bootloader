#include "main.h"
#include "events.h"
#include "queue.h"

/*
 * EVENTS, EVENT QUEUE
 * Basically simply a wrapper around the queue api. main loop processes
 * these events
 */

uint32_t eventQueueBuffer[CNFG_EVENTS_QUEUE_SIZE];
Queue_t eventQueueHandle;

void Events_Init(void)
{
  eventQueueHandle = Queue_Init(CNFG_EVENTS_QUEUE_SIZE, (uint32_t *) &eventQueueBuffer);
}

void Events_Enqueue(Event_t e)
{
  Queue_Enqueue(&eventQueueHandle, e);
}

Event_t Events_Dequeue(void)
{
  Event_t e = NULL;
  Queue_Dequeue(&eventQueueHandle, &e);
  return e;
}
