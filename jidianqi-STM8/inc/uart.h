#ifndef _UART_H_
#define _UART_H_
#include"typedef.h"

#define UART1_R_OR_vector              0x14
#define TIM4_OVR_UIF_vector            0x19  //��ʱ���õ��ж�����
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
#endif