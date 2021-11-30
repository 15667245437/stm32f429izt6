#include "delay.h"
#include "main.h"


void delay_init()
{
	SysTick->CTRL|=0X04;
	SysTick->LOAD=1800-1;
	SysTick->CTRL|=0x05;
}

void delay_us(int us)
{
	unsigned short last_val=0,latest_val=0;
	int total_time=0;
	int temp=0;
	while(total_time<(us*179))
	{
		latest_val=SysTick->VAL;
		if(latest_val<last_val)
		{
			temp=last_val-latest_val;
			total_time+=temp;
		}
		else 
		{
			temp=1799-latest_val+last_val;
			total_time+=temp;
		}
		last_val=latest_val;
	}
}

void delay_ms(int ms)
{
	for(int i=0;i<ms;i++)
	{
		delay_us(1000);
	}
}
