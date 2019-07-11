#include"iostm8s103F3.h"
#include"delay.h"

void ADC_Init(void)
{
		ADC_CSR=0x02;   //ѡ��ͨ�� AIN2/PC4 
		ADC_CR2|=0x08;  // ��������,�Ҷ���
    ADC_CR3 = 0x00; // ��ʹ�����ݻ���
		//ADC_TDRL|=0xff;  //�ر�ʩ���ش�����
    //ADC_TDRL|=0x03;
}

int ADC_Read(void)
{   
    int ret = 0;
		ADC_CR1|=0x01;  //����ת��
    delay_2us(50); // ��ʱһ��ʱ�䣬����7uS����֤ADC ģ���ϵ�������
    ADC_CR1|=0x01;      // �ٴν�CR1�Ĵ��������λ��1,����ADת��
    while((ADC_CSR & 0x80)!=0x80); // �ȴ�ת������
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