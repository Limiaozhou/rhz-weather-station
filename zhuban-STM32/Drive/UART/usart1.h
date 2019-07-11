#ifndef __USART3_H
#define __USART3_H
#include "includes.h"	 
////////////////////////////////////////////////////////////////////////////////// 
//UART3初始化、读写程序
//创建：mfg
//版本：V1.0
//修改日期:2015/8/03
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
////////////////////////////////////////////////////////////////////////////////// 

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern uchar  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 

/******************@DESCRIPTION: -- 初始化串口3 *************/	
extern void UART3_init(uint pclk2,uint bound);

/******************@DESCRIPTION: -- 1字节数据发送 *************/	
extern void UART3_SendByte(uchar data);

/******************@DESCRIPTION: --多个字节数据发送 *************/	
extern void UART3_SendStr(uchar *pdat, uchar len);

#endif	   
















