#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#include "multi_timer.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
 #define CLI() __set_PRIMASK(1) //关闭中断
 #define SEI() __set_PRIMASK(0)  //开启中断

extern char hmi_send_flag;  //屏幕发送标志位
extern char relay_send_flag;  //继电器板发送标志位

void TIM4_Int_Init(u16 arr,u16 psc);
#endif























