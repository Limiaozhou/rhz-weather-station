#include "includes.h"

////////////////////////////////////////////////////////////////////////////////// 
//SPI��ʼ������ȡ����OPC-N2���ع��ܡ��������ݽ��͹��ܵ�
//������mfg
//�汾��V1.0
//�޸�����:2015/7/31
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 

#define CS     PAout(4)  // 
#define SCK    PAout(5)  // 
#define MISO   PAin(6)  // 
#define MOSI   PAout(7)  // 

uchar count = 0, open=0, okc = 0;
/***************************************
*@DESCRIPTION: --SPI ��ʼ��
*
*@Input: --��
*
*@Out: --��
*
*@Return: --��
****************************************/	

void SPI_Init(void)
{
	
  RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	
	   	 
	GPIOA->CRL&=0X0000FFFF; 
	GPIOA->CRL|=0X38330000;//PA4 5 7�������  6����
	     
}

/***************************************
*@DESCRIPTION: --SPI ��д
*
*@Input: --dat��д������
*
*@Out: --��ȡ����
*
*@Return: --��ȡ����
****************************************/	
uchar  SPI_WriteRead(uchar dat)
{
 	uchar i,ret=0;
	
	SCK = 0;
	delay_us(2);
	for(i=0;i<8;i++)
	{
		SCK = 1;
		delay_us(2);

	 	if((dat&0x80) == 0x80)  MOSI  = 1;
		else  		              MOSI  = 0;
		
    ret<<=1;  
    if(MISO)  ret |= 0x01; 
          
    delay_us(2);
		SCK = 0;
    delay_us(20);
		dat = dat << 1;

	}
	
	SCK = 0;
	delay_us(50);
  return ret;
}

/***************************************
*@DESCRIPTION: --SPI ���ֽڶ�д
*
*@Input: --pdat��д������ָ�� pret���������ݻ��� len:��д����
*
*@Out: --��ȡ����
*
*@Return: --��
****************************************/	
void  SPI_Writebuf(uchar *pdat, uchar *pret,uchar len)
{
   uchar i;
	 CS = 0;
	 delay_us(50);
   *pret++ = SPI_WriteRead(*pdat++);
	 delay_ms(3);
   for(i=0;i<len-1;i++)
		{	 
			  *pret++ = SPI_WriteRead(*pdat++);
		 }
	 delay_ms(5);
	 CS = 1;
}

/***************************************
*@DESCRIPTION: --SPI ���ֽڶ�д
*
*@Input: --type��д������ pret���������ݻ��� len:��д����
*
*@Out: --��ȡ����
*
*@Return: --��
****************************************/	
void SPI_Readbuf(uchar type,uchar *pret, uchar len)
{
  
    int i;
    
    CS = 0;
    delay_us(50);
    *pret++ = SPI_WriteRead(type);
    delay_ms(8);
		for(i=0;i<len-1;i++)
		{	 
			*pret++ = SPI_WriteRead(type);
			delay_us(150);
		}
    
		CS = 1;
		delay_ms(6);  
}
/***************************************
*@DESCRIPTION: -- OPC-N2���ص�Դ����
*
*@Input: --dat��д������ָ��
*
*@Out: -- ��
*
*@Return: --true false
****************************************/	

BOOL PM_Control(uchar dat)
{
   uchar rbuf[2];
   uchar tbuf[2];

   tbuf[0] = 0x03;
   tbuf[1] = dat;
   SPI_Writebuf(tbuf, rbuf,2);
   if((rbuf[0] == 0xF3) && (rbuf[1] == 0x03))
   {
      return true;
   }
	 else    return false;
}

/***************************************
*@DESCRIPTION: -- OPC-N2��ο��ص�Դ����
*
*@Input: --dat��д������ָ��
*
*@Out: -- ��
*
*@Return: --true false
****************************************/	
BOOL PM_DControl(uchar dat, uchar count)
{
	 uchar num = 0;
   do{
        if(PM_Control(0)) return true;;
      } while (num++ < count);
   
  return false;
}
//�����Ƿ���Ч�ж�
BOOL CheckisValid(uchar *pdat,uchar count)
{    uchar num = count;
	   uchar *p;
	   p = pdat;
	   while(num--)
    {
       if(*p != 0 && *p != 0x30) return true;
			 p++;
    }
		return false;
}
/***************************************
*@DESCRIPTION: -- OPC-N2��ζ�ȡ��������
*
*@Input: --dat��д������ָ��
*
*@Out: -- ��
*
*@Return: --true false
****************************************/	
BOOL PM_Read(uchar *pdat, uchar count)
{
	 uchar num = 0;
   do{
		    delay_ms(300);
        SPI_Readbuf(0x30,pdat, 63);
        if(pdat[0] == 0xF3 && CheckisValid((pdat+51),12)) return true;
      } while (num++ < count);
   
  return false;
}

/***************************************
*@DESCRIPTION: -- OPC-N2��ȡ��������
*
*@Input: --pm����������ָ��
*
*@Out: -- ���ز�������
*
*@Return: --true false
****************************************/	
BOOL Get_PM(ST_PM *pm)
{
	
	 uchar rbuf[65];
   uchar index = 0;
   /*
   if(PM_DControl(0, 10))
   {
        delay_ms(300);
		    
        if(PM_Read(rbuf, 15))
        {
          index =51;
				  pm->pm1 = *(float *)(&rbuf[index]);
          index +=4;
          pm->pm2_5 = *(float *)(&rbuf[index]);
          index +=4;
          pm->pm10 = *(float *)(&rbuf[index]); 
          if(PM_DControl(1, 10))  UART3_SendStr("��Դ�سɹ�\r\n", 12);
		      else UART3_SendStr("��Դ��ʧ��r\n", 12);				
          return true;					
				}
				else  UART3_SendStr("���ݶ�ȡʧ��\r\n", 14);
    }
	 else  UART3_SendStr("��Դ��ʧ��\r\n", 12);

	if(PM_DControl(1, 10))  UART3_SendStr("��Դ�سɹ�\r\n", 12);
  else UART3_SendStr("��Դ��ʧ��r\n", 12);
		*/
  return false;
	/*
   uchar rbuf[65];
   uchar index = 0;
   
	  
	
   if(PM_Control(0))
   {
		    open++;
        delay_ms(5000);
        SPI_Readbuf(0x30,rbuf, 63);
        if(rbuf[0] == 0xF3)
        {
          //index = 37;
          //pm->Temperature = *(float *)(&rbuf[index]);
          //index +=4;
          //pm->Pressure = *(float *)(&rbuf[index]);
					okc++;
          index =51;
				  pm->pm1 = *(float *)(&rbuf[index]);
          index +=4;
          pm->pm2_5 = *(float *)(&rbuf[index]);
          index +=4;
          pm->pm10 = *(float *)(&rbuf[index]);
          
          PM_Control(1);
					return true;
        
        }
        else PM_Control(1);

    }
	 else 	PM_Control(1);

	PM_Control(1);
  return false;
	*/
}


/***************************************
*@DESCRIPTION: -- OPC-N2������������
*
*@Input: --pm����������ָ��
*
*@Out: -- ���ز�������
*
*@Return: --true false
****************************************/	
BOOL Get_ContinuousPM(ST_PM *pm)
{
   uchar rbuf[65];
   uchar index = 0;
   
   SPI_Readbuf(0x30,rbuf, 63);
   if(rbuf[0] == 0xF3)
   {
      index =51;
		  pm->pm1 = *(float *)(&rbuf[index]);
      index +=4;
      pm->pm2_5 = *(float *)(&rbuf[index]);
      index +=4;
      pm->pm10 = *(float *)(&rbuf[index]);
          
			return true;
	 }
    return false;
	 
 }




uchar Rbuf[65];
uchar Tbuf[4];
ST_PM  St[50]; 
char buf[65],len=0;
//uchar count = 0, open=0, okc = 0;
float f =0;
void PM_test(void)
{
    ST_PM pm;
	   float *p;
    
    /*buf[0]=0x05;
	  buf[1]=0x04;
	  buf[2]=0x03;
	  buf[3]=0x02;
	  buf[4]=0x01;
	  buf[5]='\0';
	  reverse(buf);
    
    len  = inttochar(buf,5000);	
	  len = inttochar(buf,-5000);
	  len =inttochar(buf,123456);
	
	  len =floattochar(buf, 123.4568, 5);
	len =floattochar(buf, 0.4568, 2);
	len =floattochar(buf, -123.4568, 5);
	len =floattochar(buf, -0.4568, 5);
	len =floattochar(buf, 123.4568, 6);
	
	  len =doubletochar(buf, 123.456789, 3);
		len =doubletochar(buf, 0.456789, 3);
		len =doubletochar(buf, 1.456789, 5);
		len =doubletochar(buf, -123.456789, 7);
		len =doubletochar(buf, -0.456789, 3);
		*/
		/*PM_Control(0);
	
	  delay_ms(500);
	
		PM_Control(1);
	
	  delay_ms(500);
	*/
	  /*
		if(PM_DControl(1, 10))  UART3_SendStr("��Դ�سɹ�\r\n", 12);
		else UART3_SendStr("��Դ��ʧ��r\n", 12);
		
		if(PM_DControl(0, 10))  UART3_SendStr("��Դ���ɹ�\r\n", 12);
		else UART3_SendStr("��Դ��ʧ��r\n", 12);
		
		if(PM_DControl(0, 10))  UART3_SendStr("��Դ���ɹ�\r\n", 12);
		else UART3_SendStr("��Դ��ʧ��r\n", 12);
		
		if(PM_DControl(1, 10))  UART3_SendStr("��Դ�سɹ�\r\n", 12);
		else UART3_SendStr("��Դ��ʧ��r\n", 12);
		*/
	/*	
    if(Get_PM(&pm))   
		{
			 len = floattochar(buf, pm.pm1, 6);
			 UART3_SendStr("PM1: ", 5);
			 UART3_SendStr((uchar *)buf, len);
			 len = floattochar(buf, pm.pm2_5, 6);
			 UART3_SendStr("  PM2.5: ", 9);
			 UART3_SendStr((uchar *)buf, len);
			 len = floattochar(buf, pm.pm10, 6);
			 UART3_SendStr("  PM10: ", 8);
			 UART3_SendStr((uchar *)buf, len);
			 UART3_SendStr("\r\n", 2);
			 if(count<50)  St[count++]  = pm;
		}*/
   
    
		/*
   //����Դ����
   Tbuf[0] = 0x03;
   Tbuf[1] = 0x00;
   SPI_Writebuf(Tbuf, Rbuf,2);
	 
   
   //��������Ϣ
   SPI_Readbuf(0x3F,Rbuf, 60);
   
   SPI_Readbuf(0x3C,Rbuf, 62);
	 SPI_Readbuf(0x3C,Rbuf, 62);
	 SPI_Readbuf(0x3C,Rbuf, 62);
   
   //����Դ����
   Tbuf[0] = 0x03;
   Tbuf[1] = 0x00;
   //Tbuf[2] = 0x03;
   SPI_Writebuf(Tbuf, Rbuf,2);
   
   SPI_Readbuf(0x30,Rbuf, 63);
	 
	 SPI_Readbuf(0x30,Rbuf, 63);
	 SPI_Readbuf(0x30,Rbuf, 63);
	 SPI_Readbuf(0x30,Rbuf, 63);
	 SPI_Readbuf(0x30,Rbuf, 63);
	 
	  //�ص�Դ����
   Tbuf[0] = 0x03;
   Tbuf[1] = 0x01;
   //Tbuf[2] = 0x03;
   SPI_Writebuf(Tbuf, Rbuf,2);
   
   //��������Ϣ
   SPI_Readbuf(0x3F,Rbuf, 60);
   
   SPI_Readbuf(0x30,Rbuf, 62);
   SPI_Readbuf(0x30,Rbuf, 62);
	 */
  
}


