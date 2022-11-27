#include <stdio.h>
#include "usart.h"

#define CNFG_DBG_UART_RX_BUFFER_SIZE 512
#define CNFG_DBG_UART_TX_BUFFER_SIZE 512
#define CNFG_DBG_CMD_BUFFER_SIZE 512

#define DBG_UART_INSTANCE UART2
#define DBG_UART_HANDLE   huart2

#warning "USART CMD UART"
#define logprint(...)      {uint16_t _size = sprintf((char *) &dbgUartTxBuffer, __VA_ARGS__) ; HAL_UART_Transmit(&DBG_UART_HANDLE, (uint8_t *) &dbgUartTxBuffer, _size, HAL_MAX_DELAY);}

extern char dbgUartTxBuffer[];

void DbgUart_Init(void);
void DbgUart_Isr(void);
void DbgUart_ProcessCommand(char *str);

uint16_t getRdr();
