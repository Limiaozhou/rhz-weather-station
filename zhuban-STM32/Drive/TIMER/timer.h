#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#include "multi_timer.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
 #define CLI() __set_PRIMASK(1) //�ر��ж�
 #define SEI() __set_PRIMASK(0)  //�����ж�

extern char hmi_send_flag;  //��Ļ���ͱ�־λ
extern char relay_send_flag;  //�̵����巢�ͱ�־λ

void TIM4_Int_Init(u16 arr,u16 psc);
#endif























