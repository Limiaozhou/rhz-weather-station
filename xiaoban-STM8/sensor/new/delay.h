#ifndef _DELAY_H_
#define _DELAY_H_
//#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 
//延时函数  非系统
//创建：mfg
//版本：V1.0
//修改日期:2015/7/31
//Copyright(C) 深圳市睿海智电子科技有限公司  
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 
/******************@DESCRIPTION: --延时1us *************/	
extern void delay_us(int t);
/******************@DESCRIPTION: --延时1ms *************/	
extern void delay_ms(int t);
void delay_s(u16 ns);

#endif
