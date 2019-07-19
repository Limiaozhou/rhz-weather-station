#include"iostm8s103F3.h"
#include "co2.h"
#include "delay.h"
void co2_init(u16 arr ,u16 psc)
{
  TIM2_CR1  =0x00;
  TIM2_CR1  |=0x01;//使能计数器
  
  PD_DDR &= ~0x08;//Pd3输入
  PD_CR1 &= ~0x08;//浮空输入
  PD_CR2 &= ~0x08;//禁止外部中断
  //定时器采用Fmaster时钟  频率为16M
 

   
  TIM2_CNTRL =arr&0xff;
  TIM2_CNTRH =arr>>8;
 
  TIM2_ARRL =0x00;
  TIM2_ARRL =arr&0xff;//重装载寄存器低位
   TIM2_ARRH =0x00;
  TIM2_ARRH =arr>>8;//重装载寄存器高位
  
  TIM2_PSCR &=0xf0;//清0
  TIM2_PSCR |=psc&0x0f;//预分频值
  
  //TIM2_CR1  |=0x80;//预装载值可以缓冲
  
  TIM2_CCMR2 =0x00;
  TIM2_CCMR2 |=0x01; //CC1通道被配置为输入，IC2映射在TI2FP2上；
  
  TIM2_CCER1 &=~(0x01<<5);//捕获发生在上升沿
  
  TIM2_IER =0x00;//
  TIM2_IER |=(0x01<<2);//CC2IE：允许捕获/比较2中断
  TIM2_IER |=0x01;//UIE：允许更新中断
  //TIM2_IER |=(0x01<<6);//触发中断使能
  
  // TIM4_EGR=0x01;//产生一次更新事件
  TIM2_CCER1 |=0x10;//CC2E：输入/捕获2输出使能。
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
#pragma vector= TIM2_OVR_UIF_vector //0x10
__interrupt void TIM2_OVR_UIF_IRQHandler(void)
{  
  ;
}

#pragma vector= TIM2_CAPCOM_CC2IF_vector //0x10
__interrupt void TIM2_CAPCOM_CC2IF_IRQHandler(void)
{  
  if((TIM2CH2_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{	  
		//if (TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)//函数判断相应的中断标志位 更新中断
                if(TIM2_SR1&0x01)//产生更新事件中断
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
	//if (TIM_GetITStatus(TIM4,TIM_IT_CC4) != RESET)//捕获1发生捕获事件
              if(TIM2_SR1 & (0x01<<3))//发生捕获中断
              {
              
		if(TIM2CH2_CAPTURE_STA&0X40)		//捕获到一个下降沿
		{	  			
                    TIM2CH2_CAPTURE_STA|=0X80;		//标记成功捕获到一次下降沿
                    //TIM4CH4_CAPTURE_VAL=TIM_GetCapture4(TIM4);
                    TIM2CH2_CAPTURE_VAL = TIM2_CCR2H;
                    TIM2CH2_CAPTURE_VAL = TIM2CH2_CAPTURE_VAL<<8+TIM2_CCR2L;
                    //TIM4CH4_CAPTURE_VAL = TIM2_CCR2H<<8+TIM2_CCR2L;//和上面的两行一个意思
                    
                    //TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获极性配置
                     TIM2_CCER1 &= ~(0x01<<5);//设置捕获发生在上升沿
		}
		else  								//还未开始,第一次捕获上升沿
		{
                    TIM2CH2_CAPTURE_STA =0;			//清空
                    TIM2CH2_CAPTURE_VAL =0;
                    //TIM_SetCounter(TIM4,0);//设置寄存器的值
                    TIM2_CNTRH =0;//计数寄存器清0
                    TIM2_CNTRL =0;
                    TIM2CH2_CAPTURE_STA|=0X40;  //标记捕获到了上升沿
                    //TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获 
                    TIM2_CCER1 |=0x1<<5;//设置为下降沿捕获
		}		    
	     }	   
    }	
     TIM2_SR1 &=~0x01;//清楚中断标志
     TIM2_SR1 &= ~0x04;//清楚中断标志
     //TIM_ClearITPendingBit(TIM4,TIM_IT_CC4|TIM_IT_Update); //清除中断标志位
  return;
}
