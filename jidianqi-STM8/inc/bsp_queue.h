#ifndef __BSP_QUEUE_H
#define __BSP_QUEUE_H

#include <stdlib.h>

typedef struct
{
	unsigned char head;  //ͷָ�룬ָ��Ҫ��ȡ���ݵ�ַ
	unsigned char tail;  //βָ�룬ָ��Ҫд�����ݵ�ַ
	
	unsigned char len;  //���ݳ���
	unsigned char size;  //�����������ߴ�
	
	unsigned char *data;  //����λ
}Queue_TypeDef;  //���ݶ��нṹ��

typedef Queue_TypeDef* pQueue_TypeDef;  //ָ����нṹ���ָ��

void queue_init(pQueue_TypeDef queue, unsigned char size);
void queue_delete(pQueue_TypeDef queue);

void queue_write(pQueue_TypeDef queue, unsigned char *pdata, unsigned char len);
void queue_read(pQueue_TypeDef queue, unsigned char *pdata, unsigned char len);

unsigned char get_queue_len(pQueue_TypeDef queue);
unsigned char queue_data_sum(pQueue_TypeDef queue);

#endif
