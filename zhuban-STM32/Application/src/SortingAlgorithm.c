#include "includes.h"		
#include "SortingAlgorithm.h"

////////////////////////////////////////////////////////////////////////////////// 
//排序算法
//创建：mfg
//版本：V1.0
//修改日期:2015/8/8
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
////////////////////////////////////////////////////////////////////////////////// 
/***************************************
*@DESCRIPTION: --整数交换
*
*@Input: --无
*
*@Out: --无
*
*@Return: --无
****************************************/	
void Int_swap(int *p1, int *p2)
{
      int temp;
      temp=*p1;
      *p1=*p2;
      *p2=temp;
}
/***************************************
*@DESCRIPTION: --浮点数交换
*
*@Input: --无
*
*@Out: --无
*
*@Return: --floag
****************************************/	
void Float_swap(float *p1, float *p2)
{
      float temp;
      temp=*p1;
      *p1=*p2;
      *p2=temp;
}
/***************************************
*@DESCRIPTION: --整数插入排序
*
*@Input: --无
*
*@Out: --无
*
*@Return: --floag
****************************************/	
void IntSort_insert(int *a,int len)
{
      int i,j;
      for(i=0;i<len;i++)
      {
          for(j=i+1;j>=1;j--)
          {
              if(a[j]<a[j-1])
                   Int_swap(&a[j],&a[j-1]);              
          }
      }
}

/***************************************
*@DESCRIPTION: --浮点数插入排序
*
*@Input: --无
*
*@Out: --无
*
*@Return: --floag
****************************************/	
void FloatSort_insert(float *a,int len)
{
      int i,j;
      for(i=0;i<len;i++)
      {
          for(j=i+1;j>=1;j--)
          {
              if(a[j]<a[j-1])
                   Float_swap(&a[j],&a[j-1]);              
          }
      }
}

/***************************************
*@DESCRIPTION: --整数冒泡排序
*
*@Input: --无
*
*@Out: --无
*
*@Return: --floag
****************************************/	
void IntSort_bubble(int *a,int len)
{
        int i,j,change;

        for(i=0;i<len;i++)
        {
               change=0;
               for(j=len-1;j>i;j--)
               {
                        if(a[j]<a[j-1])
                        {
                                change=1;
                                Int_swap(&a[j],&a[j-1]);
                         }
               } 
               if(!change)
                        break;
        }
}

/***************************************
*@DESCRIPTION: --浮点数冒泡排序
*
*@Input: --无
*
*@Out: --无
*
*@Return: --floag
****************************************/	
void floatsort_bubble(float *pdat,int len)
{
        int i,j,change;

        for(i=0;i<len;i++)
        {
               change=0;
               for(j=len-1;j>i;j--)
               {
                        if(pdat[j]<pdat[j-1])
                        {
                                change=1;
                                Float_swap(&pdat[j],&pdat[j-1]);
                         }
               } 
               if(!change)
                        break;
        }
}








