#include "tea.h"
#include "check.h"
#include "mem.h"
#include "delay.h"
#include "formatconversion.h"
#include "rs485_deal.h"
#include "usart.h"
#include "data.h"
#include "update.h"
#include "malloc.h"
#include "stdlib.h"
#include "wdg.h"


#define NUM 3
extern INFO_485 *Flash_485;


RTU_data rtu_data;
Send_485 send_485;
Cmd_send send_data;


unsigned char num_count; //send_cmd每秒触发一次，该参数在send_cmd里做累加操作，在rtu_num_plus中重新赋值为0.累加到3的时候，rtu_data.num自动加1
ST_ITEM *st_item;
EVNDAT Evndat;	//

unsigned char sed_485_cmd[8];
unsigned char send_485_error[6];
const unsigned char send_code[NUM]={FS_485, FX_485, yuxue_485};
 
//RxDat rx_data;
static char send_num=0; //发送485命令的进度
static char send_num_total=0; //所接485的个数

int Num_Rea_485=0;



/***************************************
*@DESCRIPTION: --打包RTU协议
*
*@Input: --send: 打包的数据
*        --Oper_code:操作码，一般来说是03
*        --addr: 地址
*        --Reg_addr：寄存器地址
*        --len 要读的长度
*@Out: --无
*
*@Return: --void
****************************************/	
//void send_read_RTU(unsigned char Oper_code, unsigned char addr,unsigned char Reg_addr[],unsigned char len)
//{
//  mem_clear(sed_485_cmd,8);
//  *sed_485_cmd=addr;
//  *(sed_485_cmd+1)=Oper_code;
//  *(sed_485_cmd+2)=Reg_addr[0];
//  *(sed_485_cmd+3)=Reg_addr[1];
//  
//  *(sed_485_cmd+4)=0x00;
//  *(sed_485_cmd+5)=len;
//  
//  Check_CalaCRC16(sed_485_cmd,6);
//}
//发送读485指令Address + Functioncode (1byte)+寄存器地址(2byte)+
void send_read_RTU(unsigned char addr,  unsigned char Functioncode, unsigned char Reg_addr, unsigned char len, unsigned char *send )
{
	*send = addr;
	*(send + 1) = Functioncode;
	*(send + 2) = 0x00;
	*(send + 3) = Reg_addr;
	*(send + 4) = 0x00;
	*(send + 5) = len;

	Check_CalaCRC16(send, 6);
}
/****************************************
@DESCRIPTION: --获取传输关键字
*
*@Input: --i:若同一个地址里有多个寄存器要读，i表示所读寄存器的个数
*        --cnt: send_485.format_485编号
*
*@Out: --无
*
*@Return: --void
****************************************/
unsigned char addr[2];
void get_485_key(unsigned char cnt,unsigned char i)
{
//  unsigned char tmp[2];
  if(send_485.format_485[cnt].addr<100)
  {
    addr[0]=0;
    addr[1]=send_485.format_485[cnt].addr;
  }
  else 
  {
    mem_copy(addr,(unsigned char*)&send_485.format_485[cnt].Reg_addr[i*2],2);
  }
}


/***************************************
*@DESCRIPTION: --生成485地址和和要读数据的个数
*       对每个地址的485寄存器排序
*       若寄存器间距大于20，则需再发一条命令
*
*@Input: --无
*
*@Out: --无
*
*@Return: --void
****************************************/	

void Generate_485_addr()
{
  int i=0,j=0,k=0;
  send_485.format_485[0].addr=Flash_485->addr_485;
  mem_copy(send_485.format_485[0].Reg_addr,(Flash_485)->regi_485,2);
  send_485.num=1;
  send_485.format_485[0].count=1;
  
  //将要发送的485命令存储至send_485结构体中，包括总共有的不同地址个数，每个地址要读的个数
  for(i=0;i<Num_Rea_485;i++)
  {
    if((unsigned char*)(Flash_485+i)->addr_485==0)
      break;
    for(j=0;j<i;j++)
    {
      if(!send_485.format_485[j].addr) continue;
      if((Flash_485+i)->addr_485==send_485.format_485[j].addr)
      {
        send_485.format_485[j].count++;
        mem_copy(&send_485.format_485[send_485.num-1].Reg_addr[(send_485.format_485[j].count-1)*2],(Flash_485+i)->regi_485,2);
      }
      else if(j==send_485.num-1)
      {
        send_485.format_485[send_485.num].addr=(Flash_485+i)->addr_485;
        mem_copy(send_485.format_485[send_485.num].Reg_addr,(Flash_485+i)->regi_485,2);
        send_485.format_485[send_485.num].count=1;
        send_485.num++;
        i++;
      }
    }
  }  
//  send_485.num--;
  //当一个地址有多个寄存器需要读出时，按序排列，给出这些寄存器的起始地址，并且对地址跨度大的重新加在send_485中
  for(i=0;i<send_485.num;i++)
  {
    if(send_485.format_485[i].count>1)
    {
      for(j=0;j<send_485.format_485[i].count;j++)
      { 
        for(k=0; j+k < send_485.format_485[i].count - 1; k++)
        {
          if(chartoshort(&send_485.format_485[i].Reg_addr[k*2]) > chartoshort(&send_485.format_485[i].Reg_addr[(k+1)*2]))
          {
            unsigned char tmp[2];
            mem_copy(tmp,&send_485.format_485[i].Reg_addr[k*2],2);
            mem_copy(&send_485.format_485[i].Reg_addr[(k)*2],&send_485.format_485[i].Reg_addr[(k+1)*2],2);
            mem_copy(&send_485.format_485[i].Reg_addr[(k+1)*2],tmp,2);
          }
        }
      }
      for(j=0;j<send_485.format_485[i].count;j++)
      {
        k=send_485.format_485[i].count;
        while(k--)
        {
          if((chartoshort(&send_485.format_485[i].Reg_addr[(j+1)*2]) - chartoshort(&send_485.format_485[i].Reg_addr[j*2]))>30)
          {
            send_485.format_485[send_485.num].addr=send_485.format_485[i].addr;
            mem_copy(send_485.format_485[send_485.num].Reg_addr,&send_485.format_485[i].Reg_addr[j*2+2],2*(send_485.format_485[i].count-j));
            send_485.format_485[send_485.num].count=send_485.format_485[i].count-j-1;  
            mem_clear(&send_485.format_485[i].Reg_addr[j*2+2],2*send_485.format_485[i].count-2);
            send_485.format_485[i].count-=send_485.format_485[i].count-j-1; 
            send_485.num++;
          }
        }
      }
    }
  }
  //确认每个地址的连续长度和总个数
  for(i=0;i<send_485.num;i++)
  {
    send_485.cnt+=send_485.format_485[i].count;
    for(j=0;j<send_485.format_485[i].count;j++)
    {
      if(send_485.format_485[i].count==1)
      { 
        send_485.format_485[i].len=1;
      }
      else
      {
        send_485.format_485[i].len = ((chartoshort(&send_485.format_485[i].Reg_addr[(send_485.format_485[i].count-1)*2]) - chartoshort(&send_485.format_485[i].Reg_addr[0]))/2+1);
      }
    }
  }
  st_item = (ST_ITEM *)mymalloc(SRAMIN,send_485.cnt*sizeof(ST_ITEM));
  k=0;
  for(i=0;i<send_485.num;i++)
  {
    for(j=0;j<send_485.format_485[i].count;j++)
    {
      get_485_key(i,j);
      mem_copy(st_item[k++].key,addr,2);
    }
  }
}

void rtu_num_plus()
{
  if(++send_num == NUM)
  {
    send_num=0;  
  }
   num_count=0;
}

//void rtu_num_plus()
//{
//  if(++send_num == send_485.num)
//  {
//    send_num=0;  
//  }
//  if(send_num_total==send_485.cnt)
//  {
//    send_num_total=0;
//  }
//  num_count=0;
//  //  mem_clear((unsigned char *)&rtu_data, 10);
//}

/****************************************
@DESCRIPTION: --两个字节的数据解析成浮点型再转成char类型
*
*@Input: --无
*
*@Out: --无
*
*@Return: --floag 传感器数据
****************************************/
float char2TofloatTochar4(unsigned char *pdat)
{
  float tmp;
  tmp=(*pdat)<<8;
  tmp+=*(pdat+1);
  return tmp;
}

/****************************************
@DESCRIPTION: --地址小于100时传感器采集按传感器格式过来
*
*@Input: --无
*
*@Out: --无
*
*@Return: --floag 传感器数据
****************************************/
//float get_rea_data(unsigned char *uartData)
//{
//  unsigned char ctmp[4];
//  float ftmp;
//  switch(rtu_data.addr)
//  {
//  case FX_485:
//    ftmp=char2TofloatTochar4(uartData);
//    break;
//  case FS_485:
//    ftmp=char2TofloatTochar4(uartData);
//    ftmp/=10;
//    break;
//  case LIGHT_485:
//    ftmp=char2TofloatTochar4(uartData);
//    ftmp*=100;
//    break;
//  case VOICE_485:
//    if(*uartData==0x21 && *(uartData+6)==0x24)
//    {
//      ftmp=atof(uartData+1);
//      if(ftmp)
//        ftmp+=0.005;
//    }
//    break;
//  case ULTRA_485:
//     ftmp=char2TofloatTochar4(uartData);
//    break;
//    
//  case SOILPH_485:       //土壤ph接收      oe
//    if(*uartData==0x0E && *(uartData+1)==0x03)
//    {
//      ftmp=char2TofloatTochar4(uartData + 3);
//      ftmp = ftmp / 100;
//    }
//    break;
// /*   
//  case SOILHT_485:       //温湿度      oe
//    if(*uartData==0x01 && *(uartData+1)==0x03)
//    {
//      ftmp = char2TofloatTochar4(uartData + 3);
//      ftmp = ftmp / 100;
//    }
//    break;
//   case SOILDIAN_485:       //电导率      oe
//    if(*uartData==0x01 && *(uartData+1)==0x03)
//    {
//      ftmp = char2TofloatTochar4(uartData + 3);
//      ftmp = ftmp / 100;
//    }
//    break;
//*/    
//    
//  default:
////    return uartData;
//    return 0;
//    break;
//  }
////  mem_copy(ctmp,(unsigned char*)&ftmp,4);
////  return ctmp;
//  return ftmp;
//}

/****************************************
@DESCRIPTION: --485 命令数据返回解释
*
*@Input: --无
*
*@Out: --无
*
*@Return: --floag
****************************************/
float LIGHT_DATA = 0;
int data = 0;
//static char shade_flag;
void get_RTU_data(unsigned char *uartData,unsigned char len)
{
  int i=0;

//  USART_Puts(UART4, (unsigned char *)uartData, len);//debug
//  USART_Puts(USART1, (unsigned char *)uartData, len);//debug
  while(i<len)
  {
    if (Check_CheckCRC16(uartData + i, *(uartData + i + 2) ))
    {
        if(len == (*(uartData + i + 2) ))
        {
          if (*(uartData + i + 4) == 0x80)
          {
            sensor.temp = chartofloat(uartData + i + 5);
          }
          if (*(uartData + i + 10) == 0x82)
          {
            sensor.humi = chartofloat(uartData + i + 11);
          }
          if (*(uartData + i + 16) == 0x06)
          {
            sensor.light = chartofloat(uartData + i + 17);
            if(sensor.light <= 0)  //超过量程数据会<=0
              sensor.light = 65000;
          }
          if (*(uartData + i + 22) == 0x7A)
          {
            sensor.co2 = chartofloat(uartData + i + 23);
          }
          if (*(uartData + i + 28) == 0x7C)
          {
            sensor.tvoc = chartofloat(uartData + i + 29);
          }
          if (*(uartData + i + 34) == 0x84)
          {
            sensor.pressure = chartofloat(uartData + i + 35);
          }
          if (*(uartData + i + 40) == 0x68)
          {
            sensor.pm25 = chartofloat(uartData + i + 41);
          }
        }
        
        break;
    }
    else if (Check_CheckCRC16(uartData + i, *(uartData + i + 2) + 3))
    {
      switch (rtu_data.addr)
      {
        case FS_485 :  //精迅风速，01 03 02 00 17 F8 4A
          if(len == (*(uartData + i + 2) + 5))  //长度校验
          {
            if((*(uartData + i + 0) == 0x01) && (*(uartData + i + 1) == 0x03))
            {  //头和功能码校验
              sensor.fs = *(uartData + i + 3) << 8;
              sensor.fs += *(uartData + i + 4);
              
              sensor.fs = sensor.fs / 10;
              
              rtu_num_plus();
            }
          }
        break;
        
        case FX_485 :  //精迅风向，02 03 02 00 0F BC 40
          if(len == (*(uartData + i + 2) + 5))  //长度校验
          {
            if((*(uartData + i + 0) == 0x02) && (*(uartData + i + 1) == 0x03))
            {  //头和功能码校验
              sensor.fx = *(uartData + i + 3) << 8;
              sensor.fx += *(uartData + i + 4);
              
              rtu_num_plus();
            }
          }
        break;
        
        case yuxue_485 :  //精迅雨雪，03 03 00 10 00 01 84 2D
          if(len == (*(uartData + i + 2) + 5))  //长度校验
          {
            if((*(uartData + i + 0) == 0x03) && (*(uartData + i + 1) == 0x03))
            {  //头和功能码校验
              sensor.yuxue = *(uartData + i + 3) << 8;
              sensor.yuxue += *(uartData + i + 4);
              
              rtu_num_plus();
            }
          }
        break;
        
//        case FS_485 :  //风速，01 03 02 00 26 39 9E
//          if(len == (*(uartData + i + 2) + 5))  //长度校验
//          {
//            if((*(uartData + i + 0) == 0x01) && (*(uartData + i + 1) == 0x03))
//            {  //头和功能码校验
//              sensor.fs = *(uartData + i + 3) << 8;
//              sensor.fs += *(uartData + i + 4);
//              
//              sensor.fs = sensor.fs / 10;
//              
//              rtu_num_plus();
//            }
//          }
//        break;
//        
//        case FX_485 :  //风向，02 03 02 00 26 7D 9E
//          if(len == (*(uartData + i + 2) + 5))  //长度校验
//          {
//            if((*(uartData + i + 0) == 0x02) && (*(uartData + i + 1) == 0x03))
//            {  //头和功能码校验
//              sensor.fx = *(uartData + i + 3) << 8;
//              sensor.fx += *(uartData + i + 4);
//              
//              rtu_num_plus();
//            }
//          }
//        break;
        
//        case SO2_485 :  //SO2，03 03 02 00 00 C1 84
//          if(len == (*(uartData + i + 2) + 5))
//          {
//            if((*(uartData + i + 0) == 0x03) && (*(uartData + i + 1) == 0x03))
//            {
//              sensor.so2 = *(uartData + i + 3) << 8;
//              sensor.so2 += *(uartData + i + 4);
//              if(!sensor.so2)
//                sensor.so2 = 1;
//              sensor.so2 /= 100;
//              
//              rtu_num_plus();
//            }
//          }
//        break;
        
//        case ULTRA_485 :  //紫外线，04 03 02 00 00 74 44
//          if(len == (*(uartData + i + 2) + 5))
//          {
//            if((*(uartData + i + 0) == 0x04) && (*(uartData + i + 1) == 0x03))
//            {
//              sensor.ultvio = *(uartData + i + 3) << 8;
//              sensor.ultvio += *(uartData + i + 4);
//              if(!sensor.ultvio)
//                sensor.ultvio = 0.1;
//              sensor.ultvio /= 10;
//              
//              rtu_num_plus();
//            }
//          }
//        break;
      }
      
      break;
    }

    i++;
  }
}
//end get_RTU_data


float error=0;
void send_Cmd()
{
  	unsigned char  len;
//    unsigned char num;
	rtu_data.addr = send_code[send_num];
//    USART_Puts(UART4, "UV", strlen("UV"));
	switch (rtu_data.addr)//服务器检测参数地址
	{
        case FS_485 :  //精迅风速，01 03 00 16 00 01 65 CE
          send_read_RTU(0x01, 0x03, 0x16, 0x01, send_data.cmd);
          len = 8;
        break;
        
        case FX_485 :  //精迅风向，02 03 00 17 00 01 34 3D
          send_read_RTU(0x02, 0x03, 0x17, 0x01, send_data.cmd);
          len = 8;
        break;
        
        case yuxue_485 :  //精迅雨雪，03 03 00 10 00 01 84 2D
          send_read_RTU(0x03, 0x03, 0x10, 0x01, send_data.cmd);
          len = 8;
        break;
        
//        case FS_485 :  //风速，01 03 00 00 00 01 84 0A
//          send_read_RTU(0x01, 0x03, 0x00, 0x01, send_data.cmd);
//          len = 8;
//        break;
//        
//        case FX_485 :  //风向，02 03 00 00 00 01 84 39
//          send_read_RTU(0x02, 0x03, 0x00, 0x01, send_data.cmd);
//          len = 8;
//        break;
//        
//        case SO2_485 :  //SO2，03 03 00 06 00 01 65 E9
//          send_read_RTU(0x03, 0x03, 0x06, 0x01, send_data.cmd);
//          len = 8;
//        break;
        
//        case ULTRA_485 :  //紫外线，04 03 00 08 00 01 05 9D
//          send_read_RTU(0x04, 0x03, 0x08, 0x01, send_data.cmd);
//          len = 8;
//        break;
        
//      	case CO2_485:    //二氧化碳
//        send_read_RTU(0x66, 0x03, 0x7A, 0x02, send_data.cmd);
//		len = 8;
//		break;
//                
//        case temp_485:   //温度 
//        send_read_RTU(0x66, 0x03, 0x80, 0x02, send_data.cmd);
//		len = 8;
//		break;
//                       
//        case humi_485:   //湿度 
//        send_read_RTU(0x66, 0x03, 0x82, 0x02, send_data.cmd);
//		len = 8;
//		break;
//                
//  
//        case light_485:    //光照
//        send_read_RTU(0x66, 0x03, 0x9A, 0x02, send_data.cmd);
//		len = 8;
//		break;
/*              
	case PM25_485:    
//		num = 2;
		send_read_RTU(0x66, 0x03, 0x68, 0x02, send_data.cmd);
		len = 8;
		break;
	case Ultraviolet:     //PH
//		num = 2;
		send_read_RTU(0x0A, 0x04, 0x00, 0x01, send_data.cmd);
		len = 8;
		break;
                */

	default:
		break;
	}

	if (++num_count > 2)  //没有回应就继续发一个命令，到3次
	{
//		rtu_data.cmd = 0xA2;
//		error = 10;
//		mem_copy(rtu_data.instant_data, (unsigned char *)&error, 4);
		//send_error_data();
		rtu_num_plus();  //改发送485命令，即send_num++，send_code数组循环
		// 返回错误码：-++
	}
//    while (!(UART4->SR & 0x80));//等待发送寄存器为空,(否则连续发送时数据易丢失 )
//		UART4->DR = num_count;//发送数据
	USART_Puts(USART3, send_data.cmd, len);
//    USART_Puts(UART4, send_data.cmd, len);  //debug
	delay_ms(100);

  
}
