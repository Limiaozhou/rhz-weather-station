#ifndef _WAT_FER_H_
#define _WAT_FER_H_

typedef struct
{
//    float wat;
    float fer;  //���   1 ����  2 �ر�  0 ��ˮ
    float wat1; //ˮ��1
    float wat2;//   ˮ��2
    float wat3;//   ˮ��3
    float magnetic;//��ŷ�����
    float blender;  //�����
    float insecticide; //ɱ��
    float ventilate; //ͨ��
    float light;   //����
    float high_wat; //��ˮλ
    float low_wat;  //��ˮλ
    float yang;  //����
    float con;  //�ֶ�
    float wat4;  //ˮ��
    float video;  //��Ƶ
}WAT_FER;


void wat_fer_init();
void fer_control();
void wat_control();
void part_control();
void yang_control();
void wat_fer_packet();

#endif