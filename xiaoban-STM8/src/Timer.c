#include"iostm8s103F3.h"
#include "data.h"
#include "co2.h"
#include "uart.h"
/**********
*定时器4更新周期为0.00204S
*即 2ms
********/
#include "iwdg.h"
void Init_Timer4(void)  //向下计数，ARR到0
{
  CLK_PCKENR1 |=0x01<<4;//使能定时器4的时钟
    TIM4_CR1=0x01;//使能计数器
    TIM4_IER=0x00;
    TIM4_IER=0x01;//更新中断使能
    TIM4_EGR=0x01;  //计数器重新初始化并产生更新
    TIM4_CNTR=255;//计数器值
    TIM4_ARR=255;//自动重装的值，T = 256 * 8 us = 2048us = 2.048ms
    TIM4_PSCR=0x07;//分频值，16M / 2^7 =  1/8 MHz

}
    
#pragma vector=TIM4_OVR_UIF_vector//0x19
__interrupt void TIM4_OVR_UIF_IRQHandler(void)//对应IAP的中断地址：0x8060，2.048ms中断一次
{
//  UART_Send("UVWXY", 5);
  collect_time++;  //获取传感器数据并发送计时
  IWDG_Feed();  //刷新IDDG，避免产生看门狗复位
   if(CO2_flage==1)  //CO2读取PWM标志位
  {
    CO2_cnt++;
  }  
  if(uart1_clear_cunt++ > 295)//0.604s内无数据接受将清空buf
  {
    uart1_clear_cunt=0;
    if(pos!=0)
    UART_Clear();
  }
  TIM4_SR=0x00;//好霸气的请标志位     
}