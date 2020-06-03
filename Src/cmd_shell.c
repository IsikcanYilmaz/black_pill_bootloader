
#include "cmd_shell.h"
#include "usbd_cdc_if.h"
#include <string.h>

uint8_t cmdShellReceiveBuffer[CMDSHELL_RECEIVE_BUFFER_LENGTH];
uint16_t cmdShellReceiveBufferIdx = 0;

static void CmdShellProcessCommand(char *buf);
static void CmdShellResetBuffer(void);

// Define all shell commands here
CmdShellCommand_t ShellCommandList[] = 
{
  {LOOPBACK,      "loopback" },
  {TEST_CMD,      "test_cmd" },
  {LED_SET_COLOR, "set_color"},
  {START_DFU,     "start_dfu"}
};

// Receive a character from USB CDC COM port, copy it to our buffer
void CmdShellReceive(uint8_t *buf, uint16_t len)
{
  // End of a command is denoted with a '\r'
  // If this isnt the end of a command, copy the character to the buffer
  bool isEndOfCommand = (strcmp(buf, "\r") == 0);
  bool isMultiChar = (len > 0); // TODO
  if (!isEndOfCommand)
  {
    memcpy(&cmdShellReceiveBuffer[cmdShellReceiveBufferIdx], buf, 1);
    CmdShellSendString(&cmdShellReceiveBuffer[cmdShellReceiveBufferIdx]);
    cmdShellReceiveBufferIdx++;
  }
  else
  {
    // Command received, process it, reset buffer
    CmdShellProcessCommand(&cmdShellReceiveBuffer);
#if CMDSHELL_ECHO_BACK
    memcpy(&cmdShellReceiveBuffer[cmdShellReceiveBufferIdx], "\r\n", 2);
    cmdShellReceiveBufferIdx += 2;
    CmdShellSendString(&cmdShellReceiveBuffer);
#endif
    CmdShellResetBuffer();
  }
}

// Print string thru the USB COM port
void CmdShellSendString(char *str)
{
  uint8_t res = CDC_Transmit_FS(str, strlen(str));
}

// Handle events
void CmdShellEventHandler(Event_t *e)
{
  // TODO
}

// Zero out the input buffer and index
static void CmdShellResetBuffer(void)
{
  memset(&cmdShellReceiveBuffer, 0, cmdShellReceiveBufferIdx);
  cmdShellReceiveBufferIdx = 0;
}

// Process shell command
static void CmdShellProcessCommand(char *buf)
{
  // TODO // Write a better lookup function
  for (int i = 0; i < NUM_COMMANDS; i++)
  {
    // Check if the command matches anything
    if (strcmp(buf, (char *) &(ShellCommandList[i].cmdStr)) == 0)
    {
      CmdShellSendString((char *) &(ShellCommandList[i].cmdStr));
      CmdShellSendString("ASD");
      break;
    }
  }
  CmdShellSendString("\r\n");
}
