#include"iostm8s103F3.h"
#include"delay.h"
#include"Clock.h"
#include"UART.h"
#include"eeprom.h"
#include "i2c.h"
#include"config.h"
#include"timer.h"
#include "Iwdg.h"
#include "sht20.h"
#include "data.h"
#include "mem.h"
//#include "485protocol.h"
#include "co2.h"
//#include "CCS811.h"
//#include "ms5611.h"
#include "light-BHVI.h"

#include "bsp_iic_master.h"
#include "pressure-GY.h"

//#define LK


short int collect_time=0;
//extern char Flash_data[4];


//void float_to_str(uchar *pbuf,float num,char count)
//{
//
//     *pbuf++ = (int)num/1000 + 0x30;
//     *pbuf++ = (int)num/100%10 + 0x30;
//     *pbuf++ = (int)num/10%10 +0x30;
//     *pbuf++ = (int)num%10 +0x30;
//     *pbuf++ = '.';
//     
//     
//     float dat = num- (int)num;
//     while(count--)
//     {
//        dat *= 10;
//        *pbuf++ = (char)dat + 0x30;
//        dat = dat-(int)dat;
//     }
//     
//     *pbuf++ = '\n';
//}


void main()
{
//  float light_data,temp_data,hum_data;
//   unsigned char ptr[10],ptd[10];
  
  Clock_Init();  //HSI-16MHz
  IIC_init();  //PB4-clk，PB5-sda，温湿度，光照，气压
  co2_init();  //PC4-PWM检测，引脚外部中断，定时器计时
  Init_Timer4();  //T = 2.048ms
  
  asm("rim");//开全局中断 asm("rim");//开中断，sim为关中断
  mem_clear((unsigned char*)&Evndat,sizeof(Evndat));
  
  UART_Init(1);  //UART1-9600
  Evndat.co2=580;//00 00 11 44
  IIC_Master_Init(2);  //PD2-clk，PD3-sda，TVOC
  IIC_Master_Init(3);  //PC6-clk，PC7-sda，PM
//  get_data();             //获取传感器采集的值
  
  IWDG_init();  //Tmax = 1.02s
  while (1)
  { 
    if(collect_time>1050)//2.048 * 1050 = 2.1504s 读一次数据
    {
        collect_time=0;
        get_data();//数据获取的总的函数
    
        IWDG_Feed();  //刷新IDDG，避免产生看门狗复位，定时中断也有用
    }
  }
}