
#include "main.h"
#include "events.h"

#define CMDSHELL_RECEIVE_BUFFER_LENGTH 512
#define CMDSHELL_ECHO_BACK false
#define CMDSHELL_COMMAND_LENGTH 10

typedef enum 
{
  LOOPBACK,
  TEST_CMD,
  LED_SET_COLOR,
  START_DFU,
  NUM_COMMANDS
} CmdShellCommand_e;

typedef struct
{
  CmdShellCommand_e cmdId;
  char cmdStr[CMDSHELL_COMMAND_LENGTH];
} CmdShellCommand_t;

void CmdShellReceive(uint8_t *buf, uint16_t len);
void CmdShellSendString(char *str);
void CmdShellEventHandler(Event_t *e);
