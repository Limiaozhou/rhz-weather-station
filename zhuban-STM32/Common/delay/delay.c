////////////////////////////////////////////////////////////////////////////////// 
//延时函数  非系统
//创建：mfg
//版本：V1.0
//修改日期:2015/7/31
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
////////////////////////////////////////////////////////////////////////////////// 
#include "sys.h"
#include "wdg.h"
/***************************************
*@DESCRIPTION: --延时 1us  40Mhz 时钟
*
*@Input: --无
*
*@Out: --无
*
*@Return: --int
****************************************/	
void delay_us(int t)
{
  while(t--)
  {
    __asm("NOP\n  "     
          "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  "   
	  "NOP\n  " );
  }
}
/***************************************
*@DESCRIPTION: --延时 1ms  40hzM 时钟
*
*@Input: --无
*
*@Out: --无
*
*@Return: --int
****************************************/	
void delay_ms(int t)
{
  while(t--)
  {
    delay_us(1000);
  }
}
void delay_s(u16 ns)
{
  int i=0;
  for(i=0;i<ns;i++)
  {
    delay_ms(500);
    IWDG_Feed();
    delay_ms(500);
  }
    
}
