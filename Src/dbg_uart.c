#include "dbg_uart.h"
#include "main.h"
#include "stm32l4xx_hal_uart.h"
#include "usart.h"

/*
 * DBG UART
 * How it works:
 * - USART2 (RX: PA3, TX: PA2) is the uart peripheral used for debug prints/commands. The initialization of 
 *   it is done in main.c, outside of this module.
 * - It uses DMA to push out data. Receiving is done in an ISR.
 */

uint8_t dbgUartRxBuffer[CNFG_DBG_UART_RX_BUFFER_SIZE];
uint8_t dbgUartTxBuffer[CNFG_DBG_UART_TX_BUFFER_SIZE];

uint16_t dbgUartRxHead = 0;
uint16_t dbgUartTxHead = 0;
uint16_t dbgUartRxTail = 0;
uint16_t dbgUartTxTail = 0;

void DbgUart_Init(void)
{
  //__HAL_UART_ENABLE_IT(&DBG_UART_HANDLE, UART_IT_RXNE);
//HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
 HAL_UART_Receive_IT(&DBG_UART_HANDLE, &dbgUartRxBuffer, 1);
 HAL_UART_Transmit(&DBG_UART_HANDLE, "ASD", 3, 100);
}

void DbgUart_Isr(void)
{
  //if (__HAL_UART_GET_IT(DBG_UART_HANDLE, UART_IT_RXNE))
  //{
    //DBG_UART_INSTANCE->RDR
  //}
}
