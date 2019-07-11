#include "wat_fer.h"
#include "update.h"
#include "sys.h"
#include "timer.h"
#include "rs485_deal.h"
#include "data.h"
#include "mem.h"
#include "wdg.h"
extern WAT_FER wat_fer;

extern char wat_start_flag;
extern char wat_flag ;

//char fer_start_flag =0 ;
//int fer_cnt =0;
//char fer_flag = 0;

extern char magnetic_start_flag;
extern char magnetic_flag;

extern char blender_start_flag;
extern char blender_flag;
static char step =1;

extern ST_ITEM *st_item;
extern Send_485 send_485;
static char wash_flag;

/***************************************
*@DESCRIPTION: --ˮ��һ�廯�г�ʼ��ˮ��һ�廯IO
*
*@Input: --��
*
*@Return: --��
****************************************/

void wat_fer_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);
  GPIO_SetBits(GPIOB,GPIO_Pin_9|GPIO_Pin_8);
  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
//  GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_9);
  
   GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12);
   GPIO_ResetBits(GPIOC,GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7);
//     GPIO_SetBits(GPIOC,GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1);
    
    
    wat_fer.fer = 2;//���״̬�����κζ�����Ϊ2����ˮΪ0������Ϊ1

 }

/***************************************
*@DESCRIPTION: --ˮ��һ�廯�н�ˮ�����߼�
*
*@Input: --��
*
*@Return: --��
****************************************/


void wat_control()
{

   if(wat_fer.fer == 0)
   {
       wat_fer.wat1=1; 
      if(wat_start_flag == 0)
  {
     send();	//��������
}
       wat_start_flag=1;
   
     if(wat_flag)
     {
       
        wat_fer.wat1 = 0;
        wat_fer.fer = 2 ;
        wat_start_flag =0;
        wat_flag =0;
        send();	//��������
     }
   }
   
}

/***************************************
*@DESCRIPTION: --ˮ��һ�廯�н��ʿ����߼�
*
*@Input: --��
*
*@Return: --��
****************************************/

void fer_control()
{  
      u8 ReadValue,ReadValue1; 
   
      ReadValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);
      ReadValue1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);
      wat_fer.high_wat=ReadValue;
      wat_fer.low_wat =ReadValue1;
     
        if(wat_fer.fer == 1)
        {
            if(step ==1)
            {
                if(ReadValue)
                {
                    wat_fer.wat1 = 0;  //�رս�ˮˮ��
                    wat_fer.wat2 = 1;  //�򿪹�Ͱˮ��   
                    send();	//��������
                }
                step+=1;
            }

          if(step ==2)
          {
            if(!ReadValue)
            {
               
                wat_fer.wat2 = 0;  //�رչ�Ͱˮ��
               if(wash_flag == 0)
               {
               
                  wat_fer.magnetic =1;   //������
                  
                  
               }
               wash_flag++;
                magnetic_start_flag =1;
                step+=1;
                send();	//��������
            
            }
          }
          if(step ==3)
          {
              if(magnetic_flag)
              {
              
                  
                    wat_fer.magnetic =0;      //�رռ�����       
                    wat_fer.blender =1;     //�򿪽����
                   
                    blender_start_flag = 1;
                    magnetic_flag =0;
                    magnetic_start_flag =0;
                    step+=1;
                    send();	//��������
                }
                          
          }
          if(step == 4)
          {
              if(blender_flag)
              {     
                if(!ReadValue1)
                {
                   wat_fer.blender =0;  //�رս����   
                    wat_fer.wat3 = 1;   //�򿪽���ˮ��
                    blender_flag =0;
                    blender_start_flag=0;
                     step+=1;
                     send();	//��������
                }
              }
          }
         if(step == 5)
         {
            if(ReadValue1)
            {
                wat_fer.wat3 = 0;   //�رս���ˮ��   
                if(wash_flag == 2)
                {
                  wat_fer.fer = 2;  
                  wash_flag = 0;
                }
                step=1;
                send();	//��������
            }
         }
       }
      else
       {
  //       wat_fer.wat3 = 0;   //�رս���ˮ��
 //        wat_fer.magnetic =0;      //�رռ�����  
 //        wat_fer.blender =0;  //�رս����   
 //        wat_fer.wat2 = 0;  //�رչ�Ͱˮ��
         step=1;
       }   
     
 }
 int light_time = 0;
 int light_time1 = 0;
 /*��Ӿ����Զ����ƹ��ܣ�������ǿ�����������Ӵ���100ʱ�������ر�
������ǿ������������С��100ʱ��������*/
 void yang_control()
 {
   if(wat_fer.con == 0)//0���Զ�
   {
     if(LIGHT_DATA > 100) //���ڹ���
     {
       light_time ++;
       light_time1 = 0;
       if(light_time > 600)
       {
//         USART_Puts(USART3,yang_off,7);
         light_time = 0;
         wat_fer.yang = 1;
       }
     }
     else
     {
       light_time1 ++ ;
       light_time = 0;
       if(light_time1 > 600)
       {
//         USART_Puts(USART3,yang_on,7);
         light_time1 = 0;
         wat_fer.yang = 0; 
       }
     }
   }
 }
/***************************************
*@DESCRIPTION: --ˮ��һ�廯�п��ƶ�ӦIO
*
*@Input: --��
*
*@Return: --��
****************************************/
 
void part_control()
{
  if(wat_fer.wat1 == 0)
  {
      GPIO_ResetBits(GPIOB,GPIO_Pin_7);//�ر�ˮ��1����ˮ��
  }
  if(wat_fer.wat1 == 1)
  {
     GPIO_SetBits(GPIOB,GPIO_Pin_7);//����ˮ��1����ˮ��
  }
  
   if(wat_fer.wat2 == 0)
  {
      GPIO_ResetBits(GPIOB,GPIO_Pin_6);//�ر�ˮ��2 ��ˮ��Ͱ��
  }
  if(wat_fer.wat2 == 1)
  {
     GPIO_SetBits(GPIOB,GPIO_Pin_6);//����ˮ��2��ˮ��Ͱ��
     if(wat_fer.high_wat == 0)
                {
                          wat_fer.wat2 = 0;
                          GPIO_ResetBits(GPIOB,GPIO_Pin_6);//�ر�ˮ��2 ��ˮ��Ͱ��
                          send();	//��������
                }
  }
  
  if(wat_fer.wat3 == 0)
  {
     GPIO_ResetBits(GPIOC,GPIO_Pin_3);  //�ر�ˮ��3 �����ʣ�
     
  }
    
  if(wat_fer.wat3 == 1)
  {
    GPIO_SetBits(GPIOC,GPIO_Pin_3);    //����ˮ��3�����ʣ�
    if(wat_fer.low_wat == 1)
                        {
                            wat_fer.wat3 = 0;
                            GPIO_ResetBits(GPIOC,GPIO_Pin_3);  //�ر�ˮ��3 �����ʣ�
                            send();	//��������
                        }
  }
    
   if(wat_fer.magnetic == 0)
  {
     GPIO_ResetBits(GPIOC,GPIO_Pin_2);   //  �رռ��ϵ�ŷ�
  }
   if(wat_fer.magnetic == 1)
  {
     GPIO_SetBits(GPIOC,GPIO_Pin_2);   //    �������ϵ�ŷ�
     
  }
  
  if(wat_fer.blender == 0)
  {
     GPIO_ResetBits(GPIOC,GPIO_Pin_1);   //  �رս����
  }
   if(wat_fer.blender == 1)
  {
     GPIO_SetBits(GPIOC,GPIO_Pin_1);   //  ���������
    if(wat_fer.high_wat == 1)
                        {
                          wat_fer.blender = 0;
  GPIO_ResetBits(GPIOC,GPIO_Pin_1);   //  �رս����
      send();	//��������
                        }
  }
  
    if(wat_fer.ventilate == 0)
  {
     GPIO_ResetBits(GPIOA,GPIO_Pin_8);   //  �ر�ͨ��
  }
   if(wat_fer.ventilate == 1)
  {
     GPIO_SetBits(GPIOA,GPIO_Pin_8);   //  ����ͨ��
     
  }
  
  
    if(wat_fer.light == 0)
  {
     GPIO_ResetBits(GPIOA,GPIO_Pin_11);   //  �رղ���
  }
   if(wat_fer.light == 1)
  {
     GPIO_SetBits(GPIOA,GPIO_Pin_11);   //  ��������
     
  }
  
    if(wat_fer.insecticide == 0)
  {
     GPIO_ResetBits(GPIOA,GPIO_Pin_12);   //  �ر�ɱ��
  }
   if(wat_fer.insecticide == 1)
  {
     GPIO_SetBits(GPIOA,GPIO_Pin_12);   //  ����ɱ��
  }
  
    if(wat_fer.wat4 == 0)
  {
     GPIO_ResetBits(GPIOC,GPIO_Pin_6);   //  �ر�ˮ��
  }
   if(wat_fer.wat4 == 1)
  {
     GPIO_SetBits(GPIOC,GPIO_Pin_6);   //  ����ˮ��
  }
  
    if(wat_fer.video == 0)
  {
     GPIO_ResetBits(GPIOC,GPIO_Pin_7);   //  �ر���Ƶ
  }
   if(wat_fer.video == 1)
  {
     GPIO_SetBits(GPIOC,GPIO_Pin_7);   //  ������Ƶ
  }
  
  
 }



//unsigned char wat_fer_addr[11]={0x0120,0x0122,0x0124,0x0126,0x0130,0x0132,0x0134,0x0136,0x0138,0x0140,0x0142};
unsigned char ventilate_addr[] ={0x01,0x20};
unsigned char light_addr[] ={0x01,0x22};
unsigned char insecticide_addr[] ={0x01,0x24};
unsigned char fer_addr[] ={0x01,0x26};
unsigned char wat1_addr[] ={0x01,0x30};
unsigned char wat2_addr[] ={0x01,0x32};
unsigned char wat3_addr[] ={0x01,0x34};
unsigned char blender_addr[] ={0x01,0x36};
unsigned char magnetic_addr[] ={0x01,0x38};
unsigned char high_wat_addr[] ={0x01,0x40};
unsigned char low_wat_addr[] ={0x01,0x42};
unsigned char yang_addr[] ={0x01,0x44};
unsigned char con_addr[] ={0x01,0x46}; 
unsigned char wat4_addr[] ={0x01,0x48}; 
unsigned char video_addr[] ={0x01,0x4A}; 
/***************************************
*@DESCRIPTION: --��ˮ��һ�廯״̬���뵽�ϴ�������
*
*@Input: --��
*
*@Return: --��
****************************************/

void wat_fer_packet()
{
  int i;
   
   for(i=0;i<5;i++)
   {
      switch(i)
      {
        IWDG_Feed();
        case 0:
          mem_copy(st_item[send_485.cnt+i].key,ventilate_addr,2);////ͨ��
          mem_copy(st_item[send_485.cnt+i].dat,(unsigned char*)&wat_fer.ventilate,4);
          break;
        case 1:
          mem_copy(st_item[send_485.cnt+i].key, light_addr,2);
          mem_copy(st_item[send_485.cnt+i].dat,(unsigned char*)&wat_fer.light,4);
          break;
        case 2:
          mem_copy(st_item[send_485.cnt+i].key,insecticide_addr,2);
          mem_copy(st_item[send_485.cnt+i].dat,(unsigned char*)&wat_fer.insecticide,4);
          break;

         case 3:
           mem_copy(st_item[send_485.cnt+i].key,wat4_addr,2);
          mem_copy(st_item[send_485.cnt+i].dat,(unsigned char*)&wat_fer.wat4,4);
          break;
         case 4:
           mem_copy(st_item[send_485.cnt+i].key,video_addr,2);
          mem_copy(st_item[send_485.cnt+i].dat,(unsigned char*)&wat_fer.video,4);
          break;
      default:
          break;
      }
   } 
}

