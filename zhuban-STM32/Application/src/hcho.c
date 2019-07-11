#include "hcho.h"
#include "SYS.H"
//��ȩ
//unsigned int hcho= 0;
#define BASE_VO 1.235
void  Hcho_Adc_Init(void)
{    
	//�ȳ�ʼ��IO��
 	RCC->APB2ENR|=1<<2;    //ʹ��PORTA��ʱ�� 
	GPIOA->CRL&=0XF0FFFFFF;//PA1 anolog����
	//ͨ��10/11����			 
	RCC->APB2ENR|=1<<9;    //ADC1ʱ��ʹ��	  
	RCC->APB2RSTR|=1<<9;   //ADC1��λ
	RCC->APB2RSTR&=~(1<<9);//��λ����	    
	RCC->CFGR&=~(3<<14);   //��Ƶ��������	
	//SYSCLK/DIV2=12M ADCʱ������Ϊ12M,ADC���ʱ�Ӳ��ܳ���14M!
	//���򽫵���ADC׼ȷ���½�! 
	RCC->CFGR|=2<<14;      	 
	ADC1->CR1&=0XF0FFFF;   //����ģʽ����
	ADC1->CR1|=0<<16;      //��������ģʽ  
	ADC1->CR1&=~(1<<8);    //��ɨ��ģʽ	  
	ADC1->CR2&=~(1<<1);    //����ת��ģʽ
	ADC1->CR2&=~(7<<17);	   
	ADC1->CR2|=7<<17;	   //�������ת��  
	ADC1->CR2|=1<<20;      //ʹ�����ⲿ����(SWSTART)!!!	����ʹ��һ���¼�������
	ADC1->CR2&=~(1<<11);   //�Ҷ���	 
	ADC1->SQR1&=~(0XF<<20);
	ADC1->SQR1|=0<<20;     //1��ת���ڹ��������� Ҳ����ֻת����������1 			   
	//����ͨ��1�Ĳ���ʱ��
	ADC1->SMPR2&=~(7<<18);  //ͨ��1����ʱ�����	  
 	ADC1->SMPR2|=7<<18;     //ͨ��1  239.5����,��߲���ʱ�������߾�ȷ��	 
	ADC1->SMPR2&=~(7<<21);  //ͨ��1����ʱ�����	  
 	ADC1->SMPR2|=7<<21;     //ͨ��1  239.5����,��߲���ʱ�������߾�ȷ��	 
	ADC1->SMPR2&=~(7<<12);  //ͨ��1����ʱ�����	  
 	ADC1->SMPR2|=7<<12;     //ͨ��1  239.5����,��߲���ʱ�������߾�ȷ��	 
	ADC1->CR2|=1<<0;	   //����ADת����	 
	ADC1->CR2|=1<<3;       //ʹ�ܸ�λУ׼  
	while(ADC1->CR2&1<<3); //�ȴ�У׼���� 			 
    //��λ��������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ��������� 		 
	ADC1->CR2|=1<<2;        //����ADУ׼	   
	while(ADC1->CR2&1<<2);  //�ȴ�У׼����
	//��λ����������Կ�ʼУ׼������У׼����ʱ��Ӳ�����  
}		
u16 Get_Adc(u8 ch)   
{
	//����ת������	  		 
	ADC1->SQR3&=0XFFFFFFE0;//��������1 ͨ��ch
	ADC1->SQR3|=ch;		  			    
	ADC1->CR2|=1<<22;       //��������ת��ͨ�� 
	while(!(ADC1->SR&1<<1));//�ȴ�ת������	 	   
	return ADC1->DR;		//����adcֵ	
}
float Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
//		delay_ms(5);
	}
	return (float)temp_val/times;
} 
	 
void Get_hcho(float *hcho)
{
  float dat;
//  float tmp_data;
  float tmp=0;
//  for(i=0;i<30;i++)
//    tmp += HalAdcRead(6, HAL_ADC_RESOLUTION_14);
 // tmp=tmp/30;
  tmp=Get_Adc_Average(6,10);
  dat = (float)(((tmp/4096) * 3.3/0.66))-BASE_VO; //0-3.765V
//  if(tmp>200)
//  {
    if(dat>0)
      *hcho = (float)dat/((5-BASE_VO));
    else
    {
      *hcho = 0;    
    }
 // }
 // else tmp_data = 0;
//  data[0] = (int)tmp_data;
//  tmp = tmp_data - data[0];
//  data[1] = (int)(tmp*100);
} 

