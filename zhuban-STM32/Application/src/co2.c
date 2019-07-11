#include "co2.h"
#include "led.h"
#include "usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "delay.h"
#include "sys.h"
#include "hcho.h"

u16	TIM3CH4_CAPTURE_VAL;	//���벶��ֵ	IN
u16	TIM4CH1_CAPTURE_VAL;	//���벶��ֵ	OUT

void TIM4_Cap_Init(u16 arr,u16 psc)
{		 
	GPIOA->CRL&=0XFFF0FFFF;	//PB1 ���֮ǰ����  
	GPIOA->CRL|=0X00080000;	//PB1 ����   
	RCC->APB1ENR|=1<<2;   	//TIM3 ʱ��ʹ�� 
	RCC->APB2ENR|=1<<2;    	//ʹ��PORTBʱ��  
	 
 	TIM4->ARR=arr;  		//�趨�������Զ���װֵ   
	TIM4->PSC=psc;  		//Ԥ��Ƶ�� 
	TIM4->CR1|=0x01;    	//ʹ�ܶ�ʱ��2
//	MY_NVIC_Init(2,1,TIM4_IRQn,2);//��ռ2�������ȼ�0����2	   
}
//�ⲿ�ж����ú���
//ֻ���GPIOA~G;������PVD,RTC��USB����������
//����:
//GPIOx:0~6,����GPIOA~G
//BITx:��Ҫʹ�ܵ�λ;
//TRIM:����ģʽ,1,������;2,�Ͻ���;3�������ƽ����
//�ú���һ��ֻ������1��IO��,���IO��,���ε���
//�ú������Զ�������Ӧ�ж�,�Լ�������   	    
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{
	u8 EXTADDR;
	u8 EXTOFFSET;
	EXTADDR=BITx/4;//�õ��жϼĴ�����ı��
	EXTOFFSET=(BITx%4)*4; 
	RCC->APB2ENR|=0x01;//ʹ��io����ʱ��			 
	AFIO->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET);//���ԭ�����ã�����
	AFIO->EXTICR[EXTADDR]|=GPIOx<<EXTOFFSET;//EXTI.BITxӳ�䵽GPIOx.BITx 
	//�Զ�����
	EXTI->IMR|=1<<BITx;//  ����line BITx�ϵ��ж�
	//EXTI->EMR|=1<<BITx;//������line BITx�ϵ��¼� (������������,��Ӳ�����ǿ��Ե�,��������������ʱ���޷������ж�!)
 	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;//line BITx���¼��½��ش���
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;//line BITx���¼��������ش���
} 	  
#define FTIR   1  //�½��ش���
#define RTIR   2  //�����ش���

void EXTIX_Init(void)
{
	TIM4_Cap_Init(0XFFFF,7199);

	Ex_NVIC_Config(1,4,FTIR); 			//�����ش���
	MY_NVIC_Init(2,2,EXTI4_IRQn,2);	//��ռ2�������ȼ�0����2	
}
void EXTI4_IRQHandler(void)
{
//	delay_ms(10);//����
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
	EXTI->PR=1<<4;  //���LINE0�ϵ��жϱ�־λ  
}
unsigned char collect_flag;//���Է��֣�δ��CO2ʱ��������APCEG�Ĵ�����Ӱ�죬��һ���IO�ж�ֱ�ӵ���ʶ��ΪPWM���������
void Get_co2(float *co2)
{
  if(!TIM4CH1_CAPTURE_VAL)
  {
    float tmp = Get_Adc_Average(4,5);
//	ADC1->SMPR2&=~(7<<12);  //ͨ��1����ʱ�����	  
 //	ADC1->SMPR2|=7<<12;     //ͨ��1  239.5����,��߲���ʱ�������߾�ȷ��	 
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
