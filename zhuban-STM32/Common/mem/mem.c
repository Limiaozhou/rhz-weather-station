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

void mem_fclear(float*pdest, int len)
{ 
  int i;
  for(i=0;i<len; i++)
  {
    *pdest++ = 0;
  }
}
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

/***************************************
*@DESCRIPTION: --�Ƚ�
*
*@Input: --str1�� ���Ƚϵ��ַ���1
         --str2�����Ƚϵ��ַ���2  
         len��   ��Ƚϵĳ���
*
*@Out: --��
*
*@Return: --��
****************************************/	

void mem_cmp(unsigned char *str1, unsigned char *str2,int len)
{ 
  int i;
  for( i=0;i<len; i++)
  {
    *str1++ = *str2++;
  }
}

/***************************************
*@DESCRIPTION: --��s1�в���s2����Ϣ������s2���׵�ַ
*
*@Input: --s1 Ҫ���ҵ�Դ   s2�������ҵ���Ϣ  
len1��s1�ĳ��� len2��s2�ĳ���
*
*@Out: --��
*
*@Return: --�ҵ�s2���׵�ַ
****************************************/	
char* find_str(const char *s1, const char *s2,int len1,int len2)  
{  
  int n,i;  
  if (*s2)  
  {  
    for( i=0;i<len1;i++)
    {  
      for (n=0; *(s1 + n) == *(s2 + n); n++)  
      {  
        if (n==len2-1)  
          return (char *)s1;  
      }  
      s1++;  
    }  
    return NULL;  
  }  
  else  
    return (char *)s1;  
  
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

void mem_fcopy(float *pdest, float *psrc,unsigned char len)
{ 
  int i;
  for( i=0;i<len; i++)
  {
    *pdest++ = *psrc++;
  }
}



