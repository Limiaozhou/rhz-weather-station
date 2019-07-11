#ifndef YL

#include"iostm8s103F3.h"
#include "co2.h"
#include "delay.h"
void co2_init()
{
 asm("sim");//��ȫ���ж�
   PC_DDR &=~(0X01<<4);//PC4��Ϊ����ģʽ
   PC_CR1 &=~(0X01<<4);//���ø�������
   PC_CR2 |=0X01<<4;//ʹ��PC4�ⲿ�ж�

   ITC_SPR2 &= ~(0x03<<6);//����ж����ȼ��Ĵ�����0
   ITC_SPR2 |= (0x03<<6);//����������ȼ�
   
   EXTI_CR1 &= ~(0x03<<4);//�ⲿ�жϿ��ƼĴ�������
   EXTI_CR1 |= 0x01<<4;//�˿�C�������ش����ж�
   asm("rim");
}
u8 CO2_flage=0;
u16 CO2_cnt=0,CO2_cnt_save=0;
u32 CO2temp_s=0;
void Get_CO2(float *tempfs)
{
  float data;
  if(CO2_flage==0)
  {
    
      data=CO2_cnt_save*2.04*3.992;//��λms  CO2 ��λPPM
      CO2_cnt_save=0;
      
      if(data<4200 &&data>380)
        *tempfs=data;
  }
 /* if(0!= *tempfs)
  {
    *tempfs +=0.005;
  }	*/
  
}

#pragma vector= EXTIC_vector
 __interrupt void  EXTI_PORTC_IRQHandler(void)
{
  if((PC_IDR&(0X01<<4))&&(CO2_flage==0))  //��������û��ʼ��ʱ
   {
     CO2_flage=1;  //��ʼ��ʱ�ߵ�ƽʱ��
     EXTI_CR1 |= 0x02<<4;//�˿�C���½��ش����ж�
   } 
   if(((PC_IDR&(0X01<<4))==0)&&(CO2_flage==1))  //�½������ѿ�ʼ��ʱ
   {
     CO2_flage=0;  //ֹͣ��ʱ
     if(CO2_cnt&&(CO2_cnt<491)&&(CO2_cnt>=49))  //��ʱ����2.048ms��100.352--1003.52ms
     {
     CO2_cnt_save=CO2_cnt;  //��¼����ߵ�ƽʱ��
//     CO2_cnt=0;  //��0��ʱ���Դ�ͷ��ʱ
     }
     CO2_cnt=0;  //��0��ʱ���Դ�ͷ��ʱ
     EXTI_CR1 |= 0x01<<4;//�˿�C�������ش����ж�
   } 
}
#endif
