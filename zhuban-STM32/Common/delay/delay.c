////////////////////////////////////////////////////////////////////////////////// 
//��ʱ����  ��ϵͳ
//������mfg
//�汾��V1.0
//�޸�����:2015/7/31
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 
#include "sys.h"
#include "wdg.h"
/***************************************
*@DESCRIPTION: --��ʱ 1us  40Mhz ʱ��
*
*@Input: --��
*
*@Out: --��
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
*@DESCRIPTION: --��ʱ 1ms  40hzM ʱ��
*
*@Input: --��
*
*@Out: --��
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
