
/****��ʱ 2us  4M ʱ��********/
void delay_2us(int t)
{
    while(t--)
    {
        asm("NOP");
       
    }
}

void delay_ms(int t) 
{
    while(t--)
    {
        delay_2us(500);
        IWDG_Feed();
    }
}