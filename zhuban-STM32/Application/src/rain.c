#include "delay.h"

extern int rain_count;
extern unsigned char rain_flag;//该标记表示雨量传感器是否完成一次（1分钟）采集

void Get_rain(float *temprain)
{
	if(rain_flag)
		*temprain = rain_count*0.2;
}
