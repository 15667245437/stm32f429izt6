#ifndef _IIC_H
#define _IIC_H
#include "sys.h"

#define SDA_IN()  {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=0<<5*2;}	
#define SDA_OUT() {GPIOH->MODER&=~(3<<(5*2));GPIOH->MODER|=1<<5*2;}

#define IIC_SCL   PHout(4) 
#define IIC_SDA   PHout(5)
#define READ_SDA  PHin(5) 

void IIC_Init(void);
void IIC_START(void);
void IIC_STOP(void);
unsigned char IIC_Wait_ACK(void);
void IIC_ACK(void);
void IIC_NACK(void);
void IIC_Send_Byte(unsigned char txd);
unsigned char IIC_Read_Byte(unsigned char ack);
#endif
