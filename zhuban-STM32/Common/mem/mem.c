#include "mem.h"
#include "string.h"
////////////////////////////////////////////////////////////////////////////////// 
//内存操作
//创建：mfg
//版本：V1.0
//修改日期:2015/7/31
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
////////////////////////////////////////////////////////////////////////////////// 
/***************************************
*@DESCRIPTION: --清零
*
*@Input: --pdest： 目的指针   len：数据长度
*
*@Out: --无
*
*@Return: --无
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
*@DESCRIPTION: --清零
*
*@Input: --pdest： 目的指针   len：数据长度
*
*@Out: --无
*
*@Return: --无
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
*@DESCRIPTION: --置值
*
*@Input: --pdest： 目的指针   len：数据长度  dat:设置数据
*
*@Out: --无
*
*@Return: --无
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
*@DESCRIPTION: --复制
*
*@Input: --pdest： 目的指针   psrc：源指针  len：数据长度
*
*@Out: --无
*
*@Return: --无
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
*@DESCRIPTION: --比较
*
*@Input: --str1： 待比较的字符串1
         --str2：待比较的字符串2  
         len：   需比较的长度
*
*@Out: --无
*
*@Return: --无
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
*@DESCRIPTION: --在s1中查找s2的信息，返回s2的首地址
*
*@Input: --s1 要查找的源   s2：待查找的信息  
len1：s1的长度 len2：s2的长度
*
*@Out: --无
*
*@Return: --找到s2的首地址
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
*@DESCRIPTION: --复制
*
*@Input: --pdest： 目的指针   psrc：源指针  len：数据长度
*
*@Out: --无
*
*@Return: --无
****************************************/	

void mem_fcopy(float *pdest, float *psrc,unsigned char len)
{ 
  int i;
  for( i=0;i<len; i++)
  {
    *pdest++ = *psrc++;
  }
}



