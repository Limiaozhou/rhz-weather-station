#include"iostm8s103F3.h"

void Clock_Init(void)
{
  CLK_SWR=0xe1; //HSIΪ��ʱ��Դ
  while(!(CLK_ICKR&0x02));
  CLK_CKDIVR=0x00;//CPUʱ��0��Ƶ��ϵͳʱ��0��Ƶ
  CLK_CSSR=0x01;//ʱ�Ӱ�ȫ���ʹ��
  CLK_SWCR=0x02;//ʹ���Զ�ʱ���л�
  
}
