#include "oled.h"
#include "delay.h"
#include "iic.h"
#include "oledfont.h"
#include "string.h"

u8 OLED_GRAM[128][8];

void WriteCmd(u8 command)
{
	IIC_START();
	IIC_Send_Byte(0x78);
	IIC_ACK();
	IIC_Send_Byte(0x00);
	IIC_ACK();
	IIC_Send_Byte(command);
	IIC_ACK();
	IIC_STOP();
}

void WriteDat(u8 data)
{
	IIC_START();
	IIC_Send_Byte(0x78);
	IIC_ACK();
	IIC_Send_Byte(0x40);
	IIC_ACK();
	IIC_Send_Byte(data);
	IIC_ACK();
	IIC_STOP();
}

void OLED_Init(void)
{
	delay_ms(1000);
	WriteCmd(0xAE);	//display off
	WriteCmd(0x20); //Set Memory Addressing Mode
	WriteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8); //Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
  WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	OLED_CLS();
}

void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		WriteCmd (0xb0+i);    //set page address(0-7)
		WriteCmd (0x00);      //set display position-set low column address
		WriteCmd (0x10);      //set display position-set high column address
		for(n=0;n<128;n++)WriteDat(OLED_GRAM[n][i]); //circulate 128*8 times through IIC
	}   
}

void OLED_CLS(void)
{
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();
}

void OLED_SetPos(unsigned char x, unsigned char y,unsigned char t)
{
	u8 page,row,temp;
	if(x>127||y>63)	return;
	page=y/8;
	row=y%8;
	temp=1<<row;
	if(t)	OLED_GRAM[x][page]|=temp;
	else	OLED_GRAM[x][page]&=~temp;
}

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size)
{
	u8 i,t,buffer,temp,y1;
	u8 csize=(size/8+((size%8)?1:0))*(size/2); //get the number of byte of revelant size
	chr=chr-' ';//' ''s asc11 is 32, shift the input character
	y1=y;
	for(i=0;i<csize;i++)
	{
		if(csize==12) buffer=asc2_1206[chr][i];
		else if (csize==16) buffer=asc2_1608[chr][i];
		else if (csize==16) buffer=asc2_2412[chr][i];
		else return;
		for(t=0;t<8;t++)
		{
			temp=buffer&0x80;
			if(temp) OLED_SetPos(x,y,1);
			else OLED_SetPos(x,y,0);
			buffer<<=1;
			y++;
			if(y-y1==size)
			{
				y=y1;
				x++;
				break;
			}
		}
	}
}

void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{
	while(((*p<='~')&&(*p>=' '))||(*p=='\r')||(*p=='\n'))
	{       
		if(x>(128-(size/2))){x=0;y+=size;}
		if(y>(64-size)){y=x=0;}
		if((*p=='\r')||(*p=='\n')){y+=size;}
		if((*p<='~')&&(*p>=' ')){OLED_ShowChar(x,y,*p,size);}
		x+=size/2;
		p++;
	} 
	
}

void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 x,y;
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_SetPos(x,y,1);
	}
	OLED_Refresh_Gram();
}

void oled_printf(unsigned char* strings, u8 rows)
{
	OLED_CLS();
	OLED_ShowString(1,(rows-1)*12-1,strings,12);
	OLED_Refresh_Gram();
}

