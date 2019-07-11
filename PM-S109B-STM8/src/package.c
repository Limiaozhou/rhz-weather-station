//#include"iostm8s103K3.h"
//#include"iostm8s103F3.h"
#include"UART.h"
#include"typedef.h"
#include"config.h"

 unsigned char read_pm25[5]={0x11,0x02,0x0b,0x01,0xe1};

void Package_Deal(void)
{   uchar buf[20];
    uchar len=0;
    float dat = 0;
    unsigned char *pdat;

    len = UART_Receive(buf);
       pdat = &buf[0];
       while (len > 0)
       {
          switch (*pdat)
          { 
            case 0xC0:
            case 0xC2:
                 dat = *(pdat+1) + *(pdat+2) * 0.01;
                 Config_Add(dat);
                 if(len>=3)
                 {
                    pdat +=3;
                    len -=3;
                 }
                 else len = 0;
                 break;
            case 0xC1:
                 pdat++;
                 len--;
                 break;
            /*case 0xC2:
                 dat = *(pdat+1) + *(pdat+2) * 0.01;
                 //add
                 if(len>=3)
                 {
                    pdat +=3;
                    len -=3;
                 }
                 else len = 0;
                 break;*/
            case 0xC3:
                 Config_Send();
                 break;
            case 0xC4:
                 pdat++;
                 len--;
                 break;
            default:
                 pdat++;
                 len--;
                 break;
          }
       }
    
}
