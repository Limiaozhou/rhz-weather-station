#ifndef _UART_H_
#define _UART_H_
#include"typedef.h"

#define UART1_R_OR_vector              0x14
#define TIM4_OVR_UIF_vector            0x19  //定时器用的中断向量
//初始化
//extern void UART_Init(void);
extern void UART_Init(int baud);
//发送
extern void UART_Send(unsigned char *pbuf, unsigned char len);
//接收数据包
//extern char UART_Receive(unsigned char *pbuf);
//extern void Deal();
//extern void UART_Receive();
extern void UART_Clear(void);
extern short int uart1_clear_cunt;
extern char pos;
#endif