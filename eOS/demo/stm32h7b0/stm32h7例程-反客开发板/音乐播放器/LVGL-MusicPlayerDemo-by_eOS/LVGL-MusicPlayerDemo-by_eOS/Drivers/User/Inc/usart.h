#ifndef __USART_H
#define __USART_H

#include "main.h"
//STM32H7B0VBT6 : ROM(FLASH)-128K  RAM(SRAM)-1184K

//(USART1)
#define UART1_DMA_RXBUFF_LEN 512    //DMA串口接收缓存，分配512B
#define UART1_RXBUFF_LEN 	 512    //串口接收分配512B
#define UART1_TXBUFF_LEN 	 512    //分配512B


typedef struct
{
	uint32_t rx_len;
	uint32_t tx_len;
	uint8_t bool_rx_ok;
	uint8_t bool_tx_ok;
	uint8_t bool_rx_dma_full;
	uint8_t rx_dma_buff[UART1_DMA_RXBUFF_LEN];
	uint8_t rx_buff[UART1_RXBUFF_LEN];
	uint8_t tx_buff[UART1_TXBUFF_LEN];	
	uint8_t bool_rx_ing;
	uint8_t bool_tx_ing;
}T_UART1;
extern T_UART1 usart1;

void usart1_init(uint32_t bardrate);

#endif //__USART_H





