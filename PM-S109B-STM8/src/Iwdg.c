#include "Iwdg.h"
#include"iostm8s103F3.h"
void IWDG_init(void)
{
  IWDG_KR = 0xCC; //����IWDG    
  IWDG_KR = 0x55; //��� PR �� RLR ��д����    
  IWDG_RLR = 0xff; //���Ź���������װ����ֵ          
  IWDG_PR = 0x06; //��Ƶϵ��Ϊ64 1.02s    
  IWDG_KR = 0xAA; //ˢ��IDDG������������Ź���λ��ͬʱ�ָ� PR �� RLR ��д����״̬ 
}