#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define CNFG_QUEUE_RESET_AFTER_DEQUEUE true

typedef struct {
  uint16_t capacity;
  uint16_t size;
  uint16_t head;
  uint16_t tail;
  uint32_t *queueBuffer;
} Queue_t;

typedef enum {
  NO_ERROR,
  QUEUE_FULL,
  QUEUE_EMPTY,
} QueueError_e;

Queue_t Queue_Init(uint16_t capcity, uint32_t *bufferPtr);
QueueError_e Queue_Dequeue(Queue_t *queue, uint32_t *element);
QueueError_e Queue_Enqueue(Queue_t *queue, uint32_t element);
uint16_t Queue_GetSize(Queue_t *queue);
bool Queue_IsFull(Queue_t *queue);


