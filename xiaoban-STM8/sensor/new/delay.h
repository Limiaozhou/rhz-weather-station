#ifndef _DELAY_H_
#define _DELAY_H_
//#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 
//��ʱ����  ��ϵͳ
//������mfg
//�汾��V1.0
//�޸�����:2015/7/31
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 
/******************@DESCRIPTION: --��ʱ1us *************/	
extern void delay_us(int t);
/******************@DESCRIPTION: --��ʱ1ms *************/	
extern void delay_ms(int t);
void delay_s(u16 ns);

#endif
