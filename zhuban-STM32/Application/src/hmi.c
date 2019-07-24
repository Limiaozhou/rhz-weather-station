#include "hmi.h"

void hmi_send(void)  //发送屏幕数据
{
    unsigned char buf[25] = {0};  //发送数据缓存
    unsigned char end_buf[3] = {0xFF, 0xFF, 0xFF};  //发送数据结束符
    int data = 0;
    unsigned char len = 0;
    
    data = (int)(sensor.temp * 100);  //温度
    len = sprintf((char *)buf, "main.x0.val=%d", data);
    USART_Puts(USART2, buf, len);
    USART_Puts(USART2, end_buf, 3);
    delay_ms(100);
    
    data = (int)(sensor.humi * 100);  //湿度
    len = sprintf((char *)buf, "main.x1.val=%d", data);
    USART_Puts(USART2, buf, len);
    USART_Puts(USART2, end_buf, 3);
    delay_ms(100);
    
    data = (int)(sensor.light * 100);  //光照
    len = sprintf((char *)buf, "main.x2.val=%d", data);
    USART_Puts(USART2, buf, len);
    USART_Puts(USART2, end_buf, 3);
    
    if(wat_fer.ventilate)  //风机开显示按下颜色
    {
      len = sprintf((char *)buf, "menu.b0.bco=1024");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    else  //风机关显示弹起颜色
    {
      len = sprintf((char *)buf, "menu.b0.bco=34815");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    
    if(wat_fer.juanmo)  //卷膜开显示按下颜色
    {
      len = sprintf((char *)buf, "menu.b1.bco=1024");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    else  //卷膜关显示弹起颜色
    {
      len = sprintf((char *)buf, "menu.b1.bco=34815");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    
    if(wat_fer.wat4)  //水帘开显示按下颜色
    {
      len = sprintf((char *)buf, "menu.b2.bco=1024");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    else  //水帘关显示弹起颜色
    {
      len = sprintf((char *)buf, "menu.b2.bco=34815");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    
    if(wat_fer.yang)  //内遮阳开显示按下颜色
    {
      len = sprintf((char *)buf, "menu.b3.bco=1024");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    else  //内遮阳关显示弹起颜色
    {
      len = sprintf((char *)buf, "menu.b3.bco=34815");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    
    if(wat_fer.yang1)  //外遮阳开显示按下颜色
    {
      len = sprintf((char *)buf, "menu.b4.bco=1024");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    else  //外遮阳关显示弹起颜色
    {
      len = sprintf((char *)buf, "menu.b4.bco=34815");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
//    data = (int)wat_fer.ventilate;
//    if(data & (0x01 << 0))
//    {
//      len = sprintf((char *)buf, "fengji.b0.bco=1024");
//      USART_Puts(USART2, buf, len);
//      USART_Puts(USART2, end_buf, 3);
//    }
//    else
//    {
//      len = sprintf((char *)buf, "fengji.b0.bco=34815");
//      USART_Puts(USART2, buf, len);
//      USART_Puts(USART2, end_buf, 3);
//    }
//    if(data & (0x01 << 1))
//    {
//      len = sprintf((char *)buf, "fengji.b1.bco=1024");
//      USART_Puts(USART2, buf, len);
//      USART_Puts(USART2, end_buf, 3);
//    }
//    else
//    {
//      len = sprintf((char *)buf, "fengji.b1.bco=34815");
//      USART_Puts(USART2, buf, len);
//      USART_Puts(USART2, end_buf, 3);
//    }
//    if(data & (0x01 << 2))
//    {
//      len = sprintf((char *)buf, "fengji.b2.bco=1024");
//      USART_Puts(USART2, buf, len);
//      USART_Puts(USART2, end_buf, 3);
//    }
//    else
//    {
//      len = sprintf((char *)buf, "fengji.b2.bco=34815");
//      USART_Puts(USART2, buf, len);
//      USART_Puts(USART2, end_buf, 3);
//    }
//    if(data & (0x01 << 3))
//    {
//      len = sprintf((char *)buf, "fengji.b3.bco=1024");
//      USART_Puts(USART2, buf, len);
//      USART_Puts(USART2, end_buf, 3);
//    }
//    else
//    {
//      len = sprintf((char *)buf, "fengji.b3.bco=34815");
//      USART_Puts(USART2, buf, len);
//      USART_Puts(USART2, end_buf, 3);
//    }
//    if(data & (0x01 << 4))
//    {
//      len = sprintf((char *)buf, "fengji.b4.bco=1024");
//      USART_Puts(USART2, buf, len);
//      USART_Puts(USART2, end_buf, 3);
//    }
//    else
//    {
//      len = sprintf((char *)buf, "fengji.b4.bco=34815");
//      USART_Puts(USART2, buf, len);
//      USART_Puts(USART2, end_buf, 3);
//    }
}

void hmi_read(unsigned char *uartData,unsigned char len)
{
  char i;
//  typedef union
//  {
//    int int_data;
//    struct
//    {
//      int bit0:1;
//      int bit1:1;
//      int bit2:1;
//      int bit3:1;
//      int bit4:1;
//      int bit5:1;
//      int bit6:1;
//      int bit7:1;
//      
//      int bit8:1;
//      int bit9:1;
//      int bit10:1;
//      int bit11:1;
//      int bit12:1;
//      int bit13:1;
//      int bit14:1;
//      int bit15:1;
//      
//      int bit16:1;
//      int bit17:1;
//      int bit18:1;
//      int bit19:1;
//      int bit20:1;
//      int bit21:1;
//      int bit22:1;
//      int bit23:1;
//      
//      int bit24:1;
//      int bit25:1;
//      int bit26:1;
//      int bit27:1;
//      int bit28:1;
//      int bit29:1;
//      int bit30:1;
//      int bit31:1;
//    }Bit;
//  }IntToBit;
//  
//  IntToBit data;
  
    for(i = 0; len - i >= 4; i++)  //寻找帧头及长度校验，长度要大于4
    {
        if((uartData[i]==0x55) && (uartData[i+1]==0xAA) && (uartData[i+2]==0x01))//帧头、长度位校验
        {
          switch(uartData[i+3])
          {
            case 0x11:
    //          data.int_data = (int)wat_fer.ventilate;
    //          data.Bit.bit0 = !data.Bit.bit0;
              wat_fer.ventilate = !wat_fer.ventilate;  //风机
            break;
          
            case 0x12:
    //          data.int_data = (int)wat_fer.ventilate;
    //          data.Bit.bit1 = !data.Bit.bit1;
              wat_fer.juanmo = !wat_fer.juanmo;  //卷膜
            break;
          
            case 0x13:
    //          data.int_data = (int)wat_fer.ventilate;
    //          data.Bit.bit2 = !data.Bit.bit2;
              wat_fer.wat4 = !wat_fer.wat4;  //水帘
            break;
            
            case 0x14:
    //          data.int_data = (int)wat_fer.ventilate;
    //          data.Bit.bit3 = !data.Bit.bit3;
              wat_fer.yang = !wat_fer.yang;  //内遮阳
            break;
            
            case 0x15:
    //          data.int_data = (int)wat_fer.ventilate;
    //          data.Bit.bit4 = !data.Bit.bit4;
              wat_fer.yang1 = !wat_fer.yang1;  //外遮阳
            break;
          }
          
          hmi_send();
        }
    }
}
