#include"iostm8s103F3.h"
#include "YL.h"
#if 0
char rain_flag,rain_count=0;
 void exti_init(void)
 {
  asm("sim");//��ȫ���ж�
   PC_DDR &=~(0X01<<3);//PC3��Ϊ����ģʽ
   PC_CR1 &=~(0X01<<3);//���ø�������
   //PC_CR1 |=0X01<<3;//������������
   PC_CR2 |=0X01<<3;//ʹ��PC3�ⲿ�ж�

 

   ITC_SPR2 &= ~(0x03<<6);//����ж����ȼ��Ĵ�����0
   ITC_SPR2 |= 0x01<<6;//16���ж��������ȼ���Ϊ1 pc6
   
   EXTI_CR1 &= ~(0x03<<4);//�ⲿ�жϿ��ƼĴ�������
   //EXTI_CR1 |= 0x02<<4;//�˿�C���½��ش����ж�
   EXTI_CR1 |= 0x01<<4;//�˿�C�������ش����ж�
   asm("rim");
 }
#pragma vector= EXTIC_vector
 __interrupt void  EXTI_PORTC_IRQHandler(void)
{
   delay_ms(100);
   if((PC_IDR&(0X01<<3))==0)
	rain_count++;
	//TIM4->CR1&=~0x01;
}

void Get_rain(float *temprain)
{
	if(rain_flag)
	{
            *temprain = rain_count*0.2;//����û©��ÿ����һ����0.2mm
            if(rain_count != 0.0)
            {
            *temprain +=0.005;//����0.005�������׼ȷ��
            }
            rain_flag=0;
            rain_count = 0;
	}
}

#endif