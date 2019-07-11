#ifndef _FORMATCONVERSION_H_
#define _FORMATCONVERSION_H_
//#include "typedef.h"

////////////////////////////////////////////////////////////////////////////////// 
//数据格式转换
//创建：mfg
//版本：V1.0
//修改日期:2015/7/31
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
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

/******************@DESCRIPTION: -- 字符串转double *************/	
extern double chartodouble(uchar *pdat);
/******************@DESCRIPTION: -- 字符串转float *************/	
extern float chartofloat(uchar *pdat);
/******************@DESCRIPTION: -- 字符串转int *************/	
extern int chartoint(uchar *pdat);

/******************@DESCRIPTION: -- 字符串反转 *************/	
extern void reverse(char *s);
/******************@DESCRIPTION: -- 整数转字符串 *************/	
extern char inttochar(char *s,int n);
/******************@DESCRIPTION: -- float转字符串 *************/	
extern char floattochar(char *str, float number, uchar dd);
/******************@DESCRIPTION: -- double转字符串*************/	
extern char doubletochar(char *str, double number, uchar dd);

extern short chartoshort(uchar *pdat);

#endif
