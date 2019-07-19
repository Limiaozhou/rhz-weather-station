#include"iostm8s103F3.h"
#include "co2.h"
#include "delay.h"
void co2_init(u16 arr ,u16 psc)
{
  TIM2_CR1  =0x00;
  TIM2_CR1  |=0x01;//ʹ�ܼ�����
  
  PD_DDR &= ~0x08;//Pd3����
  PD_CR1 &= ~0x08;//��������
  PD_CR2 &= ~0x08;//��ֹ�ⲿ�ж�
  //��ʱ������Fmasterʱ��  Ƶ��Ϊ16M
 

   
  TIM2_CNTRL =arr&0xff;
  TIM2_CNTRH =arr>>8;
 
  TIM2_ARRL =0x00;
  TIM2_ARRL =arr&0xff;//��װ�ؼĴ�����λ
   TIM2_ARRH =0x00;
  TIM2_ARRH =arr>>8;//��װ�ؼĴ�����λ
  
  TIM2_PSCR &=0xf0;//��0
  TIM2_PSCR |=psc&0x0f;//Ԥ��Ƶֵ
  
  //TIM2_CR1  |=0x80;//Ԥװ��ֵ���Ի���
  
  TIM2_CCMR2 =0x00;
  TIM2_CCMR2 |=0x01; //CC1ͨ��������Ϊ���룬IC2ӳ����TI2FP2�ϣ�
  
  TIM2_CCER1 &=~(0x01<<5);//��������������
  
  TIM2_IER =0x00;//
  TIM2_IER |=(0x01<<2);//CC2IE��������/�Ƚ�2�ж�
  TIM2_IER |=0x01;//UIE����������ж�
  //TIM2_IER |=(0x01<<6);//�����ж�ʹ��
  
  // TIM4_EGR=0x01;//����һ�θ����¼�
  TIM2_CCER1 |=0x10;//CC2E������/����2���ʹ�ܡ�
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
#pragma vector= TIM2_OVR_UIF_vector //0x10
__interrupt void TIM2_OVR_UIF_IRQHandler(void)
{  
  ;
}

#pragma vector= TIM2_CAPCOM_CC2IF_vector //0x10
__interrupt void TIM2_CAPCOM_CC2IF_IRQHandler(void)
{  
  if((TIM2CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{	  
		//if (TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)//�����ж���Ӧ���жϱ�־λ �����ж�
                if(TIM2_SR1&0x01)//���������¼��ж�
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
	//if (TIM_GetITStatus(TIM4,TIM_IT_CC4) != RESET)//����1���������¼�
              if(TIM2_SR1 & (0x01<<3))//���������ж�
              {
              
		if(TIM2CH2_CAPTURE_STA&0X40)		//����һ���½���
		{	  			
                    TIM2CH2_CAPTURE_STA|=0X80;		//��ǳɹ�����һ���½���
                    //TIM4CH4_CAPTURE_VAL=TIM_GetCapture4(TIM4);
                    TIM2CH2_CAPTURE_VAL = TIM2_CCR2H;
                    TIM2CH2_CAPTURE_VAL = TIM2CH2_CAPTURE_VAL<<8+TIM2_CCR2L;
                    //TIM4CH4_CAPTURE_VAL = TIM2_CCR2H<<8+TIM2_CCR2L;//�����������һ����˼
                    
                    //TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���������
                     TIM2_CCER1 &= ~(0x01<<5);//���ò�������������
		}
		else  								//��δ��ʼ,��һ�β���������
		{
                    TIM2CH2_CAPTURE_STA =0;			//���
                    TIM2CH2_CAPTURE_VAL =0;
                    //TIM_SetCounter(TIM4,0);//���üĴ�����ֵ
                    TIM2_CNTRH =0;//�����Ĵ�����0
                    TIM2_CNTRL =0;
                    TIM2CH2_CAPTURE_STA|=0X40;  //��ǲ�����������
                    //TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز��� 
                    TIM2_CCER1 |=0x1<<5;//����Ϊ�½��ز���
		}		    
	     }	   
    }	
     TIM2_SR1 &=~0x01;//����жϱ�־
     TIM2_SR1 &= ~0x04;//����жϱ�־
     //TIM_ClearITPendingBit(TIM4,TIM_IT_CC4|TIM_IT_Update); //����жϱ�־λ
  return;
}
