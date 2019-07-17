#include "timer.h"
#include "led.h"
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

//��ʱ��3�жϷ������	 
#include "gprs.h"
#include "delay.h"
#include "data.h"
#include "wdg.h"
#include "led.h"
#include "usart.h"
#include "sys.h"
#include "air_cond_swi.h"
//extern ST_EVNBALEDAT *psenddat;
//extern void packet_send(unsigned char *pdat);
char Ana_Flag=0;

int send_cnt=0;//7200
int send_time=50;       //Ĭ�Ϸ������ڣ�5��
char send_flag=0;       //���ݷ��ͱ�־λ

int send485_cnt=10;
char send485_flag=0;     

char wat_start_flag=0;    //��ˮˮ��1��ʼ��־
int wat_cnt = 0;
char wat_flag =0;        //��ˮʱ�䵽10s��־

//char fer_start_flag =0 ;
//int fer_cnt =0;
//char fer_flag = 0;

char magnetic_start_flag = 0;
int magnetic_cnt =0;
char magnetic_flag =0;

char blender_start_flag =0;
int blender_cnt =0;
char blender_flag = 0;



void TIM4_IRQHandler(void)
{ 		    		  			    
  if(TIM4->SR&0X0001)//����ж�(200ms)
  {
    Ana_Flag=1;
    
    if(send485_cnt++>=10)//2���ӷ���һ��485����
    {
      send485_flag=1;
      send485_cnt=0;
    }
    
    if(send_cnt++>=send_time)
    {
      send_flag=1;
      send_cnt=0;
    }
    
     if(wat_start_flag)
    {
      if(wat_cnt++>=500)     //100s
      {
          wat_flag=1;
          wat_cnt =0;
       }
    }
    else
    {
      wat_cnt = 0;
    }
    
//     if(fer_start_flag)
//    {
//      if(fer_cnt++>=3000)
//      {
//          fer_flag=1;
//        
//          fer_cnt =0;
//       }
//    }
    
     if(magnetic_start_flag)
     {
      if(magnetic_cnt++>=50)
      {
          magnetic_flag=1;
          magnetic_cnt =0;
       }
     }
    else
    {
      magnetic_cnt = 0;
    }
    
   
     if(blender_start_flag)
    {
      if(blender_cnt++>=150)
      {
          blender_flag=1; 
          blender_cnt =0;
       }
    }
    else
    {
      blender_cnt = 0;
    }
    
    
  }				   
  TIM4->SR&=~(1<<0);//����жϱ�־λ 	    time3_cnt++;
}
//ͨ�ö�ʱ��4�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��4
void TIM4_Int_Init(u16 arr,u16 psc)
{
  RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ��
  TIM4->ARR=arr;  	//�趨�������Զ���װֵ��2000 / 10 = 200ms
  TIM4->PSC=psc;  	//Ԥ��Ƶ��3600��36M/3600 = 0.01M = 10K
  TIM4->DIER|=1<<0;   //��������ж�
  TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  MY_NVIC_Init(1,3,TIM4_IRQn,2);//��ռ1�������ȼ�3����2
}














