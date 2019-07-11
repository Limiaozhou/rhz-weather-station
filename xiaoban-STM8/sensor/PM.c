#include <iostm8s103F3.h>
#include "PM.h"

//读取PM数据处理
void PM_Read(void)
{
  float ret = 0;
  unsigned char data[6] = {0};
  
  IIC_Master_Read(3, PM_DEVICE_ADDRESS, 0, data, 6);
  
  if(Check_CheckCRC16(data,4))
  {
    ret = data[0] * 100.0 + data[1];
    
    if(!ret)
      ret = 1;  //令PM不显示为0
    
    if(ret <= 5000)
    {
      Evndat.pm25 = ret;
    }
  }
  else
    Evndat.pm25 = 0;
}
