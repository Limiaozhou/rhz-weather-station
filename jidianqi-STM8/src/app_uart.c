#include "app_uart.h"
//#include "bsp_time.h"

#include "control.h"
#include "485protocol.h"

//static unsigned long int time_uart_send, time_uart_send1;
//static unsigned char time_uartsend_num, time_uartsend_num1;

/* uart1��ȡ���ݴ��� pdata������ָ�룬 len�����ݳ��� */
void uart1_read_deal(unsigned char *pdata, unsigned char len)
{
	unsigned char i;
//	unsigned char tdata[20];
    int data = 0;
    
    for(i = 0; len - i >= 30; i++)  //�жϲ������Դ����������ݣ�5���豸Ϊ30�ֽ�
    {
        if((pdata[i]==0x66) && (pdata[i+1]==0xBB) && (pdata[i+2]>5))//֡ͷУ�飬����У�����2
        {
            if((pdata[i+3] == 0x00) && (pdata[i+4] == 0x01))  //�豸1
            {
                data = (int)chartofloat(pdata + i + 5);
                if(data & 0x01)
                    valve1 = 1;
                else
                    valve1 = 0;
            }
            if((pdata[i+9] == 0x00) && (pdata[i+10] == 0x03))  //�豸2
            {
                data = (int)chartofloat(pdata + i + 11);
                if(data & 0x01)
                    valve2 = 1;
                else
                    valve2 = 0;
            }
            if((pdata[i+15] == 0x00) && (pdata[i+16] == 0x05))  //�豸3
            {
                data = (int)chartofloat(pdata + i + 17);
                if(data & 0x01)
                    valve3 = 1;
                else
                    valve3 = 0;
            }
            if((pdata[i+21] == 0x00) && (pdata[i+22] == 0x07))  //�豸4
            {
                data = (int)chartofloat(pdata + i + 23);
                if(data & 0x01)
                    valve4 = 1;
                else
                    valve4 = 0;
            }
            if((pdata[i+27] == 0x00) && (pdata[i+28] == 0x09))  //�豸5
            {
                data = (int)chartofloat(pdata + i + 29);
                if(data & 0x01)
                    valve5 = 1;
                else
                    valve5 = 0;
            }
        }
    }
	
//	if((pdata[0] == 0x66) && (pdata[1] == 0x06) && (pdata[2] == (len - 4)) && (pdata[len - 1] == 0x060))
//	{  //66 06 01 00 60��01Ϊ���ݳ��ȣ�00Ϊ���ݣ�˳�����
//		for(i = 0; i < len; i++)
//		{
//			tdata[i] = pdata[i];
//		}
//		
//		uart_write(UART1_Select, tdata, len);
//	}
//	else if((pdata[0] == 0x77) && (pdata[1] == 0x07) && (pdata[2] == (len - 4)) && (pdata[len - 1] == 0x070))
//	{  //77 07 01 00 70��01Ϊ���ݳ��ȣ�00Ϊ���ݣ��������
//		for(i = 0; i < len; i++)
//			tdata[i] = pdata[len - 1 - i];
//		
//		uart_write(UART1_Select, tdata, len);
//	}
}

//��ʱ���ͺ�����ʼ����ע�ᶨʱ��
//void uart1_send_ontime_int(void)
//{
//    time_uartsend_num = time_register(&time_uart_send);
//    time_start(time_uartsend_num);
//    time_uartsend_num1 = time_register(&time_uart_send1);
//}

//��ʱ���ͺ���
//void uart1_send_ontime(void)
//{
//    unsigned char tx_data[5] = {0x11, 0x02, 0x0B, 0x01, 0xE1};  //��ȡ��������
//    unsigned char tx_data1[6] = {0x11, 0x03, 0x0C, 0x02, 0x1E, 0xC0};  //�򿪷�������
//    static unsigned char count = 0;
//    
//    if(time_uart_send >= 1000)  //ÿ���ȡ����һ��
//    {
//        time_clean(time_uartsend_num);
//        uart_write(UART1_Select, tx_data, 5);  //��ȡ����
//        time_start(time_uartsend_num);
//        if(++count >= 10)  //��ȡ10�����ݺ󣬸�500ms�򿪷���һ��
//        {
//            count = 0;
//            time_start(time_uartsend_num1);
//        }
//    }
//    
//    if(time_uart_send1 >= 500)
//    {
//        time_clean(time_uartsend_num1);
//        uart_write(UART1_Select, tx_data1, 6);  //�򿪷���
//    }
//}
