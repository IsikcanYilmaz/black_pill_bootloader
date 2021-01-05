
#include "main.h"

#define SHELL_RX_BUFFER_SIZE 512
#define SHELL_CMD_TERMINATOR_CHAR '\r'
#define SHELL_ECHO true

typedef struct 
{
  char *cmdStr;
  void (*cmdFunction)(void);
  bool match;
} Shell_Command_Entry_t;

GENERIC_STATUS_e Shell_Init(void);
GENERIC_STATUS_e Shell_CdcReceive(uint8_t *Buf, uint32_t Len);
GENERIC_STATUS_e Shell_ProcessCommand(void);
