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
*@DESCRIPTION: --水肥一体化中初始化水肥一体化IO
*
*@Input: --无
*
*@Return: --无
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
    
    
    wat_fer.fer = 2;//灌溉状态在无任何动作是为2，浇水为0，浇肥为1

 }

/***************************************
*@DESCRIPTION: --水肥一体化中浇水控制逻辑
*
*@Input: --无
*
*@Return: --无
****************************************/


void wat_control()
{

   if(wat_fer.fer == 0)
   {
       wat_fer.wat1=1; 
      if(wat_start_flag == 0)
  {
     send();	//发送数据
}
       wat_start_flag=1;
   
     if(wat_flag)
     {
       
        wat_fer.wat1 = 0;
        wat_fer.fer = 2 ;
        wat_start_flag =0;
        wat_flag =0;
        send();	//发送数据
     }
   }
   
}

/***************************************
*@DESCRIPTION: --水肥一体化中浇肥控制逻辑
*
*@Input: --无
*
*@Return: --无
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
                    wat_fer.wat1 = 0;  //关闭浇水水阀
                    wat_fer.wat2 = 1;  //打开灌桶水阀   
                    send();	//发送数据
                }
                step+=1;
            }

          if(step ==2)
          {
            if(!ReadValue)
            {
               
                wat_fer.wat2 = 0;  //关闭灌桶水阀
               if(wash_flag == 0)
               {
               
                  wat_fer.magnetic =1;   //加饲料
                  
                  
               }
               wash_flag++;
                magnetic_start_flag =1;
                step+=1;
                send();	//发送数据
            
            }
          }
          if(step ==3)
          {
              if(magnetic_flag)
              {
              
                  
                    wat_fer.magnetic =0;      //关闭加饲料       
                    wat_fer.blender =1;     //打开搅拌机
                   
                    blender_start_flag = 1;
                    magnetic_flag =0;
                    magnetic_start_flag =0;
                    step+=1;
                    send();	//发送数据
                }
                          
          }
          if(step == 4)
          {
              if(blender_flag)
              {     
                if(!ReadValue1)
                {
                   wat_fer.blender =0;  //关闭搅拌机   
                    wat_fer.wat3 = 1;   //打开浇肥水阀
                    blender_flag =0;
                    blender_start_flag=0;
                     step+=1;
                     send();	//发送数据
                }
              }
          }
         if(step == 5)
         {
            if(ReadValue1)
            {
                wat_fer.wat3 = 0;   //关闭浇肥水阀   
                if(wash_flag == 2)
                {
                  wat_fer.fer = 2;  
                  wash_flag = 0;
                }
                step=1;
                send();	//发送数据
            }
         }
       }
      else
       {
  //       wat_fer.wat3 = 0;   //关闭浇肥水阀
 //        wat_fer.magnetic =0;      //关闭加饲料  
 //        wat_fer.blender =0;  //关闭搅拌机   
 //        wat_fer.wat2 = 0;  //关闭灌桶水阀
         step=1;
       }   
     
 }
 int light_time = 0;
 int light_time1 = 0;
 /*添加卷帘自动控制功能，当光照强度连续两分钟大于100时，卷帘关闭
当光照强度连续两分钟小于100时，卷帘打开*/
 void yang_control()
 {
   if(wat_fer.con == 0)//0是自动
   {
     if(LIGHT_DATA > 100) //室内光照
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
*@DESCRIPTION: --水肥一体化中控制对应IO
*
*@Input: --无
*
*@Return: --无
****************************************/
 
void part_control()
{
  if(wat_fer.wat1 == 0)
  {
      GPIO_ResetBits(GPIOB,GPIO_Pin_7);//关闭水泵1（浇水）
  }
  if(wat_fer.wat1 == 1)
  {
     GPIO_SetBits(GPIOB,GPIO_Pin_7);//启动水泵1（浇水）
  }
  
   if(wat_fer.wat2 == 0)
  {
      GPIO_ResetBits(GPIOB,GPIO_Pin_6);//关闭水泵2 （水到桶）
  }
  if(wat_fer.wat2 == 1)
  {
     GPIO_SetBits(GPIOB,GPIO_Pin_6);//启动水泵2（水到桶）
     if(wat_fer.high_wat == 0)
                {
                          wat_fer.wat2 = 0;
                          GPIO_ResetBits(GPIOB,GPIO_Pin_6);//关闭水泵2 （水到桶）
                          send();	//发送数据
                }
  }
  
  if(wat_fer.wat3 == 0)
  {
     GPIO_ResetBits(GPIOC,GPIO_Pin_3);  //关闭水泵3 （浇肥）
     
  }
    
  if(wat_fer.wat3 == 1)
  {
    GPIO_SetBits(GPIOC,GPIO_Pin_3);    //启动水泵3（浇肥）
    if(wat_fer.low_wat == 1)
                        {
                            wat_fer.wat3 = 0;
                            GPIO_ResetBits(GPIOC,GPIO_Pin_3);  //关闭水泵3 （浇肥）
                            send();	//发送数据
                        }
  }
    
   if(wat_fer.magnetic == 0)
  {
     GPIO_ResetBits(GPIOC,GPIO_Pin_2);   //  关闭加料电磁阀
  }
   if(wat_fer.magnetic == 1)
  {
     GPIO_SetBits(GPIOC,GPIO_Pin_2);   //    启动加料电磁阀
     
  }
  
  if(wat_fer.blender == 0)
  {
     GPIO_ResetBits(GPIOC,GPIO_Pin_1);   //  关闭搅拌机
  }
   if(wat_fer.blender == 1)
  {
     GPIO_SetBits(GPIOC,GPIO_Pin_1);   //  启动搅拌机
    if(wat_fer.high_wat == 1)
                        {
                          wat_fer.blender = 0;
  GPIO_ResetBits(GPIOC,GPIO_Pin_1);   //  关闭搅拌机
      send();	//发送数据
                        }
  }
  
    if(wat_fer.ventilate == 0)
  {
     GPIO_ResetBits(GPIOA,GPIO_Pin_8);   //  关闭通风
  }
   if(wat_fer.ventilate == 1)
  {
     GPIO_SetBits(GPIOA,GPIO_Pin_8);   //  启动通风
     
  }
  
  
    if(wat_fer.light == 0)
  {
     GPIO_ResetBits(GPIOA,GPIO_Pin_11);   //  关闭补光
  }
   if(wat_fer.light == 1)
  {
     GPIO_SetBits(GPIOA,GPIO_Pin_11);   //  启动补光
     
  }
  
    if(wat_fer.insecticide == 0)
  {
     GPIO_ResetBits(GPIOA,GPIO_Pin_12);   //  关闭杀虫
  }
   if(wat_fer.insecticide == 1)
  {
     GPIO_SetBits(GPIOA,GPIO_Pin_12);   //  启动杀虫
  }
  
    if(wat_fer.wat4 == 0)
  {
     GPIO_ResetBits(GPIOC,GPIO_Pin_6);   //  关闭水帘
  }
   if(wat_fer.wat4 == 1)
  {
     GPIO_SetBits(GPIOC,GPIO_Pin_6);   //  启动水帘
  }
  
    if(wat_fer.video == 0)
  {
     GPIO_ResetBits(GPIOC,GPIO_Pin_7);   //  关闭视频
  }
   if(wat_fer.video == 1)
  {
     GPIO_SetBits(GPIOC,GPIO_Pin_7);   //  启动视频
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
*@DESCRIPTION: --将水肥一体化状态加入到上传数据中
*
*@Input: --无
*
*@Return: --无
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
          mem_copy(st_item[send_485.cnt+i].key,ventilate_addr,2);////通风
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

