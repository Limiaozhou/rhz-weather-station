#include "soil_Water.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_adc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F103RCT6
//���⻷�����ϵͳ
//����������
//�����ߣ�nishusheng
//�汾��V1.0
//�޸�����:2015/7/27
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 					  
////////////////////////////////////////////////////////////////////////////////// 
	   
/***************************************
*@DESCRIPTION: --��ʼ��ADC,�������ǿ�������ͨ��0~14
*
*@Input: --��
*
*@Out: --��
*
*@Return: --��
****************************************/		   
															   
void  Moisture_ADC_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
		//PC5��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}				  
/***************************************
*@DESCRIPTION: --���ADCֵ
*
*@Input: --ch��ͨ��
*
*@Out: --��
*
*@Return: --ADC1����������һ��ת�����
****************************************/
u16 Get_moisture_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

/***************************************
*@DESCRIPTION: --��õ�ѹ��ƽ��ֵ
*
*@Input: --ch��ͨ����times������
*
*@Out: --��
*
*@Return: --��
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
	temp_val=temp_val/times*(3.3/4096);    //3.3V�ο���ѹ��12λ�ֱ���
	return temp_val;
} 	 

/***************************************
*@DESCRIPTION: --��ADC1ĳ��ͨ����ֵͨ���������
*
*@Input: --channelx:ADC1ͨ������ѡ10-13
*
*@Out: --��
*
*@Return: --��
****************************************/


/***************************************
*@DESCRIPTION: --�Ѹ�����ת�����ַ�
*
*@Input: --data��Ҫת���ĸ�����;temp:ת������ַ��Ĵ洢����
*
*@Out: --��
*
*@Return: --��
****************************************/

void Get_moisture(float *temp_light)
{
	*temp_light=Get_moisture_Average(9,8);//PC5,ͨ��15
  *temp_light = 122-*temp_light*37;
  if(*temp_light>100) *temp_light = 100;
  else if(*temp_light<0) *temp_light = 0;
}

