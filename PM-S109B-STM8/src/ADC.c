#include"iostm8s103F3.h"
#include"delay.h"

void ADC_Init(void)
{
		ADC_CSR=0x02;   //选择通道 AIN2/PC4 
		ADC_CR2|=0x08;  // 数据排列,右对齐
    ADC_CR3 = 0x00; // 不使用数据缓冲
		//ADC_TDRL|=0xff;  //关闭施密特触发器
    //ADC_TDRL|=0x03;
}

int ADC_Read(void)
{   
    int ret = 0;
		ADC_CR1|=0x01;  //启动转换
    delay_2us(50); // 延时一段时间，至少7uS，保证ADC 模块上电至正常
    ADC_CR1|=0x01;      // 再次将CR1寄存器的最低位置1,开启AD转换
    while((ADC_CSR & 0x80)!=0x80); // 等待转换结束
    ADC_CSR &= 0x7F;
    ret = ((ADC_DRH<<8) & 0x0300);
    ret +=ADC_DRL;
    
    return ret;
}



float ADC_ReadVel(void)
{   
    float ret = 0;
    
    ADC_Read();
    for(int i = 0; i<10;i++)
    {
       ret += ADC_Read();
    }
    return ret/10240 *3.3;
}

float ADC_ReadNow(void)
{   
    float ret = 0;
    
    ret = ADC_Read();
    ret = ADC_Read();
    return ret/1024 *3.3;
}

/*
int tuf[100];
int ADC_Readtest(void)
{   
    int ret = 0;
    
    ret = ADC_Read();
    for(int i = 0; i<100;i++)
    {
       tuf[i] = ADC_Read();
    }
    return ret/20;
}
*/