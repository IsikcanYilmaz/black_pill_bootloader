
#include "cmd_shell.h"

uint8_t cmdShellReceiveBuffer[RECEIVE_BUFFER_LENGTH];
uint16_t cmdShellReceiveBufferIdx = 0;

void CmdShellReceive(uint8_t *buf, uint16_t len)
{
  // End of a command is denoted with a '\r'
  // If this isnt the end of a command, copy the character to the buffer
  bool isEndOfCommand = (strcmp(buf, "\r") == 0);
  bool isMultiChar = (len > 0); // TODO
  if (!isEndOfCommand)
  {
    memcpy(&cmdShellReceiveBuffer[cmdShellReceiveBufferIdx], buf, 1);
    cmdShellReceiveBufferIdx++;
  }
  else
  {
    memcpy(&cmdShellReceiveBuffer[cmdShellReceiveBufferIdx], "\r\n", 2);
    cmdShellReceiveBufferIdx += 2;
    CmdShellSendString(&cmdShellReceiveBuffer);
    memset(&cmdShellReceiveBuffer, 0, cmdShellReceiveBufferIdx);
    cmdShellReceiveBufferIdx = 0;
  }
}

void CmdShellSendString(char *str)
{
  uint8_t res = CDC_Transmit_FS(str, strlen(str));
}
