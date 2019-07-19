#include"iostm8s103F3.h"
#include "YL.h"
#if 0
char rain_flag,rain_count=0;
 void exti_init(void)
 {
  asm("sim");//关全局中断
   PC_DDR &=~(0X01<<3);//PC3设为输入模式
   PC_CR1 &=~(0X01<<3);//设置浮空输入
   //PC_CR1 |=0X01<<3;//设置上拉输入
   PC_CR2 |=0X01<<3;//使能PC3外部中断

 

   ITC_SPR2 &= ~(0x03<<6);//软件中断优先级寄存器清0
   ITC_SPR2 |= 0x01<<6;//16号中断向量优先级设为1 pc6
   
   EXTI_CR1 &= ~(0x03<<4);//外部中断控制寄存器清零
   //EXTI_CR1 |= 0x02<<4;//端口C仅下降沿触发中断
   EXTI_CR1 |= 0x01<<4;//端口C仅上升沿触发中断
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
            *temprain = rain_count*0.2;//由于没漏斗每翻动一次是0.2mm
            if(rain_count != 0.0)
            {
            *temprain +=0.005;//加上0.005提高数据准确度
            }
            rain_flag=0;
            rain_count = 0;
	}
}

#endif