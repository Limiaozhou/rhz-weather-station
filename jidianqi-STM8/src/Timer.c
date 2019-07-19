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
  CLK_PCKENR1 |=0x01<<1;//使能定时器4的时钟
  TIM2_PSCR=0x00;  //HSI,16M ,1/16us，也就是说计数器会每隔1/16us计数一次。
 // TIM2_ARRH=0x3E;       //自动重装的值 0x3e7e =16000;Fpwm=16000000/16000=10000HZ
//  TIM2_ARRL=0x80; 
  TIM2_ARRH=0x06;       //自动重装的值 0x3e7e =1600;Fpwm=16000000/1600=10000HZ
  TIM2_ARRL=0x40;
  TIM2_CCER1=bit0|bit1; //捕获发生在下降沿,OC1
  TIM2_CCMR1=bit3|bit5|bit6;  //cc1通道被配置为输出，禁止TIMx_CCR1寄存器的预装载功能，
  //可随时写入CCR1寄存器且写入的数值立即起作用。PW1模式：CNT<CCR,CH1被激活
  TIM2_CCR1H=0x06; //捕获/比较寄存器，这个寄存器决定着PWM的占空比。CCR/ARR=PWM的占空比
  TIM2_CCR1L=0x40;  //例如设置为0x1f40,那么占空比为0x1f40/0x3E80=8000/16000;
 // TIM2_IER=0x00;        //更新中断使能 
 // TIM2_CR1=0x01;        //enable counter 
}

/**********
*定时器4更新周期为0.00204S
*即 2ms
********/
#include "iwdg.h"
void Init_Timer4(void)
{
  CLK_PCKENR1 |=0x01<<4;//使能定时器4的时钟
    TIM4_CR1=0x01;//使能计数器
    TIM4_IER=0x00;
    TIM4_IER=0x01;//更新中断使能
    TIM4_EGR=0x01;
    TIM4_CNTR=255;//计数器值
    TIM4_ARR=255;//自动重装的值
//    TIM2_ARRH=0x7A;       //自动重装的值 0x3e7e =1600;Fpwm=16000000/1600=10000HZ
//  TIM2_ARRL=0x12;
    TIM4_PSCR=0x07;//分频值 128

}
int time1 = 0;
int time2 = 0;
int time3 = 0;
//char water12 = 0;
#pragma vector=TIM4_OVR_UIF_vector//0x19
__interrupt void TIM4_OVR_UIF_IRQHandler(void)//对应IAP的中断地址：0x8060 1s中断490
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
  if(uart1_clear_cunt++ > 295)//0.6s内无数据接受将清空buf
  {
    uart1_clear_cunt=0;
    if(pos!=0)
    UART_Clear();
  }
  TIM4_SR=0x00;//好霸气的请标志位     
}