#include"iostm8s103F3.h"
#include "co2.h"
#include "delay.h"
void co2_init(u16 arr ,u16 psc)
{
  CLK_PCKENR1 |=0x01<<7;//ʹ�ܶ�ʱ��1ʱ��
  TIM1_CR1   =0x00;
  TIM1_CR2   =0x00;
  TIM1_SMCR  =0x00;
  TIM1_IER   =0x00; //�ж�ʹ�ܼĴ���
  TIM1_CCER2 =0x00;
  TIM1_CCMR1 =0x00;
  PC_DDR &= ~(0x01<<4);//Pc4����
  PC_CR1 &= ~(0x01<<4);//��������
  PC_CR2 &= ~(0x01<<4);//��ֹ�ⲿ�ж�
  //��ʱ������Fmasterʱ��  Ƶ��Ϊ16M
 

  TIM1_CCMR4 =0x00;
  TIM1_CCMR4 =0x02;//CC4ͨ��������Ϊ���룬IC4ӳ����TI4FP4�ϣ�
   
   TIM1_CNTRH =arr>>8;
  TIM1_CNTRL =arr&0xff;

  //TIM1_ARRH =0x00;
  TIM1_ARRH =arr>>8;//��װ�ؼĴ�����λ
  //TIM1_ARRL =0x00;
  TIM1_ARRL =arr&0xff;//��װ�ؼĴ�����λ

  TIM1_PSCRH |=psc>>8;//Ԥ��Ƶֵ
  TIM1_PSCRL =psc&0xff;//
  
  TIM1_CCMR1 |=0x02<4;//�˲���
   
  TIM1_SMCR=00;
  TIM1_SMCR=0x04;//��λģʽ

  TIM1_IER |=0x01<<4;//������/�Ƚ�4�ж�
  TIM1_IER |=0x01;//��������ж�

  TIM1_EGR =0x00;
  TIM1_CR1  |=0x81;//ʹ�ܼ�����
  TIM1_EGR |=0x01;//����һ�θ����¼�
  TIM1_CCER2 |=0x01<<4;//��ʱ��ͨ��4����Ƚ�ʹ��
}

u8  TIM2CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u16 TIM2CH2_CAPTURE_VAL;	//���벶��ֵ



u32 CO2temp_s;
void Get_CO2(float *tempfs)
{
  delay_ms(10);
  //	TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);
  //if(TIM_GetCapture2(TIM3)==300)TIM_SetCompare2(TIM3,0);	
  if(TIM2CH2_CAPTURE_STA&0X80)//�ɹ�������һ���½���
  {
      CO2temp_s=TIM2CH2_CAPTURE_STA&0X3F;
      CO2temp_s*=65536;//���ʱ���ܺ�
      CO2temp_s+=TIM2CH2_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
      //if(temp_s==0)
      //tempfs=0.1;
      //else
      *tempfs=(float)CO2temp_s/250-8;
      if(0 != *tempfs)
      {
              *tempfs +=0.005;
      }
      TIM2CH2_CAPTURE_STA=0;//������һ�β���״̬��־λ����
  }
          //delay_ms(250);	
}
#pragma vector= TIM1_OVR_UIF_vector 
__interrupt void TIM1_OVR_UIF_IRQHandler(void)
{  
   if(TIM1_SR1&0x01)//���������¼��ж�
  {	   
      if(TIM2CH2_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
      {
          if((TIM2CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
          {
                  TIM2CH2_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
                  TIM2CH2_CAPTURE_VAL=0XFFFF;
          }
          else TIM2CH2_CAPTURE_STA++;
      }	 
  }
  TIM1_SR1 &=~0x01;//��������жϱ�־
}

#pragma vector= TIM1_CAPCOM_CC4IF_vector //0x0e
__interrupt void TIM1_CAPCOM_CC4IF_IRQHandler(void)
{  
  if((TIM2CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  

              if(TIM1_SR1 & (0x01<<4))//���������ж�
              {
              
		if(TIM2CH2_CAPTURE_STA&0X40)		//����һ���½���
		{	  			
                    TIM2CH2_CAPTURE_STA|=0X80;		//��ǳɹ�����һ���½���
                
                    TIM2CH2_CAPTURE_VAL = TIM1_CCR4H;
                    TIM2CH2_CAPTURE_VAL = TIM2CH2_CAPTURE_VAL<<8+TIM1_CCR4L;
                 
                      
                     TIM1_CCER2 &= ~(0x01<<5);//���ò�������������
		}
		else  								//��δ��ʼ,��һ�β���������
		{
                    TIM2CH2_CAPTURE_STA =0;	//���
                    TIM2CH2_CAPTURE_VAL =0;
                    //TIM_SetCounter(TIM4,0);//���üĴ�����ֵ
                    TIM1_CNTRH =0;//�����Ĵ�����0
                    TIM1_CNTRL =0;
                    TIM2CH2_CAPTURE_STA|=0X40;  //��ǲ�����������
                    //TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز��� 
                    TIM1_CCER2 |=0x1<<5;//����Ϊ�½��ز���
		}		    
	     }	   
    }	
  
   TIM1_SR1 &= ~(0x01<<4);
     //TIM_ClearITPendingBit(TIM4,TIM_IT_CC4|TIM_IT_Update); //����жϱ�־λ
  return;
}
