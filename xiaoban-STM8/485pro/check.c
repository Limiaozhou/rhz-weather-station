////////////////////////////////////////////////////////////////////////////////// 
//У���㷨
//xj
//�汾��V1.0
//�޸�����:2015/8/31
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 

#include "check.h"

/***************************************
*@DESCRIPTION: --У��ͼ���
*
*@Input: --*pdat��������У��͵�����
*			 len��������У������ݵĳ���
*
*@Return: --��
****************************************/	
void Check_Addsum(unsigned char *pdat, unsigned int len)
{
   int i,sum=0;
   for (i = 0; i < len; i++)
   {     
     sum += *pdat++;
   }

    *pdat++ = sum>>8;
    *pdat = sum & 0xFF;

}

/***************************************
*@DESCRIPTION: --У��ͼ����Ƿ�ͨ��
*
*@Input: --*pdat��������У��͵�����
*			 len��������У������ݵĳ���
*
*@Return: --bool��ͨ������true�����򷵻�false
****************************************/	
bool Check_Checksum(unsigned char *pdat, unsigned int len)
{

   int i,sum=0,crc=0;
   for (i = 0; i < len; i++)
   {     
     sum += *pdat++;
   }

    crc = (*pdat++) << 8;
    crc += *pdat++;

    if (sum == crc) return true;

    return false;
}

//CRC16����
/***************************************
*@DESCRIPTION: --CRC16����
*
*@Input: --*pdat��������CRC16������
*			 len��������CRC16���ݵĳ���
*
*@Return: --��
****************************************/	
void Check_CalaCRC16(unsigned char *pdat, unsigned int len)
{
  unsigned int i;
  unsigned char j;
  unsigned int crc = 0xFFFF;
  for (i=0; i<len; i++)
  {
    crc ^= ((*(pdat++)) & 0x00FF);
    for (j=0; j<8; j++)
    {
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      { 
        crc >>= 1;
      }
    }
  }
  
  *pdat++ = crc & 0xFF;
  *pdat = crc>>8;
}

//CRC16����
/***************************************
*@DESCRIPTION: --CRC16�����Ƿ�ͨ��
*
*@Input: --*pdat��������CRC16������
*			 len��������CRC16���ݵĳ���
*
*@Return: --bool��ͨ������true�����򷵻�false
****************************************/	
bool Check_CheckCRC16(unsigned char *pdat, unsigned int len)
{
  unsigned int i,sum;
  unsigned char j;
  unsigned int crc = 0xFFFF;
  for (i=0; i<len; i++)
  {
    crc ^= ((*pdat++) & 0x00FF);
    for (j=0; j<8; j++)
    {
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      { 
        crc >>= 1;
      }
    }
  }
  
  sum = *pdat++;
  sum += (*pdat) << 8;
  if (sum == crc) return true;

  return false;
}
