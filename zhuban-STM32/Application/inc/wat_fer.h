#ifndef _WAT_FER_H_
#define _WAT_FER_H_

typedef struct
{
//    float wat;
    float fer;  //灌溉   1 浇肥  2 关闭  0 浇水
    float wat1; //水泵1
    float wat2;//   水泵2
    float wat3;//   水泵3
    float magnetic;//电磁阀加料
    float blender;  //搅拌机
    float insecticide; //杀虫
    float ventilate; //通风
    float light;   //补光
    float high_wat; //高水位
    float low_wat;  //低水位
    float yang;  //遮阳
    float con;  //手动
    float wat4;  //水帘
    float video;  //视频
}WAT_FER;


void wat_fer_init();
void fer_control();
void wat_control();
void part_control();
void yang_control();
void wat_fer_packet();

#endif