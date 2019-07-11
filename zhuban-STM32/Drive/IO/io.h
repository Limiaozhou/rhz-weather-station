#ifndef __IO_H
#define __IO_H	 
#include "sys.h"

////////////////////////////////////////////////////////////////////////////////// 
//IO操作
//创建：mfg
//版本：V1.0
//修改日期:2015/7/31
//Copyright(C) 深圳市睿海智电子科技有限公司 
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////  

//LED端口定义
#define RLED PBout(12)  // 
#define YLED PEout(13)  //
#define GLED PBout(14)  // 

void IO_Init(void);//初始化		 				    
#endif

















