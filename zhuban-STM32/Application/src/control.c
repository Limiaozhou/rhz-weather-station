#include "control.h"
#include "sys.h"
#include "data.h"

char Fer_cmd;//水、肥命令  水：0，肥：1  
char flag=0;//状态标志位
int waite_time=0;//5分钟计时时间
extern EVNDAT Evndat;	//传感数据结构体
char high;
char low;
char Water_Level;//水位 为1表示低水位，FF表示高水位，0表示水位正常

void clean_Evndat()
{
	Evndat.Flowrate=0;
	Evndat.Flowrate_out=0;
	Evndat.Total_flow=0;
	Evndat.Total_flow_out=0;
}

void Init_control()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIOC->CRL &=~0X00FFFFFF; 
	GPIOC->CRL |= 0X00333333;
	
	GPIOA->CRL &=~0X00FF0000; 
	GPIOA->CRL |= 0X00440000;
	control_relay(0,1);
	control_relay(0,2);
	control_relay(0,3);
	control_relay(0,4);
	control_relay(0,5);
	control_relay(0,6);
	clean_Evndat();
}
//继电器控制命令，status:控制状态，0表示关，1表示开，Num：编号，1-6
void control_relay(unsigned char status, unsigned char Num)
{
	switch(status)
	{
		case 0:
			GPIOC->BRR=0X1<<(Num-1);
		break;
		case 1:
			GPIOC->BSRR=0X1<<(Num-1);
		break;
	}
}
char get_Cmd()
{
	return Fer_cmd;
}
void set_Cmd(char cmd)
{
	if(Fer_cmd!=cmd)flag=0;
	Fer_cmd=cmd;
}
char get_HighWLstatus()//获取高水位状态，返回1表示到达高水位
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
}
char get_LowWLstatus()
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
}
//启动施肥
unsigned char stir_flag=0;	//该标志位为1时，表示已达到 500L，2关，标志位为2时，表示5已开启3分钟了
void Begin_fer(void)
{
	switch(flag)
	{
		case 0:
		{
			clean_Evndat();
			control_relay(0,1);
			control_relay(1,2);
			control_relay(1,5);
			flag=1;
		}
		break;
		case 1:
		{
			if(stir_flag!=2 && waite_time++>=900)
			{
				control_relay(0,5);
				if(stir_flag==1) {flag=2;waite_time=0;control_relay(1,4);}
				stir_flag=2;
			}
			if(stir_flag!=1 && (Evndat.Total_flow>=500||Water_Level==-1))
			{
				clean_Evndat();
				control_relay(0,2);
				if(stir_flag==2) {flag=2;waite_time=0;control_relay(1,4);}
				stir_flag=1;
			}
		}
		break;
		case 2:
		{
			if(waite_time++>=1500)	//5分钟
			{
				control_relay(1,6);
				control_relay(1,3);
				flag=3;
				waite_time=0;
			}
		}
		break;
		case 3:
		{
			if(Evndat.Total_flow_out>=500||Water_Level==1)
			{
				control_relay(1,2);
				control_relay(0,6);
				control_relay(0,3);
				flag=4;
				clean_Evndat();
			}
		}
		break;
		case 4:
		{
			if(Evndat.Total_flow>=500||Water_Level==-1)
			{
				control_relay(0,2);
				control_relay(1,6);
				control_relay(1,3);
				flag=5;
				clean_Evndat();
			}
		}
		break;
		case 5:
		{
			if(Evndat.Total_flow_out>=400||Water_Level==1)
			{
				Close_fer();
				clean_Evndat();
				flag=0;
			}
		}
		break;
	}
}
	
//关闭施肥
void Close_fer(void)
{
	Fer_cmd=0;
	control_relay(0,2);
	control_relay(0,3);
	control_relay(0,6);
	control_relay(0,4);
	control_relay(0,5);
	flag = 0;
	clean_Evndat();
}
	
void Protect(void)
{
	high=get_HighWLstatus();
	low=get_LowWLstatus();
	if(low==0) {control_relay(0,6);Water_Level=1;}
	else if(high==0)  {control_relay(0,2);Water_Level=-1;}
	else Water_Level=0;
}
char get_WL(void)
{
	return Water_Level;
}
