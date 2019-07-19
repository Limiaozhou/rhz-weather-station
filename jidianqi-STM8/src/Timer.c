#include"iostm8s103F3.h"
#include "data.h"
//#include "co2.h"
#include "uart.h"
#include "control.h"


#define bit0 0x01 
#define bit1 0x02 
#define bit2 0x04 
#define bit3 0x08 
#define bit4 0x10 
#define bit5 0x20 
#define bit6 0x40 
#define bit7 0x80

void Init_Timer2(void)
{
  CLK_PCKENR1 |=0x01<<1;//ʹ�ܶ�ʱ��4��ʱ��
  TIM2_PSCR=0x00;  //HSI,16M ,1/16us��Ҳ����˵��������ÿ��1/16us����һ�Ρ�
 // TIM2_ARRH=0x3E;       //�Զ���װ��ֵ 0x3e7e =16000;Fpwm=16000000/16000=10000HZ
//  TIM2_ARRL=0x80; 
  TIM2_ARRH=0x06;       //�Զ���װ��ֵ 0x3e7e =1600;Fpwm=16000000/1600=10000HZ
  TIM2_ARRL=0x40;
  TIM2_CCER1=bit0|bit1; //���������½���,OC1
  TIM2_CCMR1=bit3|bit5|bit6;  //cc1ͨ��������Ϊ�������ֹTIMx_CCR1�Ĵ�����Ԥװ�ع��ܣ�
  //����ʱд��CCR1�Ĵ�����д�����ֵ���������á�PW1ģʽ��CNT<CCR,CH1������
  TIM2_CCR1H=0x06; //����/�ȽϼĴ���������Ĵ���������PWM��ռ�ձȡ�CCR/ARR=PWM��ռ�ձ�
  TIM2_CCR1L=0x40;  //��������Ϊ0x1f40,��ôռ�ձ�Ϊ0x1f40/0x3E80=8000/16000;
 // TIM2_IER=0x00;        //�����ж�ʹ�� 
 // TIM2_CR1=0x01;        //enable counter 
}

/**********
*��ʱ��4��������Ϊ0.00204S
*�� 2ms
********/
#include "iwdg.h"
void Init_Timer4(void)
{
  CLK_PCKENR1 |=0x01<<4;//ʹ�ܶ�ʱ��4��ʱ��
    TIM4_CR1=0x01;//ʹ�ܼ�����
    TIM4_IER=0x00;
    TIM4_IER=0x01;//�����ж�ʹ��
    TIM4_EGR=0x01;
    TIM4_CNTR=255;//������ֵ
    TIM4_ARR=255;//�Զ���װ��ֵ
//    TIM2_ARRH=0x7A;       //�Զ���װ��ֵ 0x3e7e =1600;Fpwm=16000000/1600=10000HZ
//  TIM2_ARRL=0x12;
    TIM4_PSCR=0x07;//��Ƶֵ 128

}
int time1 = 0;
int time2 = 0;
int time3 = 0;
//char water12 = 0;
#pragma vector=TIM4_OVR_UIF_vector//0x19
__interrupt void TIM4_OVR_UIF_IRQHandler(void)//��ӦIAP���жϵ�ַ��0x8060 1s�ж�490
{
  collect_time++;
  
//  IWDG_Feed();
   if(WAT.water1 > 0)
   {
     time1 ++;
      time3 = 0;
     if(time1 > 490*60)
     {
       time1 = 0;
       time2++;
       if(time2 >= WAT.water1time)
       {
         time2 = 0;
          WAT.water1 = 0;
          read485 = 1;
       }
     }
   }
   else if(WAT.water2 > 0)
   {
     time1 ++;
     time2 = 0;
     if(time1 > 490*60)
     {
       time1 = 0;
       time3++;
       if(time3 >= WAT.water2time)
       {
         time3 = 0;
          WAT.water2 = 0;
          read485 = 1;
       }
     }
   }
   else
   {
     time1 = 0;
     time2 = 0;
     time3 = 0;
   }
  if(uart1_clear_cunt++ > 295)//0.6s�������ݽ��ܽ����buf
  {
    uart1_clear_cunt=0;
    if(pos!=0)
    UART_Clear();
  }
  TIM4_SR=0x00;//�ð��������־λ     
}