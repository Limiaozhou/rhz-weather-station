#include "delay.h"

extern int rain_count;
extern unsigned char rain_flag;//�ñ�Ǳ�ʾ�����������Ƿ����һ�Σ�1���ӣ��ɼ�

void Get_rain(float *temprain)
{
	if(rain_flag)
		*temprain = rain_count*0.2;
}
