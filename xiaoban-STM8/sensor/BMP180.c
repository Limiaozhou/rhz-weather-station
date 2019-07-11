#include <math.h>
#include "delay.h" 
#include "bmp180.h"
#include "i2c.h"



#define SCL_H()  SCL=1
#define SCL_L()  SCL=0
#define SDA_H()  SDA=1
#define SDA_L()  SDA=0
 
//#define SDA  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)
 
 #define BMP180_SlaveAddr 0xee   //BMP180?????
//BMP180????
short int AC1;
short int AC2;
short int AC3;
unsigned short AC4;
unsigned short AC5;
unsigned short AC6;
short int B1;
short int B2;
short int MB;
short int MC;
short int MD;
 
//u8 BMP180_ID=0;          //BM P180?ID
float True_Temp=0;       //????,??:?
float True_Press=0;      //????,??:Pa
float True_Altitude=0;   //????,??:m
 
/*????IIC?????
 *SCL:PC1
 *SDA:PC2
*/

 void delay_us(short int sec)
 {
   delay_2us(sec*3);
 }
 
void IIC_Init(void)
{
    SCL_H();  //SCL = 1;
    delay_us(5);
    SDA_H();  //SDA = 1;
    delay_us(5);    
}
 
void IIC_Start(void)
{
    SDA_H();  //SDA = 1;
    delay_us(5);
    SCL_H();  //SCL = 1;
    delay_us(5);
    SDA_L();  //SDA = 0;
    delay_us(5);    
}
 
void IIC_Stop(void)
{
    SDA_L();   //SDA = 0;
    delay_us(5);
    SCL_H();   //SCL = 1;
    delay_us(5);
    SDA_H();   //SDA = 1;
    delay_us(5);
}
 
unsigned char IIC_ReceiveACK(void)
{
    unsigned char ACK;
 
    SDA_H();     //SDA=1;//???????????,??????????,???!
    SCL_H();     //SCL=1;
    delay_us(5);
 
    if (SDAIN==1)  //SDA??
    {
        ACK = 1;    
    }
    else ACK = 0;  //SDA??
 
 
    SCL_L();    //SCL = 0;//SCL?????SDA?????????,??????????? 
    delay_us(5);
     
    return ACK;                 
}
 
void IIC_SendACK(unsigned char ack)
{
    if (ack == 1)SDA_H();
    else if (ack == 0)SDA_L();
    //SDA = ack;
    SCL_H();   //SCL = 1;
    delay_us(5);
    SCL_L();   //SCL = 0;
    delay_us(5);
}
 
unsigned char IIC_SendByte(unsigned char dat)
{
    unsigned char i;
    unsigned char bResult=1;
     
    SCL_L();     //SCL = 0;//?????
    delay_us(5);        
 
    for( i=0;i<8;i++ ) //??SCK,?dat????????SDA?
    {
        if( (dat<<i)&0x80 )SDA_H();   //SDA = 1;//????
        else SDA_L();  //SDA = 0;
        delay_us(5);
 
        SCL_H();  //SCL = 1;
        delay_us(5);
        SCL_L();  //SCL = 0;
        delay_us(5);
    }
 
    bResult=IIC_ReceiveACK(); //??????????,????????
 
    return bResult;  //??????
}
 
unsigned char IIC_ReadByte(void)
{
    unsigned char dat;
    unsigned char i;
     
    SCL_H();     //SCL = 1;//????????????
    delay_us(5);
 
    for( i=0;i<8;i++ )
    {
        dat <<= 1;
        dat = dat | (SDAIN);
        delay_us(5);
         
        SCL_L();   //SCL = 0;
        delay_us(5);    
        SCL_H();   //SCL = 1;
        delay_us(5);    
    }
    return dat;
}
 
/*?BMP180??1??????*/
u8 BMP180_ReadOneByte(u8 ReadAddr)
{
    u8 temp = 0;
    u8 IIC_ComFlag = 1;   //IIC????,?0????,1??????
     
    IIC_Start();     //IIC start
    IIC_ComFlag = IIC_SendByte(BMP180_SlaveAddr);   //slave address+W:0
    //IIC_SendByte1(BMP180_SlaveAddr,TVOC_1);
    //IIC_ComFlag = IIC_ChkACK(TVOC_1);
    if (IIC_ComFlag == 0)                           //????0??????,?????????????
    {
        IIC_SendByte(ReadAddr);                      //??????
        //IIC_SendByte1(ReadAddr,TVOC_1);
        IIC_Start();
        IIC_SendByte(BMP180_SlaveAddr|0x01);         //slave address+R:1
        //IIC_SendByte1(BMP180_SlaveAddr|0x01,TVOC_1);
        temp = IIC_ReadByte();                       //???
       //temp = IIC_RecvByte(TVOC_1);
         IIC_SendACK(1);                 
        IIC_Stop();
    }
    return (temp);      
}
 
/*?BMP180??2??????*/
short BMP180_ReadTwoByte(u8 ReadAddr)
{
    u8 IIC_ComFlag = 1;   //IIC????,?0????,1??????
    u8 MSB,LSB;
    short int temp;
     
    IIC_Start();
    IIC_ComFlag = IIC_SendByte(BMP180_SlaveAddr);
    if (IIC_ComFlag == 0)
    {
        IIC_SendByte(ReadAddr);
        IIC_Start();
        IIC_SendByte(BMP180_SlaveAddr|0x01);
        MSB = IIC_ReadByte();       //????
        IIC_SendACK(0);         //ACK
        LSB = IIC_ReadByte();      //????
        IIC_SendACK(1);        //NACK
        IIC_Stop();
    }
    temp = MSB*256+LSB;
 
    return temp;                                                    
}
 
/*?BMP180????????????*/
void Write_OneByteToBMP180(u8 RegAdd, u8 Data)
{
    IIC_Start();                       //IIC start
    IIC_SendByte(BMP180_SlaveAddr);   //slave address+W:0
    IIC_SendByte(RegAdd);
    IIC_SendByte(Data);
    IIC_Stop(); 
}
 
 
/*??BMP180?????*/
void Read_CalibrationData(void)
{
    AC1 = BMP180_ReadTwoByte(0xaa);
    
    AC2 = BMP180_ReadTwoByte(0xac);
    
    AC3 = BMP180_ReadTwoByte(0xae);
    AC4 = BMP180_ReadTwoByte(0xb0);
    AC5 = BMP180_ReadTwoByte(0xb2);
    AC6 = BMP180_ReadTwoByte(0xb4);
    B1 = BMP180_ReadTwoByte(0xb6);
    B2 = BMP180_ReadTwoByte(0xb8);
    MB = BMP180_ReadTwoByte(0xba);
    MC = BMP180_ReadTwoByte(0xbc);
    MD = BMP180_ReadTwoByte(0xbe);
 
}
 
/*?BMP180??????????*/
long Get_BMP180UT(void)
{
    long UT;
    unsigned char MSB = 0,LSB = 0;
    Write_OneByteToBMP180(0xf4,0x2e);       //write 0x2E into reg 0xf4
    delay_ms(6);                                   //wait 4.5ms
    MSB = BMP180_ReadOneByte(0xf6);          //read reg 0xF6(MSB),0xF7(LSB)
    LSB = BMP180_ReadOneByte(0xf7);
    UT  = (MSB<<8)+LSB;
  //  printf(\"UT:%ld \r\n\",UT);
 
    return UT;
}
 
/*?BMP180??????????*/
long Get_BMP180UP(void)
{
    long UP=0;
    u8 MSB=0,LSB=0;//,XLSB=0;
    Write_OneByteToBMP180(0xf4,0x34);       //write 0x34 into reg 0xf4 
    delay_ms(10);                                    //wait 4.5ms
    //UP = BMP180_ReadTwoByte(0xf6); 
   // UP &= 0x0000FFFF;
    MSB = BMP180_ReadOneByte(0xf6); 
    LSB = BMP180_ReadOneByte(0xf7); 
    //XLSB = BMP180_ReadOneByte(0xf8); 
   // UP = (MSB<<16 + LSB<<8 + XLSB) >> (8-OSS);
    UP=(MSB<<8)+LSB;//这里的修改是的压力值会改变了
    UP &= 0x0000FFFF;
   // printf(\"UP:%ld \r\n\",UP);
     
    return UP;      
}
 
/*?????????????????????????
 *True_Temp:?????,??:?
 *True_Press:?????,??:Pa
 *True_Altitude:??????,??:m
*/
/*
float Convert_UncompensatedToTrue(long UT,long UP)
{
    long X1,X2,X3,B3,B5,B6,B7,T,P;
    unsigned long B4;
     
    X1 = ((UT-AC6)*AC5)>>15;      //printf(\"X1:%ld \r\n\",X1);
    X2 = ((long)MC<<11)/(X1+MD);  //printf(\"X2:%ld \r\n\",X2);
    B5 = X1+X2;                        //printf(\"B5:%ld \r\n\",B5);
    T = (B5+8)>>4;                      //printf(\"T:%ld \r\n\",T);
    True_Temp = T/10.0;         //   printf(\"Temperature:%.1f \r\n\",True_Temp);
 
    B6 = B5-4000;                       //printf(\"B6:%ld \r\n\",B6);
    X1 = (B2*((B6*B6)>>12))>>11;              //printf(\"X1:%ld \r\n\",X1);
    X2 = (AC2*B6)>>11;                //printf(\"X2:%ld \r\n\",X2);
    X3 = X1+X2;                         //printf(\"X3:%ld \r\n\",X3);
    B3 = (((long)AC1*4+X3)<<OSS+2)/4;    //printf(\"B3:%ld \r\n\",B3);
    X1 = (AC3*B6)>>13;                //printf(\"X1:%ld \r\n\",X1);
    X2 = (B1*((B6*B6)>>12))>>16;      //printf(\"X2:%ld \r\n\",X2);
    X3 = ((X1+X2)+2)>>2;              //printf(\"X3:%ld \r\n\",X3);
    B4 = (AC4*(unsigned long)(X3+32768))>>15;   //printf(\"B4:%lu \r\n\",B4);
    B7 = ((unsigned long)UP-B3)*(50000>>OSS);        //printf(\"B7:%lu \r\n\",B7);
    if (B7 < 0x80000000)
    {
        P = (B7*2)/B4;  
    }
    else P=(B7/B4)*2;                   //printf(\"P:%ld \r\n\",P);         
    X1 = (P/256.0)*(P/256.0);       //printf(\"X1:%ld \r\n\",X1);
    X1 = (X1*3038)>>16;               //printf(\"X1:%ld \r\n\",X1);
    X2 = (-7357*P)>>16;               //printf(\"X2:%ld \r\n\",X2);
    P = P+((X1+X2+3791)>>4);      //printf(\"P:%ld \r\n\",P);
    True_Press = (P /100.0)+0.005;              //   printf(\"Press:%.1fPa \r\n\",True_Press);
		
 
    //True_Altitude = 44330*(1-pow((P/101325.0),(1.0/5.255)));            
    return True_Press;
}*/
float Convert_UncompensatedToTrue(long UT,long UP)
{
    long X1,X2,X3,B3,B5,B6,B7,T,P;
    unsigned long B4;
  
    X1 = ((UT-AC6)*AC5)>>15;      //printf(\"X1:%ld \r\n\",X1);
    X2 = ((long)MC<<11)/(X1+MD);  //printf(\"X2:%ld \r\n\",X2);
    B5 = X1+X2;                        //printf(\"B5:%ld \r\n\",B5);
   
    B6 = B5-4000;                       //printf(\"B6:%ld \r\n\",B6);
    X1 = (B2*B6*B6)>>23;              //printf(\"X1:%ld \r\n\",X1);
    X2 = (AC2*B6)>>11;                //printf(\"X2:%ld \r\n\",X2);
    X3 = X1+X2;                         //printf(\"X3:%ld \r\n\",X3);
    B3 = (((long)AC1*4+X3)+2)/4;    //printf(\"B3:%ld \r\n\",B3);
    X1 = (AC3*B6)>>13;                //printf(\"X1:%ld \r\n\",X1);
    X2 = (B1*(B6*B6>>12))>>16;      //printf(\"X2:%ld \r\n\",X2);
    X3 = ((X1+X2)+2)>>2;              //printf(\"X3:%ld \r\n\",X3);
    B4 = AC4*(unsigned long)(X3+32768)>>15;   //printf(\"B4:%lu \r\n\",B4);
    B7 = ((unsigned long)UP-B3)*50000;        //printf(\"B7:%lu \r\n\",B7);
    if (B7 < 0x80000000)
    {
        P = (B7*2)/B4;  
    }
    else P=(B7/B4)*2;                   //printf(\"P:%ld \r\n\",P);         
    X1 = (P/256.0)*(P/256.0);       //printf(\"X1:%ld \r\n\",X1);
    X1 = (X1*3038)>>16;               //printf(\"X1:%ld \r\n\",X1);
    X2 = (-7357*P)>>16;               //printf(\"X2:%ld \r\n\",X2);
    P = P+((X1+X2+3791)>>4);      //printf(\"P:%ld \r\n\",P);
    True_Press = (P /1000.0)+0.005; //   此处除以1000表示KPa
		
    return True_Press;
}