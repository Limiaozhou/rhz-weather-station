#ifndef _ADC_H_
#define _ADC_H_

//��ʼ��
extern void ADC_Init(void);
//ADC��ȡ
extern int ADC_Read(void);

extern float ADC_ReadVel(void);

extern float ADC_ReadNow(void);

extern int ADC_Readtest(void);
#endif