#include "mem.h"
#include "string.h"
////////////////////////////////////////////////////////////////////////////////// 
//�ڴ����
//������mfg
//�汾��V1.0
//�޸�����:2015/7/31
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 
/***************************************
*@DESCRIPTION: --����
*
*@Input: --pdest�� Ŀ��ָ��   len�����ݳ���
*
*@Out: --��
*
*@Return: --��
****************************************/	
//������ڽ��պ���
void mem_clear(unsigned char *pdest, int len)
{ 
	  int i;
    for(i=0;i<len; i++)
    {
      *pdest++ = 0;
    }
}

/***************************************
*@DESCRIPTION: --����
*
*@Input: --pdest�� Ŀ��ָ��   len�����ݳ���
*
*@Out: --��
*
*@Return: --��
****************************************/	
/*
void mem_fclear(float*pdest, int len)
{ 
	  int i;
    for(i=0;i<len; i++)
    {
      *pdest++ = 0;
    }
}*/
/***************************************
*@DESCRIPTION: --��ֵ
*
*@Input: --pdest�� Ŀ��ָ��   len�����ݳ���  dat:��������
*
*@Out: --��
*
*@Return: --��
****************************************/	

void mem_set(unsigned char *pdest, int len,unsigned char dat)
{ 
	  int i;
    for(i=0;i<len; i++)
    {
      *pdest++ = dat;
    }
}

/***************************************
*@DESCRIPTION: --����
*
*@Input: --pdest�� Ŀ��ָ��   psrc��Դָ��  len�����ݳ���
*
*@Out: --��
*
*@Return: --��
****************************************/	

void mem_copy(unsigned char *pdest, unsigned char *psrc,int len)
{ 
	  int i;
    for( i=0;i<len; i++)
    {
      *pdest++ = *psrc++;
    }
}
void mem_copy_convert_port(unsigned char *pdest, unsigned char *psrc,int len)
{ 
    int i;
    unsigned char *temp=psrc+len-1;
    for( i=0;i<len; i++)
    {
      *pdest++ = *temp--;
    }
}

/***************************************
*@DESCRIPTION: --����
*
*@Input: --pdest�� Ŀ��ָ��   psrc��Դָ��  len�����ݳ���
*
*@Out: --��
*
*@Return: --��
****************************************/	
/*
void mem_fcopy(float *pdest, float *psrc,unsigned char len)
{ 
	  int i;
    for( i=0;i<len; i++)
    {
      *pdest++ = *psrc++;
    }
}
*/

