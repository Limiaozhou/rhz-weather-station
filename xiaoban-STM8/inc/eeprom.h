/********************************************************************* 
*                   Copyright (c) 2013-2015,��� 
*                     All rights reserved. 
*�� �� ����eeprom.h 
*��    ����eeprom ��д 
*��ǰ�汾��V1.00 
*��    �ߣ�MFG 
*�������ڣ�2015.01.22 
**********************************************************************/  
#ifndef __EEPROM_H__  
#define __EEPROM_H__  
#include"typedef.h"

extern void eeprom_read(ushort Addr, uchar *pdatas, uchar len);
extern bool eeprom_write(ushort Addr, uchar *pdatas, uchar len);
 
#endif  