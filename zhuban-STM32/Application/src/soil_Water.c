#include "soil_Water.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_adc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F103RCT6
//室外环境监测系统
//主函数代码
//制作者：nishusheng
//版本：V1.0
//修改日期:2015/7/27
//Copyright(C) 深圳市睿海智电子科技有限公司  	 					  
////////////////////////////////////////////////////////////////////////////////// 
	   
/***************************************
*@DESCRIPTION: --初始化ADC,这里我们开启规则通道0~14
*
*@Input: --无
*
*@Out: --无
*
*@Return: --无
****************************************/		   
															   
void  Moisture_ADC_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
		//PC5作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
/***************************************
*@DESCRIPTION: --获得ADC值
*
*@Input: --ch：通道
*
*@Out: --无
*
*@Return: --ADC1规则组的最后一次转换结果
****************************************/
u16 Get_moisture_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

/***************************************
*@DESCRIPTION: --获得电压的平均值
*
*@Input: --ch：通道；times：次数
*
*@Out: --无
*
*@Return: --无
****************************************/
double Get_moisture_Average(u8 ch,u8 times)
{
	double temp_val=0.0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_moisture_Adc(ch);
		delay_ms(5);
	}
	temp_val=temp_val/times*(3.3/4096);    //3.3V参考电压，12位分辨率
	return temp_val;
} 	 

/***************************************
*@DESCRIPTION: --将ADC1某个通道的值通过串口输出
*
*@Input: --channelx:ADC1通道，可选10-13
*
*@Out: --无
*
*@Return: --无
****************************************/


/***************************************
*@DESCRIPTION: --把浮点数转换成字符
*
*@Input: --data：要转换的浮点数;temp:转换后的字符的存储数组
*
*@Out: --无
*
*@Return: --无
****************************************/

void Get_moisture(float *temp_light)
{
	*temp_light=Get_moisture_Average(9,8);//PC5,通道15
  *temp_light = 122-*temp_light*37;
  if(*temp_light>100) *temp_light = 100;
  else if(*temp_light<0) *temp_light = 0;
}

