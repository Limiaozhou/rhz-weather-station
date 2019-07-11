#include "bsp_usart.h"

Queue_TypeDef Queue_Uart2_Rx;  //uart1接收数据缓存队列
Queue_TypeDef Queue_Uart2_Tx;  //uart1发送数据缓存队列

Queue_TypeDef Queue_Uart2_Rx_Num;  //uart1接收帧长队列，保存每帧长度

static void Uart_Port_Init(UARTx_Select_TypeDef uartx, unsigned long int bound);
static void Uart_Queue_Init(UARTx_Select_TypeDef uartx);

/* 串口初始化，包涵端口、DMA和队列初始化 */
void Uart_Init(UARTx_Select_TypeDef uartx, unsigned long int bound)
{
	Uart_Queue_Init(uartx);
	Uart_Port_Init(uartx, bound);
}

/* 串口端口初始化
 *如波特率9600：16M / 9600 = 1667 = 0683h
 *BRR1：68h
 *BRR2：03h
 */
static void Uart_Port_Init(UARTx_Select_TypeDef uartx, unsigned long int bound)
{
    unsigned long int clock_uart = 0;  //串口时钟频率
    unsigned long int uart_div = 0;  //分频倍数
    unsigned char brr1 = 0, brr2 = 0;  //波特率寄存器值缓存
    
    clock_uart = CLK_GetMasterClock_Freq();  //获取串口时钟频率
    uart_div = (unsigned long int)((float)clock_uart / bound + 0.5);  //计算分频倍数，四舍五入
    brr1 = (uart_div & 0x0FF0) >> 4;  //波特率寄存器BRR1值缓存
    brr2 = ((uart_div & 0xF000) >> 8) | (uart_div & 0x0F);  //波特率寄存器BRR2值缓存
    
	switch(uartx)
	{
		case UART1_Select :
            //STM8
            UART1_CR1 &= ~0x10;  //M，一个起始位，8个数据位，无校验
            UART1_CR3 |= 0x00;  //STOP，1个停止位
            UART1_BRR2 = brr2;
            UART1_BRR1 = brr1;
            UART1_CR2 |= 0x04;  //REN，接收使能，SR:RXNE
            UART1_CR2 |= 0x20;  //RIEN，接收中断使能，SR:RXNE
            UART1_CR2 |= 0x10;  //ILIEN，接收空闲中断使能，SR:IDLE
            UART1_CR2 |= 0x08;  //TEN，发送使能，SR:TXE,TC
        break;
		
        case UART2_Select :
        break;
        
        case UART3_Select :
        break;
        
		default :
		break;
	}
}

/* 串口队列初始化 */
static void Uart_Queue_Init(UARTx_Select_TypeDef uartx)
{
	switch(uartx)
	{
		case UART1_Select :
			queue_init(&Queue_Uart2_Rx, 100);
			queue_init(&Queue_Uart2_Tx, 100);
			
			queue_init(&Queue_Uart2_Rx_Num, 10);
		break;
		
		default :
		break;
	}
}

/* 串口发送， num：串口号， pdata：数据指针， len：数据量 */
void uart_write(UARTx_Select_TypeDef uartx, unsigned char *pdata, unsigned char len)
{
	switch(uartx)
	{
		case UART1_Select :
		{
			while(len--)
			{
                //STM8
                UART1_DR = *(pdata++);  //发送数据
                while(!UART1_SR_TXE);  //发送寄存器空标志，发完和复位值为1，写UART1_DR会清0
                UART3_DR = *(pdata++);  //发送数据
			}
		}break;
		
		default :
			break;
	}
}

/* 串口数据读取处理， num：串口号， callback：指向用户数据处理函数的回调函数 */
void uart_read(UARTx_Select_TypeDef uartx, data_deal_cb callback)
{
	unsigned char rdata[20] = {0};  //队列数据缓存数组
	unsigned char len, len_num;  //每帧长、帧数
	
	switch(uartx)
	{
		case UART1_Select :
		{
			len_num = Queue_Uart2_Rx_Num.len;  //帧数
			
			while(len_num--)  //完整取去每帧数据
			{
				queue_read(&Queue_Uart2_Rx_Num, &len, 1);  //每帧长
				if(len)
				{
					queue_read(&Queue_Uart2_Rx, rdata, len);  //取出一帧数据保存到rdata数组
					callback(rdata, len);  //将数据指针和帧长传给回调函数去解析
				}
			}
		}break;
		
		default :
			break;
	}
}

//STM8 串口接收中断函数
#pragma vector=UART2_R_OR_vector  //20
__interrupt void UART1_R_OR_IRQHandler(void)
{
    unsigned char data = 0;  //数据缓存
    unsigned char len_now = 0;  //当前帧长度
    
    if(UART1_SR_RXNE)  //接收标志位
    {
        data = UART1_DR;  //读DR同时会清0接收标志位
        queue_write(&Queue_Uart2_Rx, &data, 1);  //将读出的数据保存到接收队列
    }
    
    if(UART1_SR_IDLE)  //空闲标志位
    {
        data = UART1_SR;
        data = UART1_DR;  //顺序读完后，清0空闲IDLE标志位
        
        len_now = Queue_Uart2_Rx.len - queue_data_sum(&Queue_Uart2_Rx_Num);  //当前帧长度 = 当前数据队列长度 - 已保存的总长度
        queue_write(&Queue_Uart2_Rx_Num, &len_now, 1);  //保存当前数据帧长度
    }
}
