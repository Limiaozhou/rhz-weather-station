#ifndef _UART_H_
#define _UART_H_
#include"typedef.h"

extern unsigned char read_pm25[5];
extern unsigned char open_pump[6];

//��ʼ��
extern void UART_Init(void);
//����
extern void UART_Send(unsigned char *pbuf, unsigned char len);
//�������ݰ�
extern char UART_Receive(unsigned char *pbuf);
extern void Deal();
#endif