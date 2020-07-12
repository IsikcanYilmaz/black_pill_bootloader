
#include "queue.h"
#include <stdio.h>

#define BUF_SIZE 10

int main()
{
  uint32_t testbuffer[BUF_SIZE];
  Queue_t q = Queue_Init(BUF_SIZE, (uint32_t *) &testbuffer);
  uint32_t test;
  Queue_Enqueue(&q, 1);
  Queue_Enqueue(&q, 2);
  Queue_Enqueue(&q, 3);
  Queue_Enqueue(&q, 4);
  Queue_Dequeue(&q, &test);
  Queue_Dequeue(&q, &test);
  Queue_Dequeue(&q, &test);
  Queue_Dequeue(&q, &test);
  Queue_Enqueue(&q, 11);
  Queue_Enqueue(&q, 21);
  Queue_Enqueue(&q, 31);
  Queue_Enqueue(&q, 41);
  Queue_Enqueue(&q, 11);
  Queue_Enqueue(&q, 21);
  Queue_Enqueue(&q, 31);
  Queue_Enqueue(&q, 41);
  Queue_Dequeue(&q, &test);
  Queue_Dequeue(&q, &test);
  Queue_Dequeue(&q, &test);
  Queue_Dequeue(&q, &test);
  Queue_Enqueue(&q, 12);
  Queue_Enqueue(&q, 22);
  Queue_Enqueue(&q, 32);
  Queue_Enqueue(&q, 42);
}
