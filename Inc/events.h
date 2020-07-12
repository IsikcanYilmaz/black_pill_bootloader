
#include "main.h"

#define CNFG_EVENTS_QUEUE_SIZE 128

typedef enum
{
  LED_MODULE,
  IO_MODULE ,
  DFU_MODULE,
  CMD_MODULE,
  NUM_MODULES = CMD_MODULE,
  NUM_MODULES_MAX = 0xffff
} Module_e;

typedef enum
{
  // LED events
  LED_CHANGE_SEQUENCE,
  LED_SET_COLOR_VAL,

  // IO events
  IO_BUTTON_PRESS,
  IO_BUTTON_DEBOUNCE,

  // DFU events
  DFU_BEGIN,
  DFU_DONE,

  // CMD shell events
  // ...

  // 
  NUM_EVENTS,
  NUM_EVENTS_MAX = 0xffff
} Event_e;

typedef struct 
{
  Module_e mod;
  Event_e event;
} __attribute__((packed)) Event_t;

void Events_Init(void);
void Events_Enqueue(Event_t e);


