#ifndef __CHECK_h__
#define __CHECK_h__

#include <stdbool.h>
  
//����ۻ���
void Check_Addsum(unsigned char *pdat, unsigned int len);
//�ۻ��ͼ���
bool Check_Checksum(unsigned char *pdat, unsigned int len);
//CRC16����
void Check_CalaCRC16(unsigned char *pdat, unsigned int len);
//CRC16����
bool Check_CheckCRC16(unsigned char *pdat, unsigned int len);
#endif