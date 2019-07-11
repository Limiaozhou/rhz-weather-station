#include "led_status.h"
#include "led.h"
#include "data.h"

unsigned char com_data[16] = {0,37,0,75,0,6,0,10,0x2,0xbc,0x3,0xe8,0,50,1,0};
//extern unsigned char data[];

int period=6;//µ•Œª£∫√Î
int reset_count;

struct show_data
{
  float low_data;
  float high_data;
};
typedef struct STLedcolor
{
  struct show_data HCHO_data;
  struct show_data CO2_data;
  struct show_data TVOC_data;
  struct show_data DUST_data;
}ST_Ledcolor;
ST_Ledcolor Ledcolor;

void init_ledshow(void)
{
//  osal_nv_read(SENSOR, 0, 16, com_data);
  Ledcolor.DUST_data.low_data = (int)(com_data[0]<<8)+com_data[1];
  Ledcolor.DUST_data.high_data = (int)(com_data[2]<<8)+com_data[3];
  Ledcolor.HCHO_data.low_data = com_data[4]+com_data[5]*0.01;
  Ledcolor.HCHO_data.high_data = com_data[6]+com_data[7]*0.01;
  Ledcolor.CO2_data.low_data = (int)(com_data[8]<<8)+com_data[9];
  Ledcolor.CO2_data.high_data = (int)(com_data[10]<<8)+com_data[11];
  Ledcolor.TVOC_data.low_data = com_data[12]+com_data[13]*0.01;
  Ledcolor.TVOC_data.high_data = com_data[14]+com_data[15]*0.01;
}
void led_light(EVNDAT *data)
{
  float tvoc_data=0;
	unsigned char led_flag=0;//0:¬Ãµ∆  1:ª∆µ∆  2:∫Ïµ∆
 // GREEN_LIGHT;


  switch(6)
  {
  case 6://co2
    if(data->co2 > Ledcolor.CO2_data.high_data)
    {  led_flag=2; break; }
    else if(data->co2 <= Ledcolor.CO2_data.low_data);
    else
    { led_flag=1;}
  case 5://º◊»©
    if (data->hcho > Ledcolor.HCHO_data.high_data)
    {  led_flag=2; break; }
    else if(data->hcho <= Ledcolor.HCHO_data.low_data);
    else
    { led_flag=1;}
  case 4://ø≈¡£ŒÔ
    if (data->pm25 > Ledcolor.DUST_data.high_data)
    {  led_flag=2; break; }
    else if(data->pm25 <= Ledcolor.DUST_data.low_data);
    else
    { led_flag=1;}
  case 3://tvoc
    tvoc_data = (float)data->ltvoc/1000;
    if(tvoc_data>=0.125)
      tvoc_data = (tvoc_data-0.12)*3.6+0.12;
    if(tvoc_data > Ledcolor.TVOC_data.high_data)
    {  led_flag=2; break; }
    else if(tvoc_data <= Ledcolor.TVOC_data.low_data);
    else
    { led_flag=1;} 
  }
	switch(led_flag)
	{
		case 0:led_set(GREENLIGHT);
			break;
		case 1:led_set(YELLOWLIGHT);
			break;
		case 2:led_set(REDLIGHT);
			break;
		default:led_set(GREENLIGHT);
			break;
	}
}
