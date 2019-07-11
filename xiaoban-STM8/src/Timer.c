#include"iostm8s103F3.h"
#include "data.h"
#include "co2.h"
#include "uart.h"
/**********
*��ʱ��4��������Ϊ0.00204S
*�� 2ms
********/
#include "iwdg.h"
void Init_Timer4(void)  //���¼�����ARR��0
{
  CLK_PCKENR1 |=0x01<<4;//ʹ�ܶ�ʱ��4��ʱ��
    TIM4_CR1=0x01;//ʹ�ܼ�����
    TIM4_IER=0x00;
    TIM4_IER=0x01;//�����ж�ʹ��
    TIM4_EGR=0x01;  //���������³�ʼ������������
    TIM4_CNTR=255;//������ֵ
    TIM4_ARR=255;//�Զ���װ��ֵ��T = 256 * 8 us = 2048us = 2.048ms
    TIM4_PSCR=0x07;//��Ƶֵ��16M / 2^7 =  1/8 MHz

}
    
#pragma vector=TIM4_OVR_UIF_vector//0x19
__interrupt void TIM4_OVR_UIF_IRQHandler(void)//��ӦIAP���жϵ�ַ��0x8060��2.048ms�ж�һ��
{
//  UART_Send("UVWXY", 5);
  collect_time++;  //��ȡ���������ݲ����ͼ�ʱ
  IWDG_Feed();  //ˢ��IDDG������������Ź���λ
   if(CO2_flage==1)  //CO2��ȡPWM��־λ
  {
    CO2_cnt++;
  }  
  if(uart1_clear_cunt++ > 295)//0.604s�������ݽ��ܽ����buf
  {
    uart1_clear_cunt=0;
    if(pos!=0)
    UART_Clear();
  }
  TIM4_SR=0x00;//�ð��������־λ     
}