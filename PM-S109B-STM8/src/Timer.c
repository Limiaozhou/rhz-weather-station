#include"iostm8s103F3.h"
#include "uart.h"
#include "timer.h"

int i=0,j=0,n=0;;
 unsigned char open_pump[6]={0x11,0x03,0x0c,0x02,0x1e,0xc0};
 /*unsigned char close_pump[6]={0x11,0x03,0x0c,0x01,0x1e,0xc1};
 unsigned char read_pm25[4]={0x11,0x01,0x0b,0xe3};*/
unsigned char read_pm25[5]={0x11,0x02,0x0b,0x01,0xE1};
void Init_Timer4(void)
{
  
    TIM4_CR1=0x01;//使能计数器
  //TIM4_IER=0x00;
    TIM4_IER=0x01;//更新中断使能
    TIM4_EGR=0x01;
    TIM4_CNTR=255;//计数器值
    TIM4_ARR=255;//自动重装的值
    TIM4_PSCR=0x07;//分频值
//    UART_Send(open_pump,6);
 //   UART_Send("123456",6);
}

//45秒发送一个open_pumb[]  每5秒发送一个read_pm25[]存储于SENDAT
    
#pragma vector=TIM4_OVR_UIF_vector//0x19
__interrupt void TIM4_OVR_UIF_IRQHandler(void)//对应IAP的中断地址：0x8060
{
      i++,n++;
      TIM4_SR=0x00;
      
      if(n>=141)
      {
        IWDG_KR = 0xAA;         //              喂狗  
        n=0;
      }
      if(i>=2410)//5s
      {
        UART_Send(read_pm25,5);                //向颗粒物传感器发送读取命令
        i=0;
  //      j++;
      }
      if(i==1000)
      {
        UART_Send(open_pump,6);
      }
 /*    else if(j>=7 && i>800)
      {
        UART_Send(open_pump,6);
        j=0;
      }*/
}