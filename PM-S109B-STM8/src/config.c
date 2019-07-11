//#include"iostm8s103K3.h"
#include"iostm8s103F3.h"
#include"eeprom.h"
#include"mem.h"
#include"UART.h"
#include"ADC.h"
#include"IO.h"
#include"IIC.h"
#include"stdio.h"

#define         DEVTYPE     0x01     //声音
//#define         DEVTYPE     0x03     //甲醛
#define         CONFIGCOUNT     20     //定标数量

#define         DBMIN           0      //定标数量
#define         DBMAX           100    //定标数量
#define         PPMMIN          0      //定标数量
#define         PPMMAX          10     //定标数量

typedef  struct DBPPMConfig
{
   float Voltage;
   float dbppm;
}ST_DBPPMConfig;

typedef struct Config
{
   char type;     //0 没有配置 1 声音  2 声音标件 3 甲醛 4甲醛标件
   char count;    //已经定标数量
   ST_DBPPMConfig group[CONFIGCOUNT];
   int crc;
}ST_Config;

ST_Config config;

float avevol,maxvol;

bool Config_Checkvalue(float dat)
{   
    if(DEVTYPE==0x01)
    {
       if(dat>=DBMIN &&  dat<=DBMAX) return true;
       else return false;
    }
    else if(DEVTYPE==0x03)
    {
       if(dat>=PPMMIN &&  dat<=PPMMAX) return true;
       else return false;
    }
    
    return false;
   
}

void Config_Save(void)
{   
    char count = 10;
    while(count--)
    {
       if(eeprom_write(0x4000,(uchar *)&config.type, sizeof(config)))  break;
    }
    
    if(count<1)  //存入失败
    {
      IO_Set(0);
    }
}


void Config_Read(void)
{   
    eeprom_read(0x4000,(uchar *)&config.type, sizeof(config));
    if(config.type == 0 || config.type != DEVTYPE) //第一次进入 未初始化
    {
       mem_clear((uchar *)&config.type, sizeof(config));
       if(DEVTYPE==0x01)
       {
          config.type = DEVTYPE; 
          config.count = 4;
          config.group[0].Voltage = 0;
          config.group[0].dbppm = DBMIN;
    
          //config.group[1].Voltage = 3.3;
          //config.group[1].dbppm = DBMAX;
      
          config.group[1].Voltage = 1.43;
          config.group[1].dbppm = 58;
       
          config.group[2].Voltage = 2.0;
          config.group[2].dbppm = 80;
      
          config.group[3].Voltage = 3.3;
          config.group[3].dbppm = DBMAX;
          
       }
       else if(DEVTYPE==0x03)
       {
          config.type = DEVTYPE; 
          config.count = 2;
          config.group[0].Voltage = 0;
          config.group[0].dbppm = PPMMIN;
          config.group[1].Voltage = 3.3;
          config.group[1].dbppm = PPMMAX;
         
       }
       Config_Save();
    }

    //灯闪
    IO_light(400, 4);

}

void Config_Send(void)
{   
    UART_Send((uchar *)&config.type, sizeof(config));
}

void Config_Senddat(void)
{   
    //float_to_str(char *pbuf,float num,char count);
    UART_Send((uchar *)&config.type, sizeof(config));
}

void Config_SendVol(void)
{   
    //float_to_str(char *pbuf,float num,char count);
    UART_Send((uchar *)&config.type, sizeof(config));
}

char Config_Checkpair(ST_DBPPMConfig  dbvol)
{   
   /* char i,j;
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
  */
  return 0;
}


bool Config_Addpair(ST_DBPPMConfig  dbvol)
{   
    char i,j;
    if(config.count == CONFIGCOUNT-1) return false;
    if(dbvol.Voltage<0 && dbvol.Voltage>3.3)  return false;     
    i = config.count;
    
    //更新
    for(j=0;j<config.count;j++)
    {
        if(dbvol.dbppm == config.group[j].dbppm)   //更新 
        {  
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
    
    return false;
}

void Config_Add(float dat)
{   
    ST_DBPPMConfig  dbvol;
    
    if(Config_Checkvalue(dat))
    {
      dbvol.dbppm = dat;
      dbvol.Voltage = ADC_ReadVel();
      if(Config_Addpair(dbvol))
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
    //在标准点上
    for (i = 0; i<config.count; i++)      
    {  
        if(vol==config.group[i].Voltage)   //从后向前，找到比其小的数的位置  
        {
           SendDat =  config.group[i].dbppm;
           return;
        }
        //else break;
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

/*float tuf[50];
float muf[50];
char pos = 0,mos = 0;
*/
void Config_DealADCMaxAve(void)
{  
  
   /*tuf[pos++] = maxvol;
   if(pos>50) pos = 0;
   
   muf[mos++] = avevol;
   if(mos>50) mos = 0;
   
   //最大
   maxvol = 0;
   //平均
   avevol = 0;
  */

}

void float_to_str(char *pbuf,float num,char count)
{
     
     *pbuf++ = (char)num + 0x30;
     *pbuf++ = '.';
     float dat = num- (int)num;
     while(count--)
     {
        dat *= 10;
        *pbuf++ = (char)dat + 0x30;
        dat = dat-(int)dat;
     }
     
     *pbuf++ = '\n';
}

float tuf[100];
char pos = 0;
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




