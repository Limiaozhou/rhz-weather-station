#include"iostm8s103F3.h"
#include "co2.h"
#include "delay.h"
void co2_init(u16 arr ,u16 psc)
{
  CLK_PCKENR1 |=0x01<<7;//使能定时器1时钟
  TIM1_CR1   =0x00;
  TIM1_CR2   =0x00;
  TIM1_SMCR  =0x00;
  TIM1_IER   =0x00; //中断使能寄存器
  TIM1_CCER2 =0x00;
  TIM1_CCMR1 =0x00;
  PC_DDR &= ~(0x01<<4);//Pc4输入
  PC_CR1 &= ~(0x01<<4);//浮空输入
  PC_CR2 &= ~(0x01<<4);//禁止外部中断
  //定时器采用Fmaster时钟  频率为16M
 

  TIM1_CCMR4 =0x00;
  TIM1_CCMR4 =0x02;//CC4通道被配置为输入，IC4映射在TI4FP4上；
   
   TIM1_CNTRH =arr>>8;
  TIM1_CNTRL =arr&0xff;

  //TIM1_ARRH =0x00;
  TIM1_ARRH =arr>>8;//重装载寄存器高位
  //TIM1_ARRL =0x00;
  TIM1_ARRL =arr&0xff;//重装载寄存器低位

  TIM1_PSCRH |=psc>>8;//预分频值
  TIM1_PSCRL =psc&0xff;//
  
  TIM1_CCMR1 |=0x02<4;//滤波器
   
  TIM1_SMCR=00;
  TIM1_SMCR=0x04;//复位模式

  TIM1_IER |=0x01<<4;//允许捕获/比较4中断
  TIM1_IER |=0x01;//允许更新中断

  TIM1_EGR =0x00;
  TIM1_CR1  |=0x81;//使能计数器
  TIM1_EGR |=0x01;//产生一次更新事件
  TIM1_CCER2 |=0x01<<4;//定时器通道4捕获比较使能
}

u8  TIM2CH2_CAPTURE_STA=0;	//输入捕获状态		    				
u16 TIM2CH2_CAPTURE_VAL;	//输入捕获值



u32 CO2temp_s;
void Get_CO2(float *tempfs)
{
  delay_ms(10);
  //	TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);
  //if(TIM_GetCapture2(TIM3)==300)TIM_SetCompare2(TIM3,0);	
  if(TIM2CH2_CAPTURE_STA&0X80)//成功捕获到了一次下降沿
  {
      CO2temp_s=TIM2CH2_CAPTURE_STA&0X3F;
      CO2temp_s*=65536;//溢出时间总和
      CO2temp_s+=TIM2CH2_CAPTURE_VAL;//得到总的高电平时间
      //if(temp_s==0)
      //tempfs=0.1;
      //else
      *tempfs=(float)CO2temp_s/250-8;
      if(0 != *tempfs)
      {
              *tempfs +=0.005;
      }
      TIM2CH2_CAPTURE_STA=0;//开启下一次捕获，状态标志位清零
  }
          //delay_ms(250);	
}
#pragma vector= TIM1_OVR_UIF_vector 
__interrupt void TIM1_OVR_UIF_IRQHandler(void)
{  
   if(TIM1_SR1&0x01)//产生更新事件中断
  {	   
      if(TIM2CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
      {
          if((TIM2CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
          {
                  TIM2CH2_CAPTURE_STA|=0X80;//标记成功捕获了一次
                  TIM2CH2_CAPTURE_VAL=0XFFFF;
          }
          else TIM2CH2_CAPTURE_STA++;
      }	 
  }
  TIM1_SR1 &=~0x01;//清楚更新中断标志
}

#pragma vector= TIM1_CAPCOM_CC4IF_vector //0x0e
__interrupt void TIM1_CAPCOM_CC4IF_IRQHandler(void)
{  
  if((TIM2CH2_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  

              if(TIM1_SR1 & (0x01<<4))//发生捕获中断
              {
              
		if(TIM2CH2_CAPTURE_STA&0X40)		//捕获到一个下降沿
		{	  			
                    TIM2CH2_CAPTURE_STA|=0X80;		//标记成功捕获到一次下降沿
                
                    TIM2CH2_CAPTURE_VAL = TIM1_CCR4H;
                    TIM2CH2_CAPTURE_VAL = TIM2CH2_CAPTURE_VAL<<8+TIM1_CCR4L;
                 
                      
                     TIM1_CCER2 &= ~(0x01<<5);//设置捕获发生在上升沿
		}
		else  								//还未开始,第一次捕获上升沿
		{
                    TIM2CH2_CAPTURE_STA =0;	//清空
                    TIM2CH2_CAPTURE_VAL =0;
                    //TIM_SetCounter(TIM4,0);//设置寄存器的值
                    TIM1_CNTRH =0;//计数寄存器清0
                    TIM1_CNTRL =0;
                    TIM2CH2_CAPTURE_STA|=0X40;  //标记捕获到了上升沿
                    //TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获 
                    TIM1_CCER2 |=0x1<<5;//设置为下降沿捕获
		}		    
	     }	   
    }	
  
   TIM1_SR1 &= ~(0x01<<4);
     //TIM_ClearITPendingBit(TIM4,TIM_IT_CC4|TIM_IT_Update); //清除中断标志位
  return;
}
