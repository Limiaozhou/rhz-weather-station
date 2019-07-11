#include"iostm8s103F3.h"

void Clock_Init(void)
{
  CLK_SWR=0xe1; //HSI为主时钟源
  while(!(CLK_ICKR&0x02));
  CLK_CKDIVR=0x00;//CPU时钟0分频，系统时钟0分频
  CLK_CSSR=0x01;//时钟安全监测使能
  CLK_SWCR=0x02;//使能自动时钟切换
  
}
