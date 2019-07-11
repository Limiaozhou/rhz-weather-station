/********************************************************************* 
*                   Copyright (c) 2013-2015,睿海智 
*                     All rights reserved. 
*文 件 名：eeprom.h 
*描    述：eeprom 读写 
*当前版本：V1.00 
*作    者：MFG 
*创建日期：2015.01.22 
**********************************************************************/  
#ifndef __EEPROM_H__  
#define __EEPROM_H__  
#include"typedef.h"

extern void eeprom_read(ushort Addr, uchar *pdatas, uchar len);
extern bool eeprom_write(ushort Addr, uchar *pdatas, uchar len);
 
#endif  