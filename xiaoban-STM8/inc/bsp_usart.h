#ifndef __BSP_USART_H
#define __BSP_USART_H

//#include "stm32f7xx_hal.h"
#include "iostm8s103F3.h"
#include "bsp_queue.h"

typedef enum
{
	UART1_Select = 1,
	UART2_Select,
    UART3_Select,
}UARTx_Select_TypeDef;  //串口选择枚举体

typedef void (*data_deal_cb)(unsigned char *pdata, unsigned char len);  //数据解析回调函数

void Uart_Init(UARTx_Select_TypeDef uartx, unsigned long int bound);

void uart_write(UARTx_Select_TypeDef uartx, unsigned char *pdata, unsigned char len);
void uart_read(UARTx_Select_TypeDef uartx, data_deal_cb callback);

#endif
