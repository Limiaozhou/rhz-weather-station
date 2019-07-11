#include "formatconversion.h"	
#include "string.h"

////////////////////////////////////////////////////////////////////////////////// 
//数据格式转换
//创建：mfg
//版本：V1.0
//修改日期:2015/7/31
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
////////////////////////////////////////////////////////////////////////////////// 


/***************************************
*@DESCRIPTION: --字符串转double
*
*@Input: --无
*
*@Out: --double
*
*@Return: --double
****************************************/	
double chartodouble(uchar *pdat)
{
  Double4Byte temp;
  
  temp.double_byte.low_byte = *pdat++;
  temp.double_byte.mlow_byte = *pdat++;
  temp.double_byte.mhigh_byte = *pdat++;
  temp.double_byte.high_byte = *pdat++;
  return temp.value;
}

/***************************************
*@DESCRIPTION: --字符串转float
*
*@Input: --无
*
*@Out: --float
*
*@Return: --float
****************************************/	
float chartofloat(uchar *pdat)
{
  Float4Byte temp;
  
  temp.float_byte.low_byte = *pdat++;
  temp.float_byte.mlow_byte = *pdat++;
  temp.float_byte.mhigh_byte = *pdat++;
  temp.float_byte.high_byte = *pdat;
  return temp.value;
}

/***************************************
*@DESCRIPTION: --字符串转int
*
*@Input: --无
*
*@Out: --int
*
*@Return: --int
****************************************/	
int chartoint(uchar *pdat)
{
  Int4Byte temp;
  
  temp.int_byte.low_byte = *pdat++;
  temp.int_byte.mlow_byte = *pdat++;
  temp.int_byte.mhigh_byte = *pdat++;
  temp.int_byte.high_byte = *pdat;
  return temp.value;
}

/***************************************
*@DESCRIPTION: --字符串转short
*
*@Input: --无
*
*@Out: --short
*
*@Return: --short
****************************************/	
short chartoshort(uchar *pdat)
{
  Short2Byte temp;
  
  temp.int_byte.high_byte = *pdat++;
  temp.int_byte.low_byte = *pdat;
  return temp.value;
}

/***************************************
*@DESCRIPTION: --字符串反转
*
*@Input: --pstr：数据输入
*
*@Out: --pstr：反转后数据输出
*
*@Return: --无
****************************************/	

void reverse(char *s) 
{
  char *c;
  char i;
  c = s + strlen(s) - 1;
  while(s < c) {
    i = *s;
    *s++ = *c;
    *c-- = i;
  }
}

/***************************************
*@DESCRIPTION: --整数转字符串
*
*@Input: --s：数据输入指针 n:整数
*
*@Out: --s：转换后数据输出  
*
*@Return: --整数长度
****************************************/	
char inttochar(char *s,int n) 
{
  int sign;
  char *ptr,len=0;
  ptr = s;
  if ((sign = n) < 0) n = -n;
  do {
    *ptr++ = n % 10 + '0';
    len++;
  } while ((n = n / 10) > 0);
  
  if (sign < 0)
  {	
    *ptr++ = '-';
    len++;
  }
  
  *ptr = '\0';
  
  reverse(s);
  return len;
}

/***************************************
*@DESCRIPTION: --float转字符串
*
*@Input: --str：数据输入缓存  number：输入数据  dd：小数字位数
*
*@Out: --str：转换后数据输出
*
*@Return: --输出总长度
****************************************/	
char floattochar(char *str, float number, uchar dd)
{
  char i,len=0;
  int temp = (int)number;
  float t2 = number - (int)number;
  
  len = inttochar(str,temp); 
  if(temp==0 && number<0)
  {
    str[0] = '-';
    str[1] = '0';
    len = 2;
  }
  else
  {
    len = inttochar(str,temp); 
  }
  
  *(str+len) = '.';
  
  if(t2<0) t2=-t2;
  for(i=1; i<=dd; i++)
  {
    t2 = (t2- (int)t2)*10;
    str[len+i] = (int)t2 + '0';
  }
  *(str+len + dd +1) = '\0';
  
  return len+dd+1;
}


/***************************************
*@DESCRIPTION: --double转字符串
*
*@Input: --str：数据输入缓存  number：输入数据  dd：小数字位数
*
*@Out: --str：转换后数据输出
*
*@Return: --输出总长度
****************************************/	
char doubletochar(char *str, double number, uchar dd)
{
  char i,len=0;
  int temp = (int)number;
  double t2 = number - (int)number;
  
  len = inttochar(str,temp); 
  if(temp==0 && number<0)
  {
    str[0] = '-';
    str[1] = '0';
    len = 2;
  }
  else
  {
    len = inttochar(str,temp); 
  }
  
  *(str+len) = '.';
  
  if(t2<0) t2=-t2;
  for(i=1; i<=dd; i++)
  {
    t2 = (t2- (int)t2)*10;
    str[len+i] = (int)t2 + '0';
  }
  *(str+len + dd +1) = '\0';
  
  return len+dd+1;
}

