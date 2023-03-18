#include<reg52.h>

#ifndef _LCD_H
#define _LCD_H

sbit DU = P2^0; //����ܶ�ѡ
sbit WE = P2^1; //�����λѡ
sbit CS88 = P2^2; //8X8����ʹ��
sbit DS1302=P2^7; //HJ-C52���� ʱ�Ӹ�λ �ص�DS1302 ����Ӱ������IO��ʵ�顣
sbit LCD1602=P2^5;//HJ-C52���� LCD1602��λ �ص�LCD1602��ʾ ����Ӱ������IO��ʵ�顣
sbit LCD1602_RS = P1^0;	//λ���壬Һ��������/����ѡ��
sbit LCD1602_RW = P1^1;	//λ���壬Һ���Ķ�дѡ��
sbit LCD1602_EN = P2^5;	//λ���壬Һ��ʹ���ź�

#define LCDPORT P0	  //Һ�������ݿ�
void hj_c52();  //HJ-C52��λ����

#define LCDPORT P0	  //Һ�������ݿ�

unsigned char code ucForum[]=" www.hjmcu.com ";	  //��CODE������һ��������ʾ�ĳ����ַ���

void Delay(unsigned int uiCount);	//��ʱ����
void LCD1602_CheckBusy(void);	//Һ��æ���
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//��Һ����д���ݻ���д���0Ϊ���1Ϊ����
void LCD1602_Init(void);	//Һ����ʼ��
void LCD1602_MoveToPosition(unsigned char x,unsigned char y);	//Һ���������ƶ���ָ��λ��
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData);	//��Һ��ָ��λ����ʾ�ַ�
void LCD1602_DisplayString(unsigned char *ucStr);	//��Һ������ʾ�ַ���
void DispDecint(int dat,unsigned char len);//��Һ������ʾ��������
void LCD1602_CheckBusy(void)		
{
	unsigned char i = 255;
	LCDPORT = 0xFF;  //��֮ǰ����λ��׼����ȡIO������
	LCD1602_RS = 0;
	LCD1602_RW = 1;	 //ʹҺ�����ڶ�����״̬
	LCD1602_EN = 1;	 //ʹ��Һ�����ߵ�ƽ��Ч
	while((i--) && (LCDPORT & 0x80));	//æ���
	LCD1602_EN = 0;
}

/******************************************************************************
�������ƣ�LCD1602_WriteInformation
�������ܣ���LCD1602Һ��д�����ݻ�������
��ڲ�����ucData-Ҫд������ݻ����������
		  bComOrData-Ϊ0ʱд���ʱ���Ϊ1ʱд���ʱ����
����ֵ����
��ע����
*******************************************************************************/
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData)	 
{
	LCD1602_CheckBusy();	 //��д�����ݻ�������ǰ�Ƚ���æ���
	LCDPORT = ucData;		 //�Ƚ����ݻ�����������IO
	LCD1602_RS = bComOrData;	//ȷ����д�����ݻ���д����
	LCD1602_RW = 0;		//ʹҺ������д����Ϣ״̬
	LCD1602_EN = 1;		//ʹ��Һ�����ߵ�ƽ��Ч
	LCD1602_EN = 0;
}

/******************************************************************************
�������ƣ�LCD1602_Init
�������ܣ�Һ����ʼ������
��ڲ�������
����ֵ����
��ע����
*******************************************************************************/
void LCD1602_Init(void)		 
{
	LCD1602_WriteInformation(0x38,0);
	Delay(300);
	LCD1602_WriteInformation(0x38,0);
	Delay(100);
	LCD1602_WriteInformation(0x38,0);
	Delay(100);
	LCD1602_WriteInformation(0x38,0); 	//д�����5x7��������ʽ��8λ���ݽӿ�
	Delay(100);
	LCD1602_WriteInformation(0x0c,0);	//��ʾ���ã�����ʾ����겻��ʾ������˸
	Delay(20);
	LCD1602_WriteInformation(0x01,0);	//����ָ��
	Delay(20);
}

/******************************************************************************
�������ƣ�LCD1602_MoveToPosition
�������ܣ���Һ���Ĺ���ƶ���ָ����λ��
��ڲ�����x-Һ����ʾ����������Χ0-1
			x = 0:��Һ���ĵ�һ��
			x = 1:��Һ���ĵڶ���
		  y-Һ����ʾ����������Χ0-15
		    y = 0:��Һ���ĵ�һ��
			y = 1:��Һ���ĵڶ���
			......
			y = 15:��Һ���ĵ�ʮ����
����ֵ����
��ע��ͨ��ָ��x,y��ֵ���Խ�Һ���Ĺ���ƶ���ָ����λ��
*******************************************************************************/
void LCD1602_MoveToPosition(unsigned char x,unsigned char y)	
{
	if(0 == x)
		LCD1602_WriteInformation((0x80 | y),0);	   //��궨λ����һ�е�y��
	if(1 == x)
		LCD1602_WriteInformation((0xC0 | y),0);	   //��궨�嵽�ڶ��е�y��
}

/******************************************************************************
�������ƣ�LCD1602_DisplayOneCharOnAddr
�������ܣ���ָ����λ������ʾָ�����ַ�
��ڲ�����x-Һ����ʾ����������Χ0-1
			x = 0:��Һ���ĵ�һ��
			x = 1:��Һ���ĵڶ���
		  y-Һ����ʾ����������Χ0-15
		    y = 0:��Һ���ĵ�һ��
			y = 1:��Һ���ĵڶ���
			......
			y = 15:��Һ���ĵ�ʮ����
		  ucData-Ҫ��ʾ���ַ�����
����ֵ����
��ע��ȷ��x,y��ȡֵҪ��ָ���ķ�Χ��
*******************************************************************************/
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData)
{
	LCD1602_MoveToPosition(x,y);   //���λ��
	LCD1602_WriteInformation(ucData,1);	  //д������
}

/******************************************************************************
�������ƣ�LCD1602_DisplayString
�������ܣ���ʾ�ַ���
��ڲ�����ucStr-�ַ������׵�ַ
����ֵ����
��ע����
*******************************************************************************/
void LCD1602_DisplayString(unsigned char *ucStr)	
{
	while(*ucStr != '\0')	   //�ַ�������֮ǰ��ѭ����ʾ
	{
		 LCD1602_WriteInformation(*ucStr,1);	 //����д��ÿһ���ַ�
		 ucStr++;								 //ָ������
	}
}
/******************************************************************************/
void DispDecint(int dat,unsigned char len)
{
	char str[12];
	unsigned char i,dl;
	str[len]=0;
	for(i=0;i<len;i++)
	{
		dl=dat%10;
		dat/=10;
		str[len-i-1]=dl+'0';
	}
	LCD1602_DisplayString(str);
}
/******************************************************************************
�������ƣ�Delay
�������ܣ���ʱ����
��ڲ�����uiCount-��ʱ������ÿ��1����0.5ms
����ֵ����
��ע����
*******************************************************************************/
void Delay(unsigned int uiCount)
{
	unsigned char j = 244;
	for(;uiCount > 0;uiCount--) while(--j);	
}


/******************************************************************/
// ����ΪHJ-C52��λ����
/******************************************************************/
    void hj_c52()
    {
    LCD1602=0;//�ص�LCD1602 ����Ӱ������IO�ڵ���
    DS1302=0;//�ص�DS1302 ����Ӱ������IO�ڵ���
    CS88=0;     //�ص�8X8������ʾ
    WE=1;  
    P0=0Xff;
    WE=0;        //�ص��������ʾ
    CS88=0;      //�ص�8X8������ʾ
    }
/******************************************************************/  

#endif