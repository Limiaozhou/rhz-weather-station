#ifndef __USART3_H
#define __USART3_H
#include "includes.h"	 
////////////////////////////////////////////////////////////////////////////////// 
//UART3��ʼ������д����
//������mfg
//�汾��V1.0
//�޸�����:2015/8/03
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern uchar  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 

/******************@DESCRIPTION: -- ��ʼ������3 *************/	
extern void UART3_init(uint pclk2,uint bound);

/******************@DESCRIPTION: -- 1�ֽ����ݷ��� *************/	
extern void UART3_SendByte(uchar data);

/******************@DESCRIPTION: --����ֽ����ݷ��� *************/	
extern void UART3_SendStr(uchar *pdat, uchar len);

#endif	   
















