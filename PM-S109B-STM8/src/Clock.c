//#include"iostm8s103K3.h"
#include"iostm8s103F3.h"

void Clock_Init(void)
{
		   CLK_SWR=0xe1; //HSIΪ��ʱ��Դ
       //CLK_SWR=0xb4; //HSEΪ��ʱ��Դ
       //CLK_CKDIVR=0x00;//CPUʱ��0��Ƶ��ϵͳʱ��0��Ƶ
       CLK_CKDIVR=0x00;//CPUʱ��4��Ƶ��ϵͳʱ��0��Ƶ
       CLK_CSSR=0x01;//ʱ�Ӱ�ȫ���ʹ��
       CLK_SWCR=0x02;//ʹ���Զ�ʱ���л�
}
