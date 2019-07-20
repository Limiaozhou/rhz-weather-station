#ifndef _UART_H_
#define _UART_H_
#include"typedef.h"

#define UART1_R_OR_vector              0x14
#define TIM4_OVR_UIF_vector            0x19  //��ʱ���õ��ж�����

#define   RXBUFSIZE     100

//��ʼ��
//extern void UART_Init(void);
extern void UART_Init(int baud);
//����
extern void UART_Send(unsigned char *pbuf, unsigned char len);
//�������ݰ�
//extern char UART_Receive(unsigned char *pbuf);
//extern void Deal();
//extern void UART_Receive();
extern void UART_Clear(void);
extern short int uart1_clear_cunt;
extern char pos;

extern char Rxbuf[RXBUFSIZE];
extern unsigned char rx_flag;

#endif
