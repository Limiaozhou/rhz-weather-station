
/****—” ± 2us  4M  ±÷”********/
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