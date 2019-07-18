#include "hmi.h"

void hmi_send(void)  //·¢ËÍÆÁÄ»Êý¾Ý
{
    unsigned char buf[15] = {0};
    unsigned char end_buf[3] = {0xFF, 0xFF, 0xFF};
    int data = 0;
    unsigned char len = 0;
    
    data = (int)(sensor.temp * 100);
    len = sprintf((char *)buf, "x0.val=%d", data);
    USART_Puts(USART2, buf, len);
    USART_Puts(USART2, end_buf, 3);
    delay_ms(100);
    
    data = (int)(sensor.humi * 100);
    len = sprintf((char *)buf, "x1.val=%d", data);
    USART_Puts(USART2, buf, len);
    USART_Puts(USART2, end_buf, 3);
    delay_ms(100);
    
    data = (int)(sensor.light * 100);
    len = sprintf((char *)buf, "x2.val=%d", data);
    USART_Puts(USART2, buf, len);
    USART_Puts(USART2, end_buf, 3);
}
