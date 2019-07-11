#ifndef _UART_H_
#define _UART_H_
#include"typedef.h"

extern unsigned char read_pm25[5];
extern unsigned char open_pump[6];

//初始化
extern void UART_Init(void);
//发送
extern void UART_Send(unsigned char *pbuf, unsigned char len);
//接收数据包
extern char UART_Receive(unsigned char *pbuf);
extern void Deal();
#endif