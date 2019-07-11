#include "co2.h"
#include "led.h"
#include "usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "delay.h"
#include "sys.h"
#include "hcho.h"

u16	TIM3CH4_CAPTURE_VAL;	//输入捕获值	IN
u16	TIM4CH1_CAPTURE_VAL;	//输入捕获值	OUT

void TIM4_Cap_Init(u16 arr,u16 psc)
{		 
	GPIOA->CRL&=0XFFF0FFFF;	//PB1 清除之前设置  
	GPIOA->CRL|=0X00080000;	//PB1 输入   
	RCC->APB1ENR|=1<<2;   	//TIM3 时钟使能 
	RCC->APB2ENR|=1<<2;    	//使能PORTB时钟  
	 
 	TIM4->ARR=arr;  		//设定计数器自动重装值   
	TIM4->PSC=psc;  		//预分频器 
	TIM4->CR1|=0x01;    	//使能定时器2
//	MY_NVIC_Init(2,1,TIM4_IRQn,2);//抢占2，子优先级0，组2	   
}
//外部中断配置函数
//只针对GPIOA~G;不包括PVD,RTC和USB唤醒这三个
//参数:
//GPIOx:0~6,代表GPIOA~G
//BITx:需要使能的位;
//TRIM:触发模式,1,下升沿;2,上降沿;3，任意电平触发
//该函数一次只能配置1个IO口,多个IO口,需多次调用
//该函数会自动开启对应中断,以及屏蔽线   	    
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{
	u8 EXTADDR;
	u8 EXTOFFSET;
	EXTADDR=BITx/4;//得到中断寄存器组的编号
	EXTOFFSET=(BITx%4)*4; 
	RCC->APB2ENR|=0x01;//使能io复用时钟			 
	AFIO->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET);//清除原来设置！！！
	AFIO->EXTICR[EXTADDR]|=GPIOx<<EXTOFFSET;//EXTI.BITx映射到GPIOx.BITx 
	//自动设置
	EXTI->IMR|=1<<BITx;//  开启line BITx上的中断
	//EXTI->EMR|=1<<BITx;//不屏蔽line BITx上的事件 (如果不屏蔽这句,在硬件上是可以的,但是在软件仿真的时候无法进入中断!)
 	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;//line BITx上事件下降沿触发
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;//line BITx上事件上升降沿触发
} 	  
#define FTIR   1  //下降沿触发
#define RTIR   2  //上升沿触发

void EXTIX_Init(void)
{
	TIM4_Cap_Init(0XFFFF,7199);

	Ex_NVIC_Config(1,4,FTIR); 			//上升沿触发
	MY_NVIC_Init(2,2,EXTI4_IRQn,2);	//抢占2，子优先级0，组2	
}
void EXTI4_IRQHandler(void)
{
//	delay_ms(10);//消抖
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		if(EXTI->FTSR&0x10)
		{
			TIM4->CR1&=~0x01;
			EXTI->RTSR|=1<<4;
			EXTI->FTSR&=~(1<<4);
			TIM4CH1_CAPTURE_VAL=TIM4->CNT;
		}
		else
		{
			EXTI->FTSR|=1<<4;
			EXTI->RTSR&=~(1<<4);
			TIM4->CR1|=0x01;
			TIM4->CNT=0;
		}
	}
	EXTI->PR=1<<4;  //清除LINE0上的中断标志位  
}
unsigned char collect_flag;//测试发现，未接CO2时，可能受APCEG寄存器的影响，会一会进IO中断直接导致识别为PWM输出传感器
void Get_co2(float *co2)
{
  if(!TIM4CH1_CAPTURE_VAL)
  {
    float tmp = Get_Adc_Average(4,5);
//	ADC1->SMPR2&=~(7<<12);  //通道1采样时间清空	  
 //	ADC1->SMPR2|=7<<12;     //通道1  239.5周期,提高采样时间可以提高精确度	 
    *co2=1.61*tmp;
//    if(tmp<=2048&&tmp>100)
//    {
 //     *co2 = (float)(((tmp/2047) * 3.3)*3125-1250);
//    }
    if(*co2<0) *co2=0;
    collect_flag=0;
  }
  else
  {
    collect_flag++;
    if(collect_flag>=2)
    {
      if(TIM4CH1_CAPTURE_VAL<2000)
        *co2=400;
      else if(TIM4CH1_CAPTURE_VAL>19570)
        *co2=4000;
      else
        *co2 = TIM4CH1_CAPTURE_VAL*0.2048-8;
    }
    TIM4CH1_CAPTURE_VAL=0;
  }
	if(*co2>5000) *co2=0;
}
