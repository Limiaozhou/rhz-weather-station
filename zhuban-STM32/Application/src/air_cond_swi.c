#include "stm32f10x.h"
#include "sys.h"
#include "air_cond_swi.h"
#include "data.h"
#include "mem.h"
#include "rs485_deal.h"
#include "usart.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//加热制冷及除湿开关控制函数		   //技术论坛:www.openedv.com
//修改日期:2016/9/14
//STM32F103RCT6芯片
//版本：V1.0
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
/******实现的功能：
除湿机设置室外湿度》55%  一直工作。
除湿机电源控制流程   
室外湿度》70% 一直工作
65%》室内湿度》55%  工作2小时停止0.5小时

空调机恒温度范围 10~30度 
空调机电源控制流程
室内温度》40度 电源  一直工作
40度》室内温度》30度 电源  工作0.5小时停止0.5小时
10度》室内温度》-10度 电源  工作0.5小时停止0.5小时
室内温度《-10度 电源  一直工作

****************/
////////////////////////////////////////////////////////////////////////////////// 	

extern EVNDAT Evndat;

/***************************************
*@DESCRIPTION: --空调及除湿器开关初始化    
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
//加热器开
void heater_open()
{
	HUMIDITY_OUT=1;//用除湿代替加热
	//HEATER_OUT=1;
}
//加热器关4
void heater_close()
{
	HUMIDITY_OUT=0;//用除湿代替加热
	//	HEATER_OUT=0;
}
//制冷加热装置控制

int air_con_flag =0;
int humi_con_flag =0;
/***************************************
*@DESCRIPTION: --温度控制函数
*
*@Input: --温度
*
*@Out: --无
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
//	if(value>=40.0)//40度以上常开制冷
//	{
//		air_con_time30=0;
//		air_con_time10=0;
////		heater_close();//关闭加热
////                USART_Puts(USART3,humi_close_cmd,8);
////                 delay_ms(100);
////		air_con_open();//开启制冷
//                USART_Puts(USART3,air_open_cmd,8);
//	}
//	else if((value>=30.0)&&(value<40.0))//温度在40和30之间加热制冷交替半小时开
//	{
//		air_con_time10=0;//清空10度一下时的计数器防止持续计数
////		heater_close();//关闭加热
//                   USART_Puts(USART3,humi_close_cmd,8);
//		if(air_con_time30>=9000)//9000设置半小时开关交替
////                if(air_con_time30>=300)
//		{
//			air_con_time30=0;
////			AIR_CON_OUT=!AIR_CON_OUT;//切换制冷开关状态
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
//	else if ((value<30.0)&&(value>10.0))//?温度在30度和10度之间关闭加热制冷
//	{
//		air_con_time30=0;
//                air_con_time10=0;
////		heater_close();//关闭加热器
////                USART_Puts(USART3,humi_close_cmd,8);
////                delay_ms(100);
////		air_con_close();//关闭制冷装置
//                 USART_Puts(USART3,air_close_cmd,8);
//	}
//	else if((value<=10.0)&&(value>-10.0))//温度在10度到-10度之间加热器半小时开半小时关交替
//	{
//		air_con_time30=0;
////		air_con_close();//关闭制冷装置
//                USART_Puts(USART3,air_close_cmd,8);
//		if(air_con_time10>9000)//9000设置为半小时时间交替
////                  if(air_con_time30>=300)
//                
//		{
//			air_con_time10=0;
////			HEATER_OUT=!HEATER_OUT;//切换加热开关状态
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
//		else if(value<=-10.0)//-10度一下将常开加热器
//		{
//		air_con_time30=0;
//                air_con_time10=0;
////		heater_open();//加热器一直打开
//                USART_Puts(USART3,humi_open_cmd,8);
//                delay_ms(100);
////		air_con_close();//制冷一直关闭
//                USART_Puts(USART3,air_close_cmd,8);
//		}
	return 0;
}

/***************************************
*@DESCRIPTION: --除湿器控制 
*
*@Input: --室内和室外温度value_in：室内温度，value_out：室外温度

*
*@Out: --无
*
*@Return: --int
****************************************/	
//char humidity_flag55=0;//用来控制是否开启大于55便常开除湿器的功能

int humidity_control(float value_in,float value_out)
{
	static char open_flag=1,close_flag=0;//上次的开关状态记录标志
//	if(value_out>55.0)//室外湿度超过70%将常开除湿器，需将标志位置1
//	{
//		humidity_time=0;
//
//                USART_Puts(USART3,humi_open_cmd,8);
//	}
	if((value_out>=70.0)||(value_in>=65.0))//室外湿度大于70%室内大于65%常开除湿器
	{
		humidity_time=0;
//		humidity_open();//常开除湿器
                USART_Puts(USART3,humi_open_cmd,8);
	}
       else if((value_in<=65.0)&&(value_in>=55.0))//湿度在65%和55%之间时开2小时关半小时交替
	{
		if((open_flag==1)&&(humidity_time>9000))//9000半个小时后开启
//                if((open_flag==1)&&(humidity_time>300))
		{
			humidity_time=0;
			open_flag=0;
			close_flag=1;
//			humidity_open();//开启除湿器
                        USART_Puts(USART3,humi_open_cmd,8);
		}
		else if((close_flag==1)&&(humidity_time>36000))//36000两小时后开启
//                  else if((close_flag==1)&&(humidity_time>600))
		{
			humidity_time=0;
			open_flag=1;
			close_flag=0;
//			humidity_close();//关闭除湿器
                        USART_Puts(USART3,humi_close_cmd,8);
		}
	}
	else if(value_in<55.0)//湿度在55%之下关闭除湿器
	{
			humidity_time=0;
//			humidity_close();//关闭除湿器
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