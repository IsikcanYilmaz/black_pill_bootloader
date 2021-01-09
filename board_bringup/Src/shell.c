
#include "shell.h"
#include "i2c.h"
#include "stm32l4xx_hal_i2c.h"
#include "stm32l4xx_hal_def.h"
#include "stdio.h"
#include <string.h>

/*
 * Simple shell
 * - Pressing Enter issues the command
 */

uint8_t ShellRxBuffer[SHELL_RX_BUFFER_SIZE];
uint32_t ShellRxBufferHead = 0;
bool ShellCommandStarted = false;

const char *ShellNewline = "\r\n";

static void testFn(void){
  CDC_Transmit_FS("TEST", strlen("test"));
}

const Shell_Command_Entry_t ShellCommands[] = {
  { "test" , &testFn , false }
};

GENERIC_STATUS_e Shell_Init(void)
{

}

GENERIC_STATUS_e Shell_CdcReceive(uint8_t *Buf, uint32_t Len)
{
  GENERIC_STATUS_e ret = GENERIC_STATUS_OK;

  // Check if there's enough space in our buffer. if not, reset buffer
  if (Len + ShellRxBufferHead > SHELL_RX_BUFFER_SIZE)
  {
    ShellRxBufferHead = 0;
    return GENERIC_STATUS_BUFFER_OVERFLOW;
  }

  // Copy over the entire buffer. Check if there's a newline to signify
  // a command being terminated/entered.
  for (uint32_t i = 0; i < Len; i++)
  {
    if (Buf[i] == SHELL_CMD_TERMINATOR_CHAR)
    {
      Shell_ProcessCommand();
      ShellRxBufferHead = 0;
    }
    else
    {
      ShellRxBuffer[ShellRxBufferHead] = Buf[i];
      ShellRxBufferHead++;
    }
  }

#if SHELL_ECHO
  CDC_Transmit_FS(Buf, Len);
#endif
}

static void BoardBringupTest(void)
{
  // LSM6DS3 I2C address 110101xb. i.e. (0x35 << 1)
  uint16_t devAddr = 0x35 << 1;
  uint16_t buf = 0;
  char out[256];

/**
  * @brief  Transmits in master mode an amount of data in blocking mode.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
/*HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)*/
  
  HAL_StatusTypeDef ret = HAL_I2C_Master_Receive(&hi2c1, devAddr, &buf, 1, 5000);


  memset(&out, 0x00, sizeof(out));
  sprintf(&out, "ret %d buf %d", ret, buf);
  
  CDC_Transmit_FS(out, strlen(out));
}

// Process the command string that populates our RxBuffer
GENERIC_STATUS_e Shell_ProcessCommand(void)
{
  // For now, just take the command "doit" and use this command to do stuff. we'll look into command string handling later TODO
  //int strcmp(const char *s1, const char *s2);
  if (ShellRxBufferHead > 0 && strncmp("doit", ShellRxBuffer, ShellRxBufferHead) == 0)
  {
    BoardBringupTest();
  }
  ShellRxBufferHead = 0;

  /*// First find what command this is. So find the first occurance of "space"*/
  /*uint16_t spaceIdx = 0;*/
  /*for (uint16_t i = 0; i < ShellRxBufferHead; i++)*/
  /*{*/
    /*if (ShellRxBuffer[i] == ' ')*/
    /*{*/
      /*spaceIdx = i;*/
      /*break;*/
    /*}*/
  /*}*/

  /*// Go thru all registered commands, command strings. */
  /*// Call the matching command callback function*/
  /*for (int i = 0; i < ShellRxBufferHead; i++)*/
  /*{*/
    /*for (int c = 0; c < sizeof(ShellCommands); c++)*/
    /*{*/
    /*// TODO*/
    /*}*/
  /*}*/

  // Print newline
  CDC_Transmit_FS(ShellNewline, strlen(ShellNewline));
  return GENERIC_STATUS_OK;
}
