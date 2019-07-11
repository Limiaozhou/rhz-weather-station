#include "includes.h"		
#include "mem.h"
#include "SortingAlgorithm.h"
#include "filteralgorithm.h"
////////////////////////////////////////////////////////////////////////////////// 
//���ݸ�ʽת��
//������mfg
//�汾��V1.0
//�޸�����:2015/8/8
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 
#define uchar unsigned char
#define FILTERBUFSIZE 10
#define WIDTHSIZE 5
typedef  struct SORTFLOAT
{
   uchar index;
   float diff;
  
}ST_SORTDIFFFLOAT;

typedef  struct REPEAT
{
   uchar count;
   int dat;
  
}ST_REPEAT;


#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define FABS(a) ((a)<(0)?(-a):(a))

/***************************************
*@DESCRIPTION: --����ƽ��ֵ�˲�
*
*@Input: --��
*
*@Out: --��
*
*@Return: --floag
****************************************/	
float Filter_Avefloat(float *pdat, uchar len)
{
    float ret;
    uchar i;
	 //��ֵ��ֵ
	 for(i=0;i<len;i++)
	 {
		  ret += *pdat++;
	 }
	 
	 ret = ret/len;
	 
   return  ret;
}

/***************************************
*@DESCRIPTION: --float����ƽ��ֵ ��ֵ����
*
*@Input: --��
*
*@Out: --��
*
*@Return: --floag
****************************************/	
int Filter_Avevaluefloat(float *pdat, uchar len, uchar width)
{
    float ret,count=0,*p;
	  uchar i,num=0;
	  float sd[FILTERBUFSIZE];
	  p=pdat;
	  
	   //��ֵ��ֵ
	 for( i=0; i<len; i++)
	 {
		  count += *p++;
	 }
	 count = count/len;
	
	
	 p=pdat;
	  //�˲�
	 for( i=0; i<len; i++)
	 {
		  ret= *(p+i) - count;
			ret= FABS(ret);
		  if(ret <=width)  
			{	
				  sd[num] = *(p+i);
				  num++;
			}
	 }
	 
   mem_fcopy(pdat,sd,num);
   return  num;
}

/***************************************
*@DESCRIPTION: --�ظ��ж�
*
*@Input: --��
*
*@Out: --��
*
*@Return: --floag
****************************************/	
bool Filter_Findrepeat(int dat,ST_REPEAT *pdat, uchar len)
{
	 uchar i;
	 //Ƶ�ʼ���
	 for( i=0; i<len; i++)
	 {
		  if(dat == pdat->dat) return false;
	 }
	 
	 return true;
 }

/***************************************
*@DESCRIPTION: --�ظ�������ȡ
*
*@Input: --��
*
*@Out: --��
*
*@Return: --floag
****************************************/	
int Filter_repeat(int *pdat, uchar len)
{
	 uchar i, j,num=0,max=0;
   ST_REPEAT  re[FILTERBUFSIZE];
	 int *p;
	
	 p=pdat;
	 mem_clear((unsigned char *)&re[0].count, sizeof(re));
	 //Ƶ�ʼ���
	 for( i=0; i<len; i++)
	 {
		   if(Filter_Findrepeat(*pdat,re, num))
			 {
				  re[num].dat= *pdat;
				  for( j=0; j<len; j++)
	        {
						 if(re[num].dat == *(p+j))  re[num].count+=1;
					}
					num++;
			 }
			pdat++;
	 }
	 
	 
	 for( i=0; i<num; i++)
	 {
		  if(re[i].count > max) max =  re[i].count;
	 }
	 
	 for( i=0; i<num; i++)
	 {
		  if(re[i].count == max) return  re[i].dat;
	 }
	 
	 return 0;
	 
 }

/***************************************
*@DESCRIPTION: --floatƵ�ʹ���
*
*@Input: --��
*
*@Out: --��
*
*@Return: --floag
****************************************/	
float Filter_fftfloat(float *pdat, uchar len, uchar width)
{
    float *p;
	  uchar i,num=0,dd=0,fnum= 0;
	  float sf[FILTERBUFSIZE];
		int value,sb[FILTERBUFSIZE];
	  p=pdat;
	  
	 //mem_fclear(sf, sizeof(sf));
	// mem_iclear(sb, sizeof(sb));
	 //Ƶ�ʼ���
	 for( i=0; i<len; i++)
	 {
		  dd= ((int)*(p+i))/width;
			sb[num++] = dd;
	 }
	
	 value = Filter_repeat(sb, num);
	 
	 
	  //�ֹ���
	 for( i=0; i<len; i++)
	 {

		  dd= ((int)*(p+i))/width;
			if(dd>(value-1) && dd< (value+1))
		  {
		    sf[fnum++] = *(p+i);
			}
	 }
	 
   return  Filter_Sortdifferencefloat(sf, fnum);
}

/***************************************
*@DESCRIPTION: --float�޷�����
*
*@Input: --��
*
*@Out: --��
*
*@Return: --floag
****************************************/	
float Filter_Widthfloat(float *pdat, uchar len, uchar width)
{
    float ret,count=0,*p;
	  uchar i,j,num=0,dd=0;
	  float sd[FILTERBUFSIZE],st[FILTERBUFSIZE] ,sb[FILTERBUFSIZE];
	  
	 p=pdat;
	 //mem_fclear(sd, sizeof(sd));
	// mem_fclear(sb, sizeof(sb));
	// mem_fclear(st, sizeof(st));
	  //�ϴ�ֵ�����˲�
	 for( i=0; i<len; i++)
	 {
		  ret= *(p+i) - width;
			ret= FABS(ret);
		  if(ret <=WIDTHSIZE)  
			{	
				  sb[num] = *(p+i);
				  num++;
			}
	 }
	 
	 
	  p=sb;
	  //��ֵ
	  for(i=0;i<num;i++)
	 {
		    sd[i] = 0;
		    for( j=0;j<num;j++)
	      {
					ret= *(p+i) - *(p+j);
					ret= FABS(ret);
					sd[i] +=ret;
	      }
		  st[i] = sd[i];
	 }
	
	 //��ֵ
	 floatsort_bubble(st,num);
	 ret = st[(num-1)/2];
	 
	 //�˲�
	 for( i=0; i<num; i++)
	 {
		  if(sd[i]<=ret)  
			{	
				 count += *(pdat+i);
				 dd++;
			}
	 }
	 
	 if(dd>0) ret = count/dd;
	 else ret = 0;
   return  ret;
}

/***************************************
*@DESCRIPTION: --float��ֵ���� ƽ��ֵ�˲� ��ֵ
*
*@Input: --��
*
*@Out: --��
*
*@Return: --floag
****************************************/	
float Filter_Sortdifferencefloat(float *pdat, uchar len)
{
    float ret,count=0,*p;
	  uchar i,j,num=0;
	  float sd[FILTERBUFSIZE], st[FILTERBUFSIZE];
	
	  //mem_fclear(sd, sizeof(sd));
	  //mem_fclear(st, sizeof(st));
	  p=pdat;
	  //��ֵ
	  for(i=0;i<len;i++)
	 {
		    sd[i] = 0;
		    for( j=0;j<len;j++)
	      {
					ret= *(p+i) - *(p+j);
					ret= FABS(ret);
					sd[i] +=ret;
	      }
		  st[i] = sd[i];
	 }
	
	 //st = sd;
	 /*
	 //��ֵ��ֵ
	 for( i=0; i<len; i++)
	 {
		  ret += sd[i];
	 }
	 ret = ret/len;
	 */
	 //��ֵ
	 floatsort_bubble(st,len);
	 ret = st[(len-1)/2];
	 
	 //�˲�
	 for( i=0; i<len; i++)
	 {
		  if(sd[i]<=ret)  
			{	
				 count += *(pdat+i);
				 num++;
			}
	 }
	 
	 if(num>0) ret = count/num;
	 else ret = 0;
	 
   return  ret;
}

typedef struct 
{
  float pre_dat;
  int nol_dat;
  short flag;
  char error_flag;//?????3??????????,??????????????
}Pre_data;
void Filter_data(float *act_data,Pre_data *pre ,unsigned char range,int error_range)
{
  float tmp=*act_data;
  if((*act_data-pre->pre_dat)>=range)// || (pre.pre_dat-*act_data>range))
  {
    if(pre->flag!=0xff)
      *act_data = pre->pre_dat;
    pre->flag=1;
  }
  else if((pre->pre_dat-*act_data)>=range)
  {
    if(pre->flag!=1)
      *act_data = pre->pre_dat;
    pre->flag=-1;    
  }
  else
    pre->flag=0;
  if((error_range>0 && *act_data>error_range) || (error_range<0 && *act_data<0-error_range))// && (pre->error_flag<3))
  {
    pre->error_flag++;
    if(pre->error_flag<3)
    {
      *act_data=(float)pre->nol_dat;
    }
  }
  else
  {
    pre->nol_dat=(int)*act_data;
    pre->error_flag=0;
  }
  pre->pre_dat = tmp;
  
}



