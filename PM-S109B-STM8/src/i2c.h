//#include"iostm8s103K3.h"
#include"iostm8s103F3.h"
#include"typedef.h"

#ifndef _IIC_H_
#define _IIC_H_

//初始化
extern void IIC_Init(void);


#endif
//从机程序：
/*uchar index;
uchar I2C_Sbuffer[8];   *///存储要发送的值
/***IIC初始化****/
/*
void Init_I2C_Slave (void)
{
    CLK_PCKENR1 |= 0x01;
    I2C_CR1 =0x81;//关闭时钟延展，启动IIC
    I2C_CR2=0x04;
    I2C_FREQR=1;
    I2C_OARL=0xa0;//设备地址
    I2C_OARH=0x40;
    I2C_ITR=0x06; 
    index = 0;
}
*/
/***********IIC中断服务函数*********************/
/*
#pragma vector=I2C_ADD10_vector//0x15
__interrupt void I2C_Slave_event(void) 
{

  static uchar sr1;                                        
  static uchar sr2;
  static uchar sr3;
  uchar tsr1,tsr2,tsr3;
        

  if(I2C_SR1_ADDR)     // 地址匹配
  {
    uchar erro=8;
    index=0;
    I2C_DR=I2C_Sbuffer[0];
    tsr1=I2C_SR1;
    tsr3=I2C_SR3;
    index++;
    while(I2C_SR1_BTF&(erro--));
  }
  if(I2C_SR1_TXE)      //继续发送数据
  {
    uchar erro=8;
    I2C_DR=I2C_Sbuffer[index++];
    while(I2C_SR1_BTF&(erro--));
  }
  if(I2C_SR2_AF)
  {
    I2C_SR2 &= ~I2C_SR2_AF;
  }
  if(I2C_SR2_BERR || I2C_SR1_STOPF)
  {
    tsr1=I2C_SR1;
    I2C_SR2 &= ~I2C_SR2_BERR;
  }


}*/
/*******************************************************/