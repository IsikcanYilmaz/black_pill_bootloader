
#define CNFG_DBG_UART_RX_BUFFER_SIZE 256
#define CNFG_DBG_UART_TX_BUFFER_SIZE 256

#define DBG_UART_INSTANCE UART2
#define DBG_UART_HANDLE   huart2

void DbgUart_Init(void);
void DbgUart_Isr(void);

