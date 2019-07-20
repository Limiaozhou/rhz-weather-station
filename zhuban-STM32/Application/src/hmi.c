#include "hmi.h"

void hmi_send(void)  //·¢ËÍÆÁÄ»Êý¾Ý
{
    unsigned char buf[25] = {0};
    unsigned char end_buf[3] = {0xFF, 0xFF, 0xFF};
    int data = 0;
    unsigned char len = 0;
    
    data = (int)(sensor.temp * 100);
    len = sprintf((char *)buf, "main.x0.val=%d", data);
    USART_Puts(USART2, buf, len);
    USART_Puts(USART2, end_buf, 3);
    delay_ms(100);
    
    data = (int)(sensor.humi * 100);
    len = sprintf((char *)buf, "main.x1.val=%d", data);
    USART_Puts(USART2, buf, len);
    USART_Puts(USART2, end_buf, 3);
    delay_ms(100);
    
    data = (int)(sensor.light * 100);
    len = sprintf((char *)buf, "main.x2.val=%d", data);
    USART_Puts(USART2, buf, len);
    USART_Puts(USART2, end_buf, 3);
    
    data = (int)wat_fer.ventilate;
    if(data & (0x01 << 0))
    {
      len = sprintf((char *)buf, "fengji.b0.bco=1024");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    else
    {
      len = sprintf((char *)buf, "fengji.b0.bco=34815");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    if(data & (0x01 << 1))
    {
      len = sprintf((char *)buf, "fengji.b1.bco=1024");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    else
    {
      len = sprintf((char *)buf, "fengji.b1.bco=34815");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    if(data & (0x01 << 2))
    {
      len = sprintf((char *)buf, "fengji.b2.bco=1024");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    else
    {
      len = sprintf((char *)buf, "fengji.b2.bco=34815");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    if(data & (0x01 << 3))
    {
      len = sprintf((char *)buf, "fengji.b3.bco=1024");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    else
    {
      len = sprintf((char *)buf, "fengji.b3.bco=34815");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    if(data & (0x01 << 4))
    {
      len = sprintf((char *)buf, "fengji.b4.bco=1024");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
    else
    {
      len = sprintf((char *)buf, "fengji.b4.bco=34815");
      USART_Puts(USART2, buf, len);
      USART_Puts(USART2, end_buf, 3);
    }
}

void hmi_read(unsigned char *uartData,unsigned char len)
{
  typedef union
  {
    int int_data;
    struct
    {
      int bit0:1;
      int bit1:1;
      int bit2:1;
      int bit3:1;
      int bit4:1;
      int bit5:1;
      int bit6:1;
      int bit7:1;
      
      int bit8:1;
      int bit9:1;
      int bit10:1;
      int bit11:1;
      int bit12:1;
      int bit13:1;
      int bit14:1;
      int bit15:1;
      
      int bit16:1;
      int bit17:1;
      int bit18:1;
      int bit19:1;
      int bit20:1;
      int bit21:1;
      int bit22:1;
      int bit23:1;
      
      int bit24:1;
      int bit25:1;
      int bit26:1;
      int bit27:1;
      int bit28:1;
      int bit29:1;
      int bit30:1;
      int bit31:1;
    }Bit;
  }IntToBit;
  
  IntToBit data;
  
  switch(*uartData)
  {
    case 0x11:
      data.int_data = (int)wat_fer.ventilate;
      data.Bit.bit0 = !data.Bit.bit0;
      wat_fer.ventilate = data.int_data;
    break;
  
    case 0x12:
      data.int_data = (int)wat_fer.ventilate;
      data.Bit.bit1 = !data.Bit.bit1;
      wat_fer.ventilate = data.int_data;
    break;
  
    case 0x13:
      data.int_data = (int)wat_fer.ventilate;
      data.Bit.bit2 = !data.Bit.bit2;
      wat_fer.ventilate = data.int_data;
    break;
    
    case 0x14:
      data.int_data = (int)wat_fer.ventilate;
      data.Bit.bit3 = !data.Bit.bit3;
      wat_fer.ventilate = data.int_data;
    break;
    
    case 0x15:
      data.int_data = (int)wat_fer.ventilate;
      data.Bit.bit4 = !data.Bit.bit4;
      wat_fer.ventilate = data.int_data;
    break;
  }
  
  hmi_send();
}
