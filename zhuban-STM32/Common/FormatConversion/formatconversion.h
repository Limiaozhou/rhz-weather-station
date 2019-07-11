#ifndef _FORMATCONVERSION_H_
#define _FORMATCONVERSION_H_
//#include "typedef.h"

////////////////////////////////////////////////////////////////////////////////// 
//���ݸ�ʽת��
//������mfg
//�汾��V1.0
//�޸�����:2015/7/31
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 
#define uchar unsigned char

typedef union  
{   
   double  value;   
   struct   
    {  
        unsigned char low_byte;  
        unsigned char mlow_byte;  
        unsigned char mhigh_byte;  
        unsigned char high_byte;  
     }double_byte; 
}Double4Byte; 

typedef union  
{   
   float  value;   
   struct   
    {  
        unsigned char low_byte;  
        unsigned char mlow_byte;  
        unsigned char mhigh_byte;  
        unsigned char high_byte;  
     }float_byte; 
}Float4Byte; 

typedef union  
{   
   int  value;   
   struct   
    {  
        unsigned char low_byte;  
        unsigned char mlow_byte;  
        unsigned char mhigh_byte;  
        unsigned char high_byte;  
     }int_byte; 
}Int4Byte; 

typedef union  
{   
  short  value;   
  struct   
  {  
    unsigned char low_byte;  
    unsigned char high_byte;  
  }int_byte; 
}Short2Byte; 

/******************@DESCRIPTION: -- �ַ���תdouble *************/	
extern double chartodouble(uchar *pdat);
/******************@DESCRIPTION: -- �ַ���תfloat *************/	
extern float chartofloat(uchar *pdat);
/******************@DESCRIPTION: -- �ַ���תint *************/	
extern int chartoint(uchar *pdat);

/******************@DESCRIPTION: -- �ַ�����ת *************/	
extern void reverse(char *s);
/******************@DESCRIPTION: -- ����ת�ַ��� *************/	
extern char inttochar(char *s,int n);
/******************@DESCRIPTION: -- floatת�ַ��� *************/	
extern char floattochar(char *str, float number, uchar dd);
/******************@DESCRIPTION: -- doubleת�ַ���*************/	
extern char doubletochar(char *str, double number, uchar dd);

extern short chartoshort(uchar *pdat);

#endif
