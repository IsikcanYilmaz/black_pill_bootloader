#include "dbg_uart.h"
#include "main.h"
#include "stm32l4xx_hal_usart.h"
#include "usart.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#include "addressable_led_manager.h"

/*
 * DBG UART
 * How it works:
 * - USART2 (RX: PA3, TX: PA2) is the uart peripheral used for debug prints/commands. The initialization of 
 *   it is done in main.c, outside of this module.
 * - It uses DMA to push out data. Receiving is done in an ISR.
 */

char dbgUartRxBuffer[CNFG_DBG_UART_RX_BUFFER_SIZE];
char dbgUartTxBuffer[CNFG_DBG_UART_TX_BUFFER_SIZE];
char cmdBuffer[CNFG_DBG_CMD_BUFFER_SIZE];

uint16_t input_cursor = 0;

uint16_t dbgUartRxHead = 0;
uint16_t dbgUartTxHead = 0;
uint16_t dbgUartRxTail = 0;
uint16_t dbgUartTxTail = 0;

void DbgUart_Init(void)
{
  HAL_UART_Receive_DMA(&DBG_UART_HANDLE, (uint8_t *) &dbgUartRxBuffer, CNFG_DBG_UART_RX_BUFFER_SIZE);
  __HAL_USART_ENABLE_IT(&DBG_UART_HANDLE, UART_IT_RXNE);
  __HAL_USART_ENABLE_IT(&DBG_UART_HANDLE, UART_IT_ORE);
  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_TC);
  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
}

void DbgUart_SanityTest(void)
{
}

void DbgUart_Isr(void)
{
  TOGGLE_ONBOARD_LED();
  uint16_t bytesSinceLastIndex = CNFG_DBG_UART_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
  uint8_t data = DBG_UART_HANDLE.Instance->RDR;
  uint32_t status = DBG_UART_HANDLE.Instance->ISR;
  __HAL_USART_CLEAR_FLAG(&DBG_UART_HANDLE, (1<<12));

  // WRITE TO INPUT BUFFER
  if (data == '\r') // If this is a new line/enter, take in command
  {
    dbgUartRxBuffer[input_cursor] = '\0';
    logprint("\r\n");
    DbgUart_ProcessCommand(&dbgUartRxBuffer, input_cursor);
    input_cursor = 0;
  }
  else if (data == '\b') // Backspace
  {
    if (input_cursor)
    {
      input_cursor--;
      dbgUartRxBuffer[input_cursor] = '\0';
      logprint("\b \b");
    }
  }
  else // All other characters
  {
    dbgUartRxBuffer[input_cursor] = data;
    input_cursor++;
    logprint("%c", data);
  }

  // BUFFER OVERFLOW
  if(input_cursor >= CNFG_DBG_UART_RX_BUFFER_SIZE){
    logprint("UART Rx buffer over flow!\n");
    input_cursor = 0;
  }
}

uint16_t getRdr()
{
  return huart2.Instance->RDR;
}

void test(char **argv)
{
  Position_e pos = NUM_SIDES;

  if (stringsSame(argv[1], "n"))
  {
    pos = NORTH;
  }
  else if (stringsSame(argv[1], "e"))
  {
    pos = EAST;
  }
  else if (stringsSame(argv[1], "s"))
  {
    pos = SOUTH;
  }
  else if (stringsSame(argv[1], "w"))
  {
    pos = WEST;
  }
  else if (stringsSame(argv[1], "t"))
  {
    pos = TOP;
  }
  else
  {
    logprint("BAD SIDE DESCRIPTOR! %s\n", argv[0]);
    return;
  }

  SET_PIXEL_ARGS args = {
    .pos = pos,
    .x = atoi(argv[2]),
    .y = atoi(argv[3]),
    .r = atoi(argv[4]),
    .g = atoi(argv[5]),
    .b = atoi(argv[6]),
  };
  logprint("test pos %d, x %d y %d, r%d g%d b%d\n", args.pos, args.x, args.y, args.r, args.g, args.b);
  AnimationMessage_t setPix = {SET_PIXEL, (uint64_t *) &args};
  AddrLEDManager_SendMessageToCurrentAnimation(&setPix); 
} 

void DbgUart_ProcessCommand(char *str, uint16_t size)
{
  logprint("CMD[%s]\n", str);
  char *argv[CNFG_DBG_MAX_ARGS];
  memset(argv, 0x00, sizeof(argv));
  
  uint16_t i = 0;
  uint16_t argc = 0;
  while(true)
  {
    while (str[i] == ' ' && i < size) // pass by the trailing spaces
    {
      str[i] = '\0';
      i++;
    }

    if (i == size)
    {
      break;
    }
    else
    {
      argv[argc] = &(str[i]);
      argc++;
    }

    while (str[i] != ' ' && i < size)
    {
      i++;
    }
  }

  if (argc == 0)
  {
    logprint("Empty cmd str\n");
    return;
  }

  // // //
  if (stringsSame(argv[0], "set_pixel"))
  {
    test(argv);
  }

  /*for (i = 0; i < argc; i++)*/
  /*{*/
    /*logprint("%d: %s\n", i, argv[i]);*/
  /*}*/
}
