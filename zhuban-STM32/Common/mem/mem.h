#ifndef _MEM_H_
#define _MEM_H_
////////////////////////////////////////////////////////////////////////////////// 
//�ڴ����
//������mfg
//�汾��V1.0
//�޸�����:2015/7/31
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 
/******************@DESCRIPTION: --����**************/	
extern void mem_clear(unsigned char*pdest, int len);
extern void mem_fclear(float*pdest, int len);
/******************@DESCRIPTION: --��ֵ**************/	
extern void mem_set(unsigned char *pdest, int len,unsigned char dat);
/******************@DESCRIPTION: --����*************/	
extern void mem_copy(unsigned char *pdest, unsigned char *psrc,int len);
extern void mem_fcopy(float *pdest, float *psrc,unsigned char len);

extern char* find_str(const char *s1, const char *s2,int len,int len2);  

#endif
