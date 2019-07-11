//#include "stm32f10x.h"
#include "data.h"
#include "usart.h"
#include "485protocol.h"
#include "mem.h"
#include "check.h"
#include "stmflash.h"

extern EVNDAT Evndat;	//传感数据结构体
extern char Flash_addr[2];
extern char Flash_bound[2];
unsigned char send_485_data[260];
int pro485(unsigned char *cmd)
{
	int i=0,j=0;
	int Num;
	char t=3;
	int data_cnt;
	int data;
	if((*cmd==Flash_addr[1]||*cmd==0)&&(Check_CheckCRC16(cmd, 6)))
	{
		switch(*(cmd+1))
		{
			case 0x03:
			*(send_485_data)=Flash_addr[1];
			*(send_485_data+1)=0x03;
			mem_copy(send_485_data+2,cmd+2,2);
			Num=*(cmd+2);
			Num<<=8;
			Num+=*(cmd+3);
			data_cnt=*(cmd+4);
			data_cnt<<=8;
			data_cnt+=*(cmd+5);
			*(send_485_data+2)=data_cnt*2;
			j=Num/2-1;
			if(!(Num%2)&&!(data_cnt%2)&&data_cnt<=(128-((Num-2)*2)))
			{	
				if(Num==0)
				{
						mem_copy(&send_485_data[3],(unsigned char *)&Flash_addr,2);
						mem_copy(&send_485_data[5],(unsigned char *)&Flash_bound,2);
					t=7;
					j=0;
				}
				
				for(;i<data_cnt/2;i++)
				{
					mem_copy(&send_485_data[t+4*i],((unsigned char*)((unsigned char*)(&Evndat.temp))+4*j++),4);
				}
				Check_CalaCRC16(send_485_data, data_cnt*2+3);
				USART_Puts(USART1,send_485_data,data_cnt*2+5);
			}
			break;
			
			case 0x06:
			Num=*(cmd+2);
			Num<<=8;
			Num+=*(cmd+3);

			data=*(cmd+4);
			data<<=8;
			data+=*(cmd+5);
			switch(Num)
			{
				case 0x00:
				Flash_addr[1]=data;
				STMFLASH_Write(FLASH_ADDR,(u16*)Flash_addr,1);
				*(send_485_data+2)=0x0;
				break;
				case 0x01:
				Flash_bound[1]=data;
				if(data<5)
				{
					STMFLASH_Write(FLASH_BOUND,(u16*)Flash_bound,1);
					*(send_485_data+2)=0x0;
				}
				else *(send_485_data+2)=0x1;
				break;
			}
			
	//		*(send_485_data+1)=0x0;
				*(send_485_data)=Flash_addr[1];
				*(send_485_data+1)=0x06;
		
			Check_CalaCRC16(send_485_data, 3);
			USART_Puts(USART1,send_485_data,5);
			
			break;
		}
	}
	return 0;
}	

