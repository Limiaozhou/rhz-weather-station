#include "timer.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

//定时器3中断服务程序	 
#include "gprs.h"
#include "delay.h"
#include "data.h"
#include "wdg.h"
#include "led.h"
#include "usart.h"
#include "sys.h"
#include "air_cond_swi.h"
//extern ST_EVNBALEDAT *psenddat;
//extern void packet_send(unsigned char *pdat);
char Ana_Flag=0;

int send_cnt=0;//7200
int send_time=50;       //默认发送周期，5秒
char send_flag=0;       //数据发送标志位

int send485_cnt=10;
char send485_flag=0;     

char wat_start_flag=0;    //浇水水泵1开始标志
int wat_cnt = 0;
char wat_flag =0;        //浇水时间到10s标志

//char fer_start_flag =0 ;
//int fer_cnt =0;
//char fer_flag = 0;

char magnetic_start_flag = 0;
int magnetic_cnt =0;
char magnetic_flag =0;

char blender_start_flag =0;
int blender_cnt =0;
char blender_flag = 0;



void TIM4_IRQHandler(void)
{ 		    		  			    
  if(TIM4->SR&0X0001)//溢出中断(200ms)
  {
    Ana_Flag=1;
    
    if(send485_cnt++>=10)//2秒钟发送一个485命令
    {
      send485_flag=1;
      send485_cnt=0;
    }
    
    if(send_cnt++>=send_time)
    {
      send_flag=1;
      send_cnt=0;
    }
    
     if(wat_start_flag)
    {
      if(wat_cnt++>=500)     //100s
      {
          wat_flag=1;
          wat_cnt =0;
       }
    }
    else
    {
      wat_cnt = 0;
    }
    
//     if(fer_start_flag)
//    {
//      if(fer_cnt++>=3000)
//      {
//          fer_flag=1;
//        
//          fer_cnt =0;
//       }
//    }
    
     if(magnetic_start_flag)
     {
      if(magnetic_cnt++>=50)
      {
          magnetic_flag=1;
          magnetic_cnt =0;
       }
     }
    else
    {
      magnetic_cnt = 0;
    }
    
   
     if(blender_start_flag)
    {
      if(blender_cnt++>=150)
      {
          blender_flag=1; 
          blender_cnt =0;
       }
    }
    else
    {
      blender_cnt = 0;
    }
    
    
  }				   
  TIM4->SR&=~(1<<0);//清除中断标志位 	    time3_cnt++;
}
//通用定时器4中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器4
void TIM4_Int_Init(u16 arr,u16 psc)
{
  RCC->APB1ENR|=1<<2;	//TIM4时钟使能
  TIM4->ARR=arr;  	//设定计数器自动重装值，2000 / 10 = 200ms
  TIM4->PSC=psc;  	//预分频器3600，36M/3600 = 0.01M = 10K
  TIM4->DIER|=1<<0;   //允许更新中断
  TIM4->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,3,TIM4_IRQn,2);//抢占1，子优先级3，组2
}














