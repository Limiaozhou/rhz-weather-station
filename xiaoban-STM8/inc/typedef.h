/********************************************************************* 
*                   Copyright (c) 2013-2015,睿海智 
*                     All rights reserved. 
*文 件 名：typedef.h 
*描    述：定义数据类型 (cpu data type) 
*当前版本：V1.00 
*作    者：MFG 
*创建日期：2015.01.22 
**********************************************************************/  
#ifndef __TYPEDEF_H__  
#define __TYPEDEF_H__  

typedef  unsigned char       BOOLEAN;  
typedef  unsigned char       boolean;  
typedef  unsigned char       INT8U; 
typedef  unsigned char       uchar; 
typedef  signed   char       INT8S;  
typedef  unsigned short      INT16U; 
typedef  signed   short      INT16S;  
typedef  unsigned int        INT32U;  
typedef  signed   int        INT32S;   
  
/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

#define UCHAR  unsigned char   
#define UINT   unsigned int  
#define USHORT unsigned short  
#define ULONG  unsigned long 
#define BOOL   unsigned char  
  
#define uchar  unsigned char   
#define uint   unsigned int  
#define ushort unsigned short 
#define ulong  unsigned long  
#define bool   unsigned char 

#ifndef    true  
  #define  true   1    
#endif 
#ifndef    false  
  #define  false   0    
#endif 

#ifndef    TRUE  
  #define  TRUE   1    
#endif 
#ifndef    FALSE  
  #define  FALSE   0    
#endif   
  
#ifndef    NULL  
  #define  NULL   (void *)0    
#endif   
#ifndef    null  
 #define   null   (void *)0    
#endif  

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, BitStatus, BitAction;
 
#endif  