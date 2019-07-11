#include "bsp_iic_master.h"

static void delay_us(unsigned int utime);

static void IIC_Master_Start(unsigned char type);
static void IIC_Master_Stop(unsigned char type);

static void IIC_Master_SendAck(unsigned char type, unsigned char ack);
static unsigned char IIC_Master_ReceiveAck(unsigned char type);

static unsigned char IIC_Master_SendByte(unsigned char type, unsigned char byte);
static unsigned char IIC_Master_ReceiveByte(unsigned char type);

void delay_us(unsigned int utime)
{
  while (utime--)
  {
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
  }
}

void my_delay_ms(unsigned int mtime)
{
  while (mtime--)
  {
    delay_us(1000);
  }
}

//��ʼ��Ϊ����״̬��scl��sda��Ϊ�ߣ�scl�ߵ�ƽ�շ��ȶ�sda��ƽ���͵�ƽ���ܸ�sda��ƽ
void IIC_Master_Init(unsigned char type)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_DIR_OUT();  //SDA���
            IIC_MASTER_SCL_DIR_OUT();
            IIC_MASTER_SDA_UP();  //SDA�������룬���������0Ϊ����
            IIC_MASTER_SCL_UP();  //SCL�������룬�������
            IIC_MASTER_SDA_IQ_OFF();  //SDA��ֹ�����жϣ��������
            IIC_MASTER_SCL_IQ_OFF();  //SCL��ֹ�����жϣ��������
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SDA_OUT(HIGH);  //SCL�ߵ�ƽ��SDA�������൱�ڷ�STOP�ź�
        break;
        
        case 2 :
            IIC_MASTER2_SDA_DIR_OUT();  //SDA���
            IIC_MASTER2_SCL_DIR_OUT();
            IIC_MASTER2_SDA_UP();  //SDA�������룬���������0Ϊ����
            IIC_MASTER2_SCL_UP();  //SCL�������룬�������
            IIC_MASTER2_SDA_IQ_OFF();  //SDA��ֹ�����жϣ��������
            IIC_MASTER2_SCL_IQ_OFF();  //SCL��ֹ�����жϣ��������
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SDA_OUT(HIGH);  //SCL�ߵ�ƽ��SDA�������൱�ڷ�STOP�ź�
        break;
        
        case 3 :
            IIC_MASTER3_SDA_DIR_OUT();  //SDA���
            IIC_MASTER3_SCL_DIR_OUT();
            IIC_MASTER3_SDA_UP();  //SDA�������룬���������0Ϊ����
            IIC_MASTER3_SCL_UP();  //SCL�������룬�������
            IIC_MASTER3_SDA_IQ_OFF();  //SDA��ֹ�����жϣ��������
            IIC_MASTER3_SCL_IQ_OFF();  //SCL��ֹ�����жϣ��������
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SDA_OUT(HIGH);  //SCL�ߵ�ƽ��SDA�������൱�ڷ�STOP�ź�
        break;
        
        default :
        break;
    }
}

//��ʼ�źţ���SCL���ڸߵ�ƽʱ��SDA�ɸߵ�ƽ��ɵ͵�ƽʱ
void IIC_Master_Start(unsigned char type)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(HIGH);
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SDA_OUT(LOW);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(HIGH);
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SDA_OUT(LOW);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(HIGH);
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SDA_OUT(LOW);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        default :
        break;
    }
}

//ֹͣ�źţ���SCL���ڸߵ�ƽʱ��SDA�ɵ͵�ƽ��ɸߵ�ƽ
void IIC_Master_Stop(unsigned char type)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(LOW);
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SDA_OUT(HIGH);
            delay_us(DELAY_US_IIC);
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(LOW);
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SDA_OUT(HIGH);
            delay_us(DELAY_US_IIC);
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(LOW);
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SDA_OUT(HIGH);
            delay_us(DELAY_US_IIC);
        break;
        
        default :
        break;
    }
}

//����Ӧ���źţ�0:ACK��1:NAK
void IIC_Master_SendAck(unsigned char type, unsigned char ack)
{
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(ack);
            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(ack);
            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER2_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(ack);
            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            IIC_MASTER3_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);
        break;
        
        default :
        break;
    }
}

//����Ӧ���źţ�0:ACK��1:NAK
unsigned char IIC_Master_ReceiveAck(unsigned char type)
{
    unsigned char ack = 1;
    
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(HIGH);

            IIC_MASTER_SDA_DIR_IN();

            IIC_MASTER_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            ack = IIC_MASTER_SDA_PIN_IN;
            IIC_MASTER_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);

            IIC_MASTER_SDA_DIR_OUT();
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(HIGH);

            IIC_MASTER2_SDA_DIR_IN();

            IIC_MASTER2_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            ack = IIC_MASTER2_SDA_PIN_IN;
            IIC_MASTER2_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);

            IIC_MASTER2_SDA_DIR_OUT();
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(HIGH);

            IIC_MASTER3_SDA_DIR_IN();

            IIC_MASTER3_SCL_OUT(HIGH);
            delay_us(DELAY_US_IIC);
            ack = IIC_MASTER3_SDA_PIN_IN;
            IIC_MASTER3_SCL_OUT(LOW);
            delay_us(DELAY_US_IIC);

            IIC_MASTER3_SDA_DIR_OUT();
        break;
        
        default :
        break;
    }

    return ack;
}

//����һ�ֽڣ�����Ӧ���ź�
unsigned char IIC_Master_SendByte(unsigned char type, unsigned char byte)
{
    unsigned char i, bit = 1;
    
    switch(type)
    {
        case 1 :
            for(i = 0; i < 8; i++)
            {
                bit = !!(byte & (0x80 >> i));  //ȡ�߼�ֵ0��1������P0_0=bitʱ��ֻ��ȡbit���ݵ����λbit0��ֵ
                IIC_MASTER_SDA_OUT(bit);
                delay_us(DELAY_US_IIC);
                IIC_MASTER_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                IIC_MASTER_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }
        break;
        
        case 2 :
            for(i = 0; i < 8; i++)
            {
                bit = !!(byte & (0x80 >> i));
                IIC_MASTER2_SDA_OUT(bit);
                delay_us(DELAY_US_IIC);
                IIC_MASTER2_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                IIC_MASTER2_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }
        break;
        
        case 3 :
            for(i = 0; i < 8; i++)
            {
                bit = !!(byte & (0x80 >> i));
                IIC_MASTER3_SDA_OUT(bit);
                delay_us(DELAY_US_IIC);
                IIC_MASTER3_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                IIC_MASTER3_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }
        break;
        
        default :
        break;
    }

    return IIC_Master_ReceiveAck(type);
}

//����һ�ֽ�
unsigned char IIC_Master_ReceiveByte(unsigned char type)
{
    unsigned char i, byte = 0;
    
    switch(type)
    {
        case 1 :
            IIC_MASTER_SDA_OUT(HIGH);  //�ͷ������ߣ�׼����ȡ����

            IIC_MASTER_SDA_DIR_IN();

            for(i = 0; i < 8; i++)
            {
                IIC_MASTER_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                byte |= IIC_MASTER_SDA_PIN_IN << (7 - i);
                IIC_MASTER_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }

            IIC_MASTER_SDA_DIR_OUT();
        break;
        
        case 2 :
            IIC_MASTER2_SDA_OUT(HIGH);  //�ͷ������ߣ�׼����ȡ����

            IIC_MASTER2_SDA_DIR_IN();

            for(i = 0; i < 8; i++)
            {
                IIC_MASTER2_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                byte |= IIC_MASTER2_SDA_PIN_IN << (7 - i);
                IIC_MASTER2_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }

            IIC_MASTER2_SDA_DIR_OUT();
        break;
        
        case 3 :
            IIC_MASTER3_SDA_OUT(HIGH);  //�ͷ������ߣ�׼����ȡ����

            IIC_MASTER3_SDA_DIR_IN();

            for(i = 0; i < 8; i++)
            {
                IIC_MASTER3_SCL_OUT(HIGH);
                delay_us(DELAY_US_IIC);
                byte |= IIC_MASTER3_SDA_PIN_IN << (7 - i);
                IIC_MASTER3_SCL_OUT(LOW);
                delay_us(DELAY_US_IIC);
            }

            IIC_MASTER3_SDA_DIR_OUT();
        break;
        
        default :
        break;
    }

    return byte;
}

//д���ݵ��豸�Ĵ������ɹ�����0
unsigned char IIC_Master_Write(unsigned char type, unsigned char device_adr, unsigned char reg_adr, unsigned char * data, unsigned char len)
{
    unsigned char i;
    
    IIC_Master_Start(type);

    if(IIC_Master_SendByte(type, device_adr + 0))  //�豸��ַ + д�źţ�bit0=0Ϊд��bit0=1Ϊ��
    {
        IIC_Master_Stop(type);
        return 1;
    }
    if(IIC_Master_SendByte(type, reg_adr))  //�Ĵ�����ַ
    {
        IIC_Master_Stop(type);
        return 1;
    }
    for(i = 0; i < len; i++)
    {
        if(IIC_Master_SendByte(type, *(data++)))  //�Ĵ�������
        {
            IIC_Master_Stop(type);
            return 1;
        }
    }

    IIC_Master_Stop(type);
    return 0;
}

//���豸�Ĵ������ȷ�д�ź��ٷ�������ʧ����������
void IIC_Master_Read(unsigned char type, unsigned char device_adr, unsigned char reg_adr, unsigned char * data, unsigned char len)
{
    unsigned char i;
    
    IIC_Master_Start(type);
    if(IIC_Master_SendByte(type, device_adr + 1))  //�豸��ַ + ���źţ�bit0=0Ϊд��bit0=1Ϊ��
    {
        IIC_Master_Stop(type);
        return;
    }
    for(i = 0; i < len; i++)
    {
        *(data + i) = IIC_Master_ReceiveByte(type);
        if(i < len - 1)
            IIC_Master_SendAck(type, 0);
    }
    IIC_Master_SendAck(type, 1);

    IIC_Master_Stop(type);
}
