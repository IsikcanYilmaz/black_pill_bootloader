#include "main.h"
#include "dbg_uart.h"

/*
 * DBG UART
 * How it works:
 * - USART2 (RX: PA3, TX: PA2) is the uart peripheral used for debug prints/commands. The initialization of 
 *   it is done in main.c, outside of this module.
 * - It uses DMA to receive and push out data.
 */


void DbgUart_Init(void)
{

}


