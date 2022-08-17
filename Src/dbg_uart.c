#include "dbg_uart.h"
#include "main.h"
#include "stm32l4xx_hal_usart.h"
#include "usart.h"
#include <string.h>
#include "utils.h"

/*
 * DBG UART
 * How it works:
 * - USART2 (RX: PA3, TX: PA2) is the uart peripheral used for debug prints/commands. The initialization of 
 *   it is done in main.c, outside of this module.
 * - It uses DMA to push out data. Receiving is done in an ISR.
 */

char dbgUartRxBuffer[CNFG_DBG_UART_RX_BUFFER_SIZE];
char dbgUartTxBuffer[CNFG_DBG_UART_TX_BUFFER_SIZE];

uint16_t input_cursor = 0;

uint16_t dbgUartRxHead = 0;
uint16_t dbgUartTxHead = 0;
uint16_t dbgUartRxTail = 0;
uint16_t dbgUartTxTail = 0;

void DbgUart_Init(void)
{
  HAL_UART_Receive_IT(&DBG_UART_HANDLE, (uint8_t *) &dbgUartRxBuffer, CNFG_DBG_UART_RX_BUFFER_SIZE);
}

void DbgUart_SanityTest(void)
{
}

void DbgUart_Isr(void)
{
  TOGGLE_ONBOARD_LED();
  uint8_t data = DBG_UART_HANDLE.Instance->RDR;
  // IF NEW LINE THIS IS A COMMAND
  if (data == (uint8_t)']'){
    logprint("COMMAND %s \n", dbgUartRxBuffer);
    /*HAL_UART_Receive_IT(&DBG_UART_HANDLE, (uint8_t *) &dbgUartRxBuffer, CNFG_DBG_UART_RX_BUFFER_SIZE);*/
    /*memset(&dbgUartRxBuffer, 0, CNFG_DBG_UART_RX_BUFFER_SIZE);*/
    /*input_cursor = 0;*/
  } else {
    logprint("char %c 0x%x huartSize %d huartCount %d\n", data, data, DBG_UART_HANDLE.RxXferSize, DBG_UART_HANDLE.RxXferCount);
  }
  input_cursor++;

  // WRITE TO INPUT BUFFER
  /*dbgUartRxBuffer[input_cursor] = data;*/
  /*input_cursor++;*/
  /*if(input_cursor >= CNFG_DBG_UART_RX_BUFFER_SIZE){*/
    /*input_cursor = 0;*/
  /*}*/
}

#if 0
void cmd_shell_isr(){
  // HANDLE POTENTIAL ERRORS
  uint32_t status = huart2.Instance->SR;
  if (huart2.Instance->CR1 & USART_IT_TXE & status){  // TRANSMIT DATA BUFFER EMPTY

  }

  if (status & USART_SR_FE) { // FRAMING ERROR

  }

  if (status & USART_SR_ORE) { // OVERRUN ERROR

  }

  if (status & USART_SR_RXNE) { // READ DATA BUFFER NOT EMPTY
    uint8_t data = huart2.Instance->DR;

    // IF NEW LINE THIS IS A COMMAND
    if (data == (uint8_t)'\n'){
      print("COMMAND %s \n", dbgUartRxBuffer);
      memset(&dbgUartRxBuffer, 0, USART_IN_BUFFER_SIZE);
    } else {
      print("char %c 0x%x\n", data, data);
    }

    // WRITE TO INPUT BUFFER
    dbgUartRxBuffer[dbgUartRxBuffer_cursor] = data;
    dbgUartRxBuffer_cursor++;
    if(dbgUartRxBuffer_cursor >= USART_IN_BUFFER_SIZE){
      dbgUartRxBuffer_cursor = 0;
    }
  }
}
#endif

uint16_t getRdr()
{
  return huart2.Instance->RDR;
}
