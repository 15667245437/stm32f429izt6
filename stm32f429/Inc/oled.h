#ifndef _OLED_H
#define _OLED_H
#include "sys.h"

void WriteCmd(unsigned char command);
void WriteDat(unsigned char data);
void OLED_Init(void);
void OLED_Refresh_Gram(void);	//刷写gram。每次更新屏幕时都需要使用
void OLED_CLS(void);	//清理屏幕。
void OLED_SetPos(unsigned char x, unsigned char y,unsigned char t);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size);	//显示字符
void OLED_ShowString(unsigned char x,unsigned char y,const unsigned char *p,unsigned char size);	//显示字符串
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);	//满屏
void oled_printf(unsigned char* strings, u8 rows);	//在oled打印字符串

#endif
