#include"iostm8s103K3.h"
#include"typedef.h"
#include "i2c_slave.h"
//从机程序：
#define  SLAVE_ADDRESS  0xA0
#define  DSIZE           8

static uchar R_index, T_index, D_ready;
uchar I2C_Sbuffer[DSIZE];   //存储要发送的值
static uchar I2C_Rbuffer[DSIZE];   //存储要发送的值
uint   Event = 0x00;
/***IIC初始化****/
void Init_I2C_Slave (void)
{
    CLK_PCKENR1 |= 0x01;
    I2C_CR1 =0x81;//关闭时钟延展，启动IIC
    I2C_CR2=0x04;
    I2C_FREQR=4;
    I2C_OARL = (SLAVE_ADDRESS << 1);//设备地址
    I2C_OARH=0x40;
    I2C_ITR=0x07; 
}

void Deal_I2C_Slave (void)
{  uchar i;
   if(D_ready>=2)
   {
       for(i=0;i<DSIZE;i++)
       {
           I2C_Sbuffer[i] = I2C_Rbuffer[i];
       }
   }
}

/**
 *
 *  2) Advanced state monitoring
 *******************************************************************************
 */
/**
  * @brief  Returns the last I2C Event.
  *
  * @note: For detailed description of Events, please refer to section
  *    I2C_Events in stm8s_i2c.h file.
  *
  * @retval The last event
  *   This parameter can be any of the  @ref I2C_Event_TypeDef enumeration.
  */
I2C_Event_TypeDef I2C_GetLastEvent(void)
{
 
  uint lastevent = 0;
  uint flag1 = 0;
  uint flag2 = 0;
 
  if ((I2C_SR2_AF) != 0x00)
  {
 
    lastevent = I2C_EVENT_SLAVE_ACK_FAILURE;
   
}
  else
  {
 
    /* Read the I2C status register */
    flag1 = I2C_SR1;
    flag2 = I2C_SR3;
 
    /* Get the last event value from I2C status register */
    lastevent = ((uint)((uint)flag2 << 8) | (uint)flag1);
   
}
  /* Return status */
  return (I2C_Event_TypeDef)lastevent;
 
}

/***********IIC中断服务函数*********************/
#pragma vector=I2C_ADD10_vector//0x15
__interrupt void I2C_Slave_event(void) 
{
  
    /* Read SR2 register to get I2C error */
  if ((I2C_SR2) != 0)
  {
    /* Clears SR2 register */
    I2C_SR2 = 0;
  }
  
  Event = I2C_GetLastEvent();
  switch (Event)
  {
      /******* Slave transmitter ******/
      /* check on EV1 */
    case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:
      T_index = 0;
      D_ready = 1;
      break;

      /* check on EV3 */
    case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:
      /* Transmit data */
         I2C_DR = I2C_Sbuffer[T_index++];
      //I2C_SendData(Slave_Buffer_Rx[Tx_Idx++]);
      break;
      /******* Slave receiver **********/
      /* check on EV1*/
    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
      R_index = 0;
      break;

      /* Check on EV2*/
    case I2C_EVENT_SLAVE_BYTE_RECEIVED:
      I2C_Rbuffer[R_index++] = I2C_DR;
      break;

      /* Check on EV4 */
    case (I2C_EVENT_SLAVE_STOP_DETECTED):
            /* write to CR2 to clear STOPF flag */
            I2C_CR2 |= I2C_CR2_ACK;
            if(D_ready==1) D_ready = 2;
      break;

    default:
      break;
  }

  /*
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
*/

}
/*******************************************************/