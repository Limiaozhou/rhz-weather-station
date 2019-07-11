#include "stm32f10x.h"
#include "sys.h"
#include "air_cond_swi.h"
#include "data.h"
#include "mem.h"
#include "rs485_deal.h"
#include "usart.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//�������估��ʪ���ؿ��ƺ���		   //������̳:www.openedv.com
//�޸�����:2016/9/14
//STM32F103RCT6оƬ
//�汾��V1.0
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
/******ʵ�ֵĹ��ܣ�
��ʪ����������ʪ�ȡ�55%  һֱ������
��ʪ����Դ��������   
����ʪ�ȡ�70% һֱ����
65%������ʪ�ȡ�55%  ����2Сʱֹͣ0.5Сʱ

�յ������¶ȷ�Χ 10~30�� 
�յ�����Դ��������
�����¶ȡ�40�� ��Դ  һֱ����
40�ȡ������¶ȡ�30�� ��Դ  ����0.5Сʱֹͣ0.5Сʱ
10�ȡ������¶ȡ�-10�� ��Դ  ����0.5Сʱֹͣ0.5Сʱ
�����¶ȡ�-10�� ��Դ  һֱ����

****************/
////////////////////////////////////////////////////////////////////////////////// 	

extern EVNDAT Evndat;

/***************************************
*@DESCRIPTION: --�յ�����ʪ�����س�ʼ��    
*
*@Input: --?T
*
*@Out: --?T
*
*@Return: --void
****************************************/	
unsigned char air_open_cmd[8]={0x6A,0x03,0x00,0x76,0x00,0x01,0x6D,0x0B};
unsigned char air_close_cmd[8]={0x6A,0x03,0x00,0x76,0x00,0x00,0xAC,0xCB};
unsigned char humi_open_cmd[8]={0x6A,0x03,0x00,0x78,0x00,0x01,0x0C,0xC8};
unsigned char humi_close_cmd[8]={0x6A,0x03,0x00,0x78,0x00,0x00,0xCD,0x08};

void air_con_dehu_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
        GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
        
}
void air_con_open()
{
	AIR_CON_OUT=1;
}

void air_con_close()
{
	AIR_CON_OUT=0;
}

void humidity_open()
{
	HUMIDITY_OUT=1;
}

void humidity_close()
{
	HUMIDITY_OUT=0;
}
//��������
void heater_open()
{
	HUMIDITY_OUT=1;//�ó�ʪ�������
	//HEATER_OUT=1;
}
//��������4
void heater_close()
{
	HUMIDITY_OUT=0;//�ó�ʪ�������
	//	HEATER_OUT=0;
}
//�������װ�ÿ���

int air_con_flag =0;
int humi_con_flag =0;
/***************************************
*@DESCRIPTION: --�¶ȿ��ƺ���
*
*@Input: --�¶�
*
*@Out: --��
*
*@Return: --int
****************************************/	
int air_con_control(float value)
{
  if(value == 1)
  {
    USART_Puts(USART3,air_open_cmd,8);
  
  }
  if(value == 0)
  {
     USART_Puts(USART3,air_close_cmd,8);
  }
//	if(value>=40.0)//40�����ϳ�������
//	{
//		air_con_time30=0;
//		air_con_time10=0;
////		heater_close();//�رռ���
////                USART_Puts(USART3,humi_close_cmd,8);
////                 delay_ms(100);
////		air_con_open();//��������
//                USART_Puts(USART3,air_open_cmd,8);
//	}
//	else if((value>=30.0)&&(value<40.0))//�¶���40��30֮��������佻���Сʱ��
//	{
//		air_con_time10=0;//���10��һ��ʱ�ļ�������ֹ��������
////		heater_close();//�رռ���
//                   USART_Puts(USART3,humi_close_cmd,8);
//		if(air_con_time30>=9000)//9000���ð�Сʱ���ؽ���
////                if(air_con_time30>=300)
//		{
//			air_con_time30=0;
////			AIR_CON_OUT=!AIR_CON_OUT;//�л����俪��״̬
//                        air_con_flag=!air_con_flag;
//                        if(air_con_flag)
//                        {
//                            USART_Puts(USART3,air_open_cmd,8);
//                        }
//                        if(!air_con_flag)
//                        {
//                            USART_Puts(USART3,air_close_cmd,8);
//                        }
//                        
//		}
//	}
//	else if ((value<30.0)&&(value>10.0))//?�¶���30�Ⱥ�10��֮��رռ�������
//	{
//		air_con_time30=0;
//                air_con_time10=0;
////		heater_close();//�رռ�����
////                USART_Puts(USART3,humi_close_cmd,8);
////                delay_ms(100);
////		air_con_close();//�ر�����װ��
//                 USART_Puts(USART3,air_close_cmd,8);
//	}
//	else if((value<=10.0)&&(value>-10.0))//�¶���10�ȵ�-10��֮���������Сʱ����Сʱ�ؽ���
//	{
//		air_con_time30=0;
////		air_con_close();//�ر�����װ��
//                USART_Puts(USART3,air_close_cmd,8);
//		if(air_con_time10>9000)//9000����Ϊ��Сʱʱ�佻��
////                  if(air_con_time30>=300)
//                
//		{
//			air_con_time10=0;
////			HEATER_OUT=!HEATER_OUT;//�л����ȿ���״̬
//                        humi_con_flag =!humi_con_flag;
//                        if(humi_con_flag)
//                        {
//                           USART_Puts(USART3,humi_open_cmd,8);
//                        }
//                        if(!humi_con_flag)
//                        {
//                           USART_Puts(USART3,humi_close_cmd,8);
//                        }
//		}
//	}
//		else if(value<=-10.0)//-10��һ�½�����������
//		{
//		air_con_time30=0;
//                air_con_time10=0;
////		heater_open();//������һֱ��
//                USART_Puts(USART3,humi_open_cmd,8);
//                delay_ms(100);
////		air_con_close();//����һֱ�ر�
//                USART_Puts(USART3,air_close_cmd,8);
//		}
	return 0;
}

/***************************************
*@DESCRIPTION: --��ʪ������ 
*
*@Input: --���ں������¶�value_in�������¶ȣ�value_out�������¶�

*
*@Out: --��
*
*@Return: --int
****************************************/	
//char humidity_flag55=0;//���������Ƿ�������55�㳣����ʪ���Ĺ���

int humidity_control(float value_in,float value_out)
{
	static char open_flag=1,close_flag=0;//�ϴεĿ���״̬��¼��־
//	if(value_out>55.0)//����ʪ�ȳ���70%��������ʪ�����轫��־λ��1
//	{
//		humidity_time=0;
//
//                USART_Puts(USART3,humi_open_cmd,8);
//	}
	if((value_out>=70.0)||(value_in>=65.0))//����ʪ�ȴ���70%���ڴ���65%������ʪ��
	{
		humidity_time=0;
//		humidity_open();//������ʪ��
                USART_Puts(USART3,humi_open_cmd,8);
	}
       else if((value_in<=65.0)&&(value_in>=55.0))//ʪ����65%��55%֮��ʱ��2Сʱ�ذ�Сʱ����
	{
		if((open_flag==1)&&(humidity_time>9000))//9000���Сʱ����
//                if((open_flag==1)&&(humidity_time>300))
		{
			humidity_time=0;
			open_flag=0;
			close_flag=1;
//			humidity_open();//������ʪ��
                        USART_Puts(USART3,humi_open_cmd,8);
		}
		else if((close_flag==1)&&(humidity_time>36000))//36000��Сʱ����
//                  else if((close_flag==1)&&(humidity_time>600))
		{
			humidity_time=0;
			open_flag=1;
			close_flag=0;
//			humidity_close();//�رճ�ʪ��
                        USART_Puts(USART3,humi_close_cmd,8);
		}
	}
	else if(value_in<55.0)//ʪ����55%֮�¹رճ�ʪ��
	{
			humidity_time=0;
//			humidity_close();//�رճ�ʪ��
                        USART_Puts(USART3,humi_close_cmd,8);
	}
	return 0;
}

int temp_humi_cont(void)
{
		//float humi_out=0;
		//mem_copy((unsigned char *)&humi_out,psenddat->data.humi.dat,4);
		air_con_control(Evndat.temp_in);
                delay_ms(200);
		humidity_control(Evndat.humi_in,Evndat.humi);
	return 0;
}