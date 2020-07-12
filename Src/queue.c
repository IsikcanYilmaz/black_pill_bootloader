#include "queue.h"

/* 
 * QUEUE API
 * This file defines queue functionality that can store uint32_t data
 * structures. The user must allocate the buffer space the queue will use
 * outside of the init call, must pass the pointer to the buffer to the init,
 * and get the queue handle from the init function.
 */

Queue_t Queue_Init(uint16_t capacity, uint32_t *bufferPtr)
{
  Queue_t queue;
  queue.capacity = capacity;
  queue.queueBuffer = bufferPtr;
  queue.head = 0;
  queue.tail = 0;
  queue.size = 0;
  memset(bufferPtr, 0x00, sizeof(uint32_t) * capacity);
  return queue;
}

QueueError_e Queue_Dequeue(Queue_t *queue, uint32_t *element)
{
  if (queue->size == 0)
    return QUEUE_EMPTY;
  *element = queue->queueBuffer[queue->head];
#if CNFG_QUEUE_RESET_AFTER_DEQUEUE
  queue->queueBuffer[queue->head] = 0x0;
#endif
  queue->head = (queue->head + 1) % queue->capacity;
  queue->size--;
  return NO_ERROR;
}

QueueError_e Queue_Enqueue(Queue_t *queue, uint32_t element)
{
  if (queue->size == queue->capacity)
    return QUEUE_FULL;
  queue->queueBuffer[queue->tail] = element;
  queue->tail = (queue->tail + 1) % queue->capacity;
  queue->size++;
  return NO_ERROR;
}

uint16_t Queue_GetSize(Queue_t *queue)
{
  return queue->size;
}

bool Queue_IsFull(Queue_t *queue)
{
  return (queue->size == queue->capacity);
}

