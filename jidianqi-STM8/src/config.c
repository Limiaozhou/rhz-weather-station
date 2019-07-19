//#include"iostm8s103K3.h"
#include"iostm8s103F3.h"
#include"eeprom.h"
#include"mem.h"
#include"UART.h"
#include"ADC.h"
#include"IO.h"
#include"IIC.h"
#include"stdio.h"
#include "config.h"

//#define         DEVTYPE     0x03     //甲醛

#define         DBMIN           0      //定标数量
#define         DBMAX           100    //定标数量
#define         PPMMIN          0      //定标数量
#define         PPMMAX          10     //定标数量

#define         MIN           0      //定标数量
#define         MAX           100    //定标数量
#define         VOLMIN           0      //定标数量
#define         VOLMAX           3.3    //定标数量

char Error_code;
ST_Config config;

float avevol,maxvol;
//检查浓度值是否在范围之内
bool Config_Checkvalue(float dat)
{   
  if(dat>=MIN &&  dat<=MAX) return true;
  
  else { Error_code=8; return false;}
  
}

void Config_Save(void)
{   
    char count = 10;
    while(count--)
    {
       if(eeprom_write(0x4000,(uchar *)config.addr, sizeof(config)))  break;
    }
    
    if(count<1)  //存入失败
    {
      Error_code=2;
      IO_Set(0);
    }
}

void Config_Init(void)
{
  mem_clear((uchar *)&config.type, sizeof(config)-8);
  config.type = 0; 
  config.count = 2;
  config.group[0].Voltage = VOLMIN;
  config.group[0].dbppm = MIN;
  
  config.group[1].Voltage = VOLMAX;
  config.group[1].dbppm = DBMAX;
  
  Config_Save();
}

void Config_Read(void)
{   
    eeprom_read(0x4000,(uchar *)config.addr, sizeof(config));
    if(config.type == 0 || config.type != DEVTYPE) //第一次进入 未初始化
    {
      Config_Init();
    }

    //灯闪
    IO_light(400, 4);

}

void Config_Send_Error(void)
{   
    
    UART_Send((uchar *)&config.addr, sizeof(config));
}

void Config_Senddat(void)
{   
    //float_to_str(char *pbuf,float num,char count);
    UART_Send((uchar *)&config.addr, sizeof(config));
}

void Config_SendVol(void)
{   
    //float_to_str(char *pbuf,float num,char count);
    UART_Send((uchar *)&config.addr, sizeof(config));
}

char Config_Checkpair(ST_DBPPMConfig  dbvol)
{   
    char i,j;
    if(config.count >= CONFIGCOUNT-1) return 0xFF;
    
    
    for (i = 0; i<config.count; i++)    
    {  
        if(dbvol.dbppm<config.group[i].dbppm)   //从后向前，找到比其小的数的位置 
        {
        }
        else break;
    }
    
    for (j = config.count; j>0; j--)    
    {  
        if(dbvol.dbppm<config.group[j].dbppm)   //从后向前，找到比其小的数的位置 
        {
        }
        else break;
    }
    
    if(i == j)  return j;
    else        return 0xFF;  
  
  return 0;
}
//配置参数确认，一定要是线性关系
bool Config_CheckPara()
{
  char i;
  for(i=1;i<config.count;i++)
  {
    if(config.group[i].dbppm<config.group[i-1].dbppm)
    { Error_code=5;  return false;}
    if(config.group[i].Voltage<config.group[i-1].Voltage)
    { Error_code=4;  return false;}
  }
  return true;
}

//添加
bool Config_Addpair(ST_DBPPMConfig  dbvol)
{   
    char i,j;
    if(config.count >= CONFIGCOUNT-1){ Error_code=9; return false;}
    if(dbvol.Voltage<0 && dbvol.Voltage>3.3)  {Error_code=10; return false; }    
    i = config.count;
    
    //更新
    for(j=0;j<config.count;j++)
    {
        if(dbvol.dbppm == config.group[j].dbppm)   //更新 
        {  
            Error_code=6;
            config.group[j].Voltage = dbvol.Voltage;
            return true;
        }
    }
      
    //插入
    while(i--)
    {  
        if(dbvol.Voltage<config.group[i].Voltage)   //从后向前，找到比其小的数的位置  
        {  
            config.group[i+1] = config.group[i];    //向后挪动  
        }
        else 
        {  
            config.group[i+1] = dbvol;
            config.count+=1;
            return true;
        }
    }
    Error_code=11;
    return false;
}

void Config_Add(float dat)
{   
    ST_DBPPMConfig  dbvol;
    
    if(Config_Checkvalue(dat))
    {
      dbvol.dbppm = dat;
      dbvol.Voltage = ADC_ReadVel();
      if(Config_Addpair(dbvol) && Config_CheckPara())
      {
         Config_Save();
         IO_light(500, 4);
      }
      else
      {
         //失败提示
         IO_light(500, 5);
         //IO_Set(0);
         
      } 
    }
    else
      {
         //失败提示
        IO_light(500, 5);
        // IO_Set(0);
      } 
    
}

//计算实际输出
void Config_Dealdat(float vol)
{   
  char i = 0;
  float dat=0;
  ST_DBPPMConfig  low,high;
  for (i = 0; i<config.count; i++)      
  {  
    //在标准点上
    if(vol==config.group[i].Voltage)   //从后向前，找到比其小的数的位置  
    {
      SendDat =  config.group[i].dbppm;
      return;
    }
    //在两点之间
    if(vol>config.group[i].Voltage && vol<config.group[i+1].Voltage)   //从后向前，找到比其小的数的位置  
    {
      low = config.group[i];
      high = config.group[i+1];
      
      dat = low.dbppm*(vol-high.Voltage)/(low.Voltage- high.Voltage) + high.dbppm*(vol-low.Voltage)/(high.Voltage-low.Voltage);
      if(Config_Checkvalue(dat))
      {
        SendDat = dat;
        return;
      } 
    }
  }
}

void Config_ADCMaxAve(float vol)
{  
   //最大
   if(maxvol<vol)  maxvol = vol;
   
   //平均
   if(avevol>0) 
   {
      avevol = (avevol + vol)/2;
   }
   else
   {
      avevol = vol;
   }
}

float tuf[100];
//char pos = 0;
//计算实际输出
void Config_Deal(void)
{   
    char i = 0;
    float dat=0;
    ST_DBPPMConfig  low,high;
    /*ST_DBPPMConfig dbvol;
    dbvol.dbppm = 0.1;
    dbvol.Voltage = 0.2;
    Config_Addpair(dbvol);
    
    dbvol.dbppm = 1;
    dbvol.Voltage = 0.5;
    Config_Addpair(dbvol);
    
    dbvol.dbppm = 2;
    dbvol.Voltage = 0.8;
    Config_Addpair(dbvol);
    
    
    Config_Dealdat(0.5);
    Config_Dealdat(1);
    Config_Dealdat(1.45);
    Config_Dealdat(2.0);
    Config_Dealdat(3.2);
    */
    
    //测试
    
    //
    float vol = ADC_ReadVel();
    Config_Dealdat(vol);
    /*uchar d[3];
    uchar s = (uchar)SendDat;
    d[0] = 0x30 + s/10;
    d[1] = 0x30 + s%10;
    d[2] = '\n';
    UART_Send(d, 3);
    
    
    for(int i=0; i<100;i++)
    {
    float vol = ADC_ReadNow();
    
    tuf[pos++] = vol;
    if(pos>100) pos = 0;
    }
    
    //Config_ADCMaxAve(vol);
    //Config_Dealdat(vol);
    uchar d[3];
    uchar s = (uchar)SendDat;
    d[0] = 0x30 + s/10;
    d[1] = 0x30 + s%10;
    d[2] = '\n';
    UART_Send(d, 3);
    
    
    for(int i=0; i<100;i++)
    {
       char d[10];
       float_to_str(d,tuf[i],4);
       UART_Send(d, 7);
    }.
     */
     //Config_Deal();
    /*
     //在标准点上
    for (i = 0; i<config.count; i++)      
    {  
        if(vol==config.group[i].Voltage)   //从后向前，找到比其小的数的位置  
        {0
           SendDat =  config.group[i].dbppm;
           return;              
        }
        //else break;0000000
    }
    //在两点之间
    for (i = 0; i<config.count; i++)    
    {  
        if(vol>config.group[i].Voltage &&  vol<config.group[i+1].Voltage)   //从后向前，找到比其小的数的位置  
        {
           low = config.group[i];
           high = config.group[i+1];
           
           dat = low.dbppm*(vol-high.Voltage)/(low.Voltage- high.Voltage) + high.dbppm*(vol-low.Voltage)/(high.Voltage-low.Voltage);
           if(Config_Checkvalue(dat))
           {
              SendDat = dat;
              return;
           } 
        }      
    }  
    */
    
}




