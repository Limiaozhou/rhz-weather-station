#ifndef _USART_H_
#define _USART_H_

#include "sys.h"
//#include "stm32f10x_usart.h"
#define UART_RX_LEN (300)
//设置FLAG，置1时表示是该串口接收的数据
//#include "stm32f10x.h"

extern unsigned char USART1_RX_BUF[UART_RX_LEN]; //????,??USART_REC_LEN???.??????? 
extern unsigned char USART2_RX_BUF[UART_RX_LEN]; //????,??USART_REC_LEN???.??????? 
extern unsigned char USART3_RX_BUF[UART_RX_LEN]; //????,??USART_REC_LEN???.??????? 
//extern unsigned char UART4_RX_BUF[UART_RX_LEN];
//extern unsigned char USART_RX_BUF[UART_RX_LEN];      //接收到的数据复制在该缓冲区

//void USART_Config(unsigned char Num,unsigned int bound);
void USART_Puts(USART_TypeDef* USARTx,unsigned char * str,int len);
void Usart_init(unsigned char Num,u32 bound);
unsigned char DMA_Usart_Receive(unsigned char Num);
void GenericApp_package_Deal(unsigned char Num);
void deal_receive(unsigned char *t);
void UART4_init(u32 pclk2, u32 bound);
//void send_3gData(unsigned char *ptmp,unsigned char len);
#endif
