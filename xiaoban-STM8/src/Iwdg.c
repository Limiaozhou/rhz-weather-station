#include "Iwdg.h"
#include"iostm8s103F3.h"
void IWDG_init(void)
{
  IWDG_KR = 0xCC; //����IWDG    
  IWDG_KR = 0x55; //��� PR �� RLR ��д����    
  IWDG_RLR = 0xff; //���Ź���������װ����ֵ��Tmax = 255 * 4ms = 1.02s
  IWDG_PR = 0x06; //��Ƶϵ��Ϊ256,128K / 2 /256 = 0.25KHz
  IWDG_KR = 0xAA; //ˢ��IDDG������������Ź���λ��ͬʱ�ָ� PR �� RLR ��д����״̬ 
}

void IWDG_Feed()
{
  IWDG_KR = 0xAA;  //ˢ��IDDG������������Ź���λ
}