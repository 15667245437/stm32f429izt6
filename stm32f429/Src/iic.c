#include "iic.h"
#include "delay.h"

void IIC_Init()
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOH_CLK_ENABLE();
	
	GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  
	GPIO_Initure.Pull=GPIO_PULLUP;       
	GPIO_Initure.Speed=GPIO_SPEED_FAST;     
	HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	
	IIC_SCL=1;
	IIC_SDA=1;
}

void IIC_START()
{
	SDA_OUT();
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;
	delay_us(4);
	IIC_SCL=0;
}

void IIC_STOP()
{
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;
 	delay_us(4);
	IIC_SCL=1; 
	delay_us(4);			
	IIC_SDA=1;
}

u8 IIC_Wait_ACK()
{
	u8 ucErrTime=0;
	SDA_IN();
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_STOP();
			return 1;
		}
	}
	IIC_SCL=0;
	return 0;
}

void IIC_ACK()
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

void IIC_NACK()
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL=0;
	for(t=0;t<8;t++)
	{              
		IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	  
		delay_us(2);  
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
	}	 
}

u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
	for(i=0;i<8;i++ )
	{
		IIC_SCL=0;
		delay_us(2);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA)receive++;   
		delay_us(1); 
   }					 
	if (!ack)	IIC_NACK();
	else	IIC_ACK();  
	return receive;
}

