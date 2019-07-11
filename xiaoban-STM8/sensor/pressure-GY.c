#include "pressure-GY.h"

unsigned int c1, c2, c3, c4, c5, c6;  //校准数据

static unsigned long int Pressure_IIC_ReadTemp(void);
static unsigned long int Pressure_IIC_ReadPres(void);

//读取温度temperature，d2
static unsigned long int Pressure_IIC_ReadTemp(void)
{
    unsigned char buf[3] = {0};
    unsigned long int data = 0;
    
    if(!IIC_Master_Write(1, PRES_DEVICE_ADDRESS, 0x58, buf, 0))
    {
        my_delay_ms(DELAY_CONVERSION);  //等待转换接收，10ms
        
        if(!IIC_Master_Write(1, PRES_DEVICE_ADDRESS, 0x00, buf, 0))
        {
            IIC_Master_Read(1, PRES_DEVICE_ADDRESS, 0x00, buf, 3);
            data = (unsigned long int)buf[0] << 16;
            data += (unsigned long int)buf[1] << 8;
            data += buf[2];
        }
    }
    
    return data;
}

//读取气压pressure，d1
static unsigned long int Pressure_IIC_ReadPres(void)
{
    unsigned char buf[3] = {0};
    unsigned long int data = 0;

    if(!IIC_Master_Write(1, PRES_DEVICE_ADDRESS, 0x48, buf, 0))
    {
        my_delay_ms(DELAY_CONVERSION);  //等待转换接收，10ms
        
        if(!IIC_Master_Write(1, PRES_DEVICE_ADDRESS, 0x00, buf, 0))
        {
            IIC_Master_Read(1, PRES_DEVICE_ADDRESS, 0x00, buf, 3);
            data = (unsigned long int)buf[0] << 16;
            data += (unsigned long int)buf[1] << 8;
            data += buf[2];
        }
    }
    
    return data;
}

//传感器初始化，读取校准数据
void Pressure_Init(void)
{
    unsigned char buf[2] = {0};

//    IIC_Master_Init(1);
    
    if(!IIC_Master_Write(1, PRES_DEVICE_ADDRESS, 0xA2, buf, 0))
    {
        IIC_Master_Read(1, PRES_DEVICE_ADDRESS, 0xA2, buf, 2);
        c1 = ((unsigned int) buf[0] << 8) | buf[1];
    }
    
    if(!IIC_Master_Write(1, PRES_DEVICE_ADDRESS, 0xA4, buf, 0))
    {
        IIC_Master_Read(1, PRES_DEVICE_ADDRESS, 0xA4, buf, 2);
        c2 = ((unsigned int) buf[0] << 8) | buf[1];
    }
    
    if(!IIC_Master_Write(1, PRES_DEVICE_ADDRESS, 0xA6, buf, 0))
    {
        IIC_Master_Read(1, PRES_DEVICE_ADDRESS, 0xA6, buf, 2);
        c3 = ((unsigned int) buf[0] << 8) | buf[1];
    }
    
    if(!IIC_Master_Write(1, PRES_DEVICE_ADDRESS, 0xA8, buf, 0))
    {
        IIC_Master_Read(1, PRES_DEVICE_ADDRESS, 0xA8, buf, 2);
        c4 = ((unsigned int) buf[0] << 8) | buf[1];
    }
    
    if(!IIC_Master_Write(1, PRES_DEVICE_ADDRESS, 0xAA, buf, 0))
    {
        IIC_Master_Read(1, PRES_DEVICE_ADDRESS, 0xAA, buf, 2);
        c5 = ((unsigned int) buf[0] << 8) | buf[1];
    }
    
    if(!IIC_Master_Write(1, PRES_DEVICE_ADDRESS, 0xAC, buf, 0))
    {
        IIC_Master_Read(1, PRES_DEVICE_ADDRESS, 0xAC, buf, 2);
        c6 = ((unsigned int) buf[0] << 8) | buf[1];
    }
    
    my_delay_ms(5);
}

//传感器数据读取和计算
void Pressure_Convert(void)
{
  static unsigned char flag = 0;  //通信成功标志位,0为没通信上
  unsigned long int d1, d2;
  long int dt, temp, p;
  long long int off, sens;
//  long int off2, sens2, t2;
  
  if(!flag)  //没通信上就发复位
  {
    if(!Pressure_Reset())  //通信上了
    {
      my_delay_ms(100);
      Pressure_Init();  //读取校准数据
      flag = 1;  //停止发复位和读取校准数据
    }
  }
  else  //通信上后读取、计算数据
  {
    d1 = Pressure_IIC_ReadPres();  //读取气压数据
    d2 = Pressure_IIC_ReadTemp();  //读取温度数据

    dt = d2 - ((unsigned long int)c5 << 8);
    temp = 2000 + (dt * c6 >> 23);

    off = ((unsigned long long int)c2 << 16) + (c4 * dt >> 7);
    sens = ((unsigned long long int)c1 << 15) + (c3 * dt >> 8);
    p = ((d1 * sens >> 21) - off) >> 15;

    Evndat.press_temp = temp / 100.0;  //度
    Evndat.press = p / 1000.0;  //mbar/10--100Pa/10--KPa
  }
}

//传感器复位
unsigned char Pressure_Reset(void)
{
  unsigned char data;
  if(!IIC_Master_Write(1, PRES_DEVICE_ADDRESS, 0x1E, &data, 0))
    return 0;  //成功返回0
  else
      return 1;  //失败返回1
}
