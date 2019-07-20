#include "formatconversion.h"	
#include "string.h"

////////////////////////////////////////////////////////////////////////////////// 
//���ݸ�ʽת��
//������mfg
//�汾��V1.0
//�޸�����:2015/7/31
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 


/***************************************
*@DESCRIPTION: --�ַ���תdouble
*
*@Input: --��
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
*@DESCRIPTION: --�ַ���תfloat
*
*@Input: --��
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
*@DESCRIPTION: --�ַ���תint
*
*@Input: --��
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
*@DESCRIPTION: --�ַ���תshort
*
*@Input: --��
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
*@DESCRIPTION: --�ַ�����ת
*
*@Input: --pstr����������
*
*@Out: --pstr����ת���������
*
*@Return: --��
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
*@DESCRIPTION: --����ת�ַ���
*
*@Input: --s����������ָ�� n:����
*
*@Out: --s��ת�����������  
*
*@Return: --��������
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
*@DESCRIPTION: --floatת�ַ���
*
*@Input: --str���������뻺��  number����������  dd��С����λ��
*
*@Out: --str��ת�����������
*
*@Return: --����ܳ���
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
*@DESCRIPTION: --doubleת�ַ���
*
*@Input: --str���������뻺��  number����������  dd��С����λ��
*
*@Out: --str��ת�����������
*
*@Return: --����ܳ���
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

