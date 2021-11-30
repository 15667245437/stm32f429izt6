#ifndef _OLED_H
#define _OLED_H
#include "sys.h"

void WriteCmd(unsigned char command);
void WriteDat(unsigned char data);
void OLED_Init(void);
void OLED_Refresh_Gram(void);	//ˢдgram��ÿ�θ�����Ļʱ����Ҫʹ��
void OLED_CLS(void);	//������Ļ��
void OLED_SetPos(unsigned char x, unsigned char y,unsigned char t);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size);	//��ʾ�ַ�
void OLED_ShowString(unsigned char x,unsigned char y,const unsigned char *p,unsigned char size);	//��ʾ�ַ���
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);	//����
void oled_printf(unsigned char* strings, u8 rows);	//��oled��ӡ�ַ���

#endif
