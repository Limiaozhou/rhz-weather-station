//#include"iostm8s103K3.h"
#include"iostm8s103F3.h"
#include"delay.h"

void IO_Init(void)
{
  
	  PD_DDR|=0x80;
	  PD_CR1|=0x80;
	  PD_CR2|=0x00;
  
   /* PB_DDR|=0x80;
	  PB_CR1|=0x80;
	  PB_CR2|=0x00;
  */
}

void IO_Set(char flag)
{
	 PD_ODR_ODR7 = flag;
}

void IO_light(int time, int count)
{
	 for(int i = 0; i< count; i++)
   {
      PD_ODR^=0x80;
      delay_ms(time);
   }
}