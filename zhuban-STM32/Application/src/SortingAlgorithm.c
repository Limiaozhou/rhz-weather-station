#include "includes.h"		
#include "SortingAlgorithm.h"

////////////////////////////////////////////////////////////////////////////////// 
//�����㷨
//������mfg
//�汾��V1.0
//�޸�����:2015/8/8
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 
/***************************************
*@DESCRIPTION: --��������
*
*@Input: --��
*
*@Out: --��
*
*@Return: --��
****************************************/	
void Int_swap(int *p1, int *p2)
{
      int temp;
      temp=*p1;
      *p1=*p2;
      *p2=temp;
}
/***************************************
*@DESCRIPTION: --����������
*
*@Input: --��
*
*@Out: --��
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
*@DESCRIPTION: --������������
*
*@Input: --��
*
*@Out: --��
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
*@DESCRIPTION: --��������������
*
*@Input: --��
*
*@Out: --��
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
*@DESCRIPTION: --����ð������
*
*@Input: --��
*
*@Out: --��
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
*@DESCRIPTION: --������ð������
*
*@Input: --��
*
*@Out: --��
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








