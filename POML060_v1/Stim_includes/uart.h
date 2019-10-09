
#ifdef __cplusplus
extern "C" {
#endif

#define UART_Reset(uart)
#define PLIB_USART_INDEX_GUI 0



typedef int USART_MODULE_ID;

size_t UART_Write(const USART_MODULE_ID id, char * source, size_t nBytes);
// Says there is always plenty of space in the transmit buffer
size_t UART_WriteBytesAvailable(const USART_MODULE_ID id);

void UART_PC_Buffer_Flush(const USART_MODULE_ID id);
size_t UART_PC_Buffered_Write(const USART_MODULE_ID id, char * source, size_t nBytes);
#define UART_Write UART_PC_Buffered_Write

#ifdef __cplusplus
}
#endif
