#ifndef _FILTERALGORITHM_H_
#define _FILTERALGORITHM_H_
//#include "typedef.h"

////////////////////////////////////////////////////////////////////////////////// 
//�˲��㷨
//������mfg
//�汾��V1.0
//�޸�����:2015/8/8
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 
#define   FILTERBUFSIZE    10
#define   WIDTHSIZE    40
#define uchar unsigned char
	
typedef struct 
{
  float pre_dat;
  int nol_dat;
  char flag;
  char error_flag;//?????3??????????,??????????????
}Pre_data;

extern float Filter_Avefloat(float *pdat, uchar len);

extern float Filter_fftfloat(float *pdat, uchar len, uchar width);

extern float Filter_Widthfloat(float *pdat, uchar len, uchar width);

extern float Filter_Sortdifferencefloat(float *pdat, uchar len);

extern void Filter_data(float *act_data,Pre_data *pre,unsigned char range,int error_range);
	
#endif
