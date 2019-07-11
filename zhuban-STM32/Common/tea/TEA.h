#ifndef __TEA_h__
#define __TEA_h__


//TEA加密函数  加密数据必须》=8个  且为4的整数倍，不足补0
void btea_encrypt( unsigned char* buf, unsigned char count);
//TEA解密函数
void btea_decrpyt( unsigned char* buf, unsigned char count);


#endif
