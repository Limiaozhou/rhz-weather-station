#include "control.h"
#include "mem.h"
#include "uart.h"
#include "check.h"
#include "timer.h"
char valve1 =  0;
char valve2 =  0;
char valve3 =  0;
char valve4 =  0;
char valve5 =  0;
char read485 = 0;
CON  WAT;
#define valve1_H()  valve1io = 1
#define valve1_L()  valve1io = 0
#define valve2_H()  valve2io = 1
#define valve2_L()  valve2io = 0
#define valve3_H()  valve3io = 1
#define valve3_L()  valve3io = 0
#define valve4_H()  valve4io = 1
#define valve4_L()  valve4io = 0
#define valve5_H()  valve5io = 1
#define valve5_L()  valve5io = 0

extern char Flash_data[4];

void struct_init(void)
{
  WAT.water1 = 0;
  WAT.water2 = 0;
  
//  WAT.valve4flow = 10;
  
}
void io_init(void)
{
  PC_DDR|=0x10;//PC4，输出
  PC_CR1|=0x10;//推挽输出
  PC_CR2 &= ~0x10;  //2M输出 
  
  PC_DDR|=0x20;//PC5
  PC_CR1|=0x20;//推挽输出
  PC_CR2 &= ~0x20;  //2M输出 
  
  PC_DDR|=0x40;//PC6
  PC_CR1|=0x40;//推挽输出
  PC_CR2 &= ~0x40;  //2M输出 
  
  PC_DDR|=0x80;//PC7
  PC_CR1|=0x80;//推挽输出
  PC_CR2 &= ~0x80;  //2M输出
  
  PA_DDR|=0x08;//PA3
  PA_CR1|=0x08;//推挽输出
  PA_CR2 &= ~0x08;  //2M输出
  
  //all relays off
  valve1_L();
  valve2_L();
  valve3_L();
  valve4_L();
  valve5_L();
}

void control(void)
{
  if(WAT.water1 > 0)//shui(water) on
  {
    valve1 =  1;
    valve3 =  1;
    valve5 =  1;
    valve2 =  0;
    valve4 = 0;
    WAT.water2 = 0;
    if(WAT.water1 > 1)
    {
      WAT.water1time = WAT.water1;
    }
    else if(WAT.water1 == 1)
    {
      WAT.water1time = Flash_data[2];
    }
  }
  else if(WAT.water2 > 0)//fei(fertilizer) on
  {
    valve1 =  1;
    valve2 =  1;
    valve4 =  1; 
    valve5 =  1;
    valve3 =  0;
    WAT.water1 =  0;
    if(WAT.water2 > 1)
    {
      WAT.water2time = WAT.water2;
    }
    else if(WAT.water2 == 1)
    {
      WAT.water2time = Flash_data[3];
    }
  }
  else 
  {
    valve1 =  0;
    valve3 =  0;
    valve5 =  0;
    valve2 =  0;
    valve4 = 0;
  }
  
}


void io_control(void)
{
  if(valve1 == 1)
  {
    valve1_H();//总阀开
  }
  else
  {
    valve1_L();//总阀开
  }
  if(valve2 == 1)
  {
    valve2_H(); //肥开
  }
  else
  {
    valve2_L();//肥关
  }
  if(valve3 == 1)
  {
    valve3_H();//水开
  }
  else
  {
    valve3_L();//水关
  }
  if(valve4 == 1)//Stiring on
  {
    valve4_H();//水开
  }
  else
  {
    valve4_L();//水关
  }
  if(valve5 == 1)
  {
    valve5_H();//水泵开
  }
  else
  {
    valve5_L();//水泵关
  }
}



void wat_data(void)
{
  int i = 0;
  float time = 0;
   unsigned char seng485[100];
 //  seng485[0] = Flash_data[1];
   seng485[0] = 0x8A;
   seng485[1] = 0x03;
   seng485[2] = 0x1E;
   for(i=0;i<5;i++)
   {
      switch(i)
      {
 //       IWDG_Feed();
        case 0:
   //       WAT.water1 = 1;
          seng485[3+i*6] = 0x01;
          seng485[4+i*6] = 0x50;
          if(WAT.water1 > 0)
          WAT.time = WAT.water1time - time2;
          else
           WAT.time = 0; 
          mem_copy_convert_port(seng485+5+i*6,(unsigned char*)&(WAT.time),4);
  //        mem_copy_convert_port(seng485+5+i*6,(unsigned char*)&(WAT.water1),4);
          break;
         case 1:
         //  WAT.water2 = 1;
          seng485[3+i*6] = 0x1;
          seng485[4+i*6] = 0x52;
          if(WAT.water2 > 0)
          WAT.time = WAT.water2time - time3;
          else
            WAT.time = 0;
          mem_copy_convert_port(seng485+5+i*6,(unsigned char*)&(WAT.time),4);
 //         mem_copy_convert_port(seng485+5+i*6,(unsigned char*)&(WAT.water2),4);
          break;
         case 2:
          seng485[3+i*6] = 0x1;
          seng485[4+i*6] = 0x54;
          
          mem_copy_convert_port(seng485+5+i*6,(unsigned char*)&(WAT.valve4flow),4);
          break;
 /*        case 3:
          seng485[3+i*6] = 0x1;
          seng485[4+i*6] = 0x56;
          mem_copy_convert_port(seng485+5+i*6,(unsigned char*)&(WAT.water1time),4);
          break;
         case 4:
          seng485[3+i*6] = 0x1;
          seng485[4+i*6] = 0x58;
          mem_copy_convert_port(seng485+5+i*6,(unsigned char*)&(WAT.water2time),4);
          break; */
         case 3:
          seng485[3+i*6] = 0x1;
          seng485[4+i*6] = 0x56;
          time = (float )Flash_data[2];
          mem_copy_convert_port(seng485+5+i*6,(unsigned char*)&(time),4);
          break;
         case 4:
          seng485[3+i*6] = 0x1;
          seng485[4+i*6] = 0x58;
          time = (float )Flash_data[3];
          mem_copy_convert_port(seng485+5+i*6,(unsigned char*)&(time),4);
          break;
 /*        case 5:
          seng485[3+i*6] = 0x1;
          seng485[4+i*6] = 0x2A;
          mem_copy(seng485+5,(unsigned char*)&WAT.water1,4);
          break;
         case 6:
          seng485[3+i*6] = 0x1;
          seng485[4+i*6] = 0x2C;
          mem_copy(seng485+5,(unsigned char*)&WAT.water1,4);
          break;*/
 
//      default:
 //         break;
      }
   }
   Check_CalaCRC16(seng485, i*6+3);
   UART_Send(seng485,i*6+5);
}

void pwm_on(void)
{
  TIM2_CR1=0x01;
  int cycle = 0;
  cycle = (int)WAT.valve4flow;//What the f*k is this, stiring time?
  cycle =(100 - cycle)*16;
  TIM2_CCR1H=cycle/256; //捕获/比较寄存器，这个寄存器决定着PWM的占空比。CCR/ARR=PWM的占空比
  TIM2_CCR1L=cycle%256;  
 // TIM2_CCR1H=0x03; //捕获/比较寄存器，这个寄存器决定着PWM的占空比。CCR/ARR=PWM的占空比
 // TIM2_CCR1L=0x20;
}

void pwm_off(void)
{
  TIM2_CR1=0x01;
  TIM2_CCR1H=0x06; //捕获/比较寄存器，这个寄存器决定着PWM的占空比。CCR/ARR=PWM的占空比
  TIM2_CCR1L=0x40;   
//  TIM2_CR1=0x0;
  
//  int cycle = 0;
//  cycle = (int)WAT.valve4flow;
 // cycle =(100 - 1)*16;
//  TIM2_CCR1H=cycle/256; //捕获/比较寄存器，这个寄存器决定着PWM的占空比。CCR/ARR=PWM的占空比
//  TIM2_CCR1L=cycle%256;
  
//  TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, uFreq / 2, TIM2_OCPOLARITY_HIGH);
}