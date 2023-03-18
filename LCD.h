#include<reg52.h>

#ifndef _LCD_H
#define _LCD_H

sbit DU = P2^0; //数码管段选
sbit WE = P2^1; //数码管位选
sbit CS88 = P2^2; //8X8点阵使能
sbit DS1302=P2^7; //HJ-C52主板 时钟复位 关掉DS1302 以免影响其他IO口实验。
sbit LCD1602=P2^5;//HJ-C52主板 LCD1602复位 关掉LCD1602显示 以免影响其他IO口实验。
sbit LCD1602_RS = P1^0;	//位定义，液晶的数据/命令选择
sbit LCD1602_RW = P1^1;	//位定义，液晶的读写选择
sbit LCD1602_EN = P2^5;	//位定义，液晶使能信号

#define LCDPORT P0	  //液晶的数据口
void hj_c52();  //HJ-C52复位函数

#define LCDPORT P0	  //液晶的数据口

unsigned char code ucForum[]=" www.hjmcu.com ";	  //在CODE区定义一个用于显示的常量字符串

void Delay(unsigned int uiCount);	//延时函数
void LCD1602_CheckBusy(void);	//液晶忙检测
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData);	//在液晶上写数据或者写命令，0为命令，1为数据
void LCD1602_Init(void);	//液晶初始化
void LCD1602_MoveToPosition(unsigned char x,unsigned char y);	//液晶的坐标移动到指定位置
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData);	//在液晶指定位置显示字符
void LCD1602_DisplayString(unsigned char *ucStr);	//在液晶上显示字符串
void DispDecint(int dat,unsigned char len);//在液晶上显示整形数据
void LCD1602_CheckBusy(void)		
{
	unsigned char i = 255;
	LCDPORT = 0xFF;  //读之前先置位，准备读取IO口数据
	LCD1602_RS = 0;
	LCD1602_RW = 1;	 //使液晶处于读数据状态
	LCD1602_EN = 1;	 //使能液晶，高电平有效
	while((i--) && (LCDPORT & 0x80));	//忙检测
	LCD1602_EN = 0;
}

/******************************************************************************
函数名称：LCD1602_WriteInformation
函数功能：向LCD1602液晶写入数据或者命令
入口参数：ucData-要写入的数据或者命令参数
		  bComOrData-为0时写入的时命令，为1时写入的时数据
返回值：无
备注：无
*******************************************************************************/
void LCD1602_WriteInformation(unsigned char ucData,bit bComOrData)	 
{
	LCD1602_CheckBusy();	 //在写入数据或者命令前先进行忙检测
	LCDPORT = ucData;		 //先将数据或者命令送至IO
	LCD1602_RS = bComOrData;	//确定是写入数据还是写命令
	LCD1602_RW = 0;		//使液晶处于写入信息状态
	LCD1602_EN = 1;		//使能液晶，高电平有效
	LCD1602_EN = 0;
}

/******************************************************************************
函数名称：LCD1602_Init
函数功能：液晶初始化函数
入口参数：无
返回值：无
备注：无
*******************************************************************************/
void LCD1602_Init(void)		 
{
	LCD1602_WriteInformation(0x38,0);
	Delay(300);
	LCD1602_WriteInformation(0x38,0);
	Delay(100);
	LCD1602_WriteInformation(0x38,0);
	Delay(100);
	LCD1602_WriteInformation(0x38,0); 	//写入命令，5x7点阵工作方式，8位数据接口
	Delay(100);
	LCD1602_WriteInformation(0x0c,0);	//显示设置，开显示，光标不显示，不闪烁
	Delay(20);
	LCD1602_WriteInformation(0x01,0);	//清屏指令
	Delay(20);
}

/******************************************************************************
函数名称：LCD1602_MoveToPosition
函数功能：将液晶的光标移动到指定的位置
入口参数：x-液晶显示的行数，范围0-1
			x = 0:在液晶的第一行
			x = 1:在液晶的第二行
		  y-液晶显示的列数，范围0-15
		    y = 0:在液晶的第一列
			y = 1:在液晶的第二列
			......
			y = 15:在液晶的第十六列
返回值：无
备注：通过指定x,y的值可以将液晶的光标移动到指定的位置
*******************************************************************************/
void LCD1602_MoveToPosition(unsigned char x,unsigned char y)	
{
	if(0 == x)
		LCD1602_WriteInformation((0x80 | y),0);	   //光标定位到第一行的y列
	if(1 == x)
		LCD1602_WriteInformation((0xC0 | y),0);	   //光标定义到第二行的y列
}

/******************************************************************************
函数名称：LCD1602_DisplayOneCharOnAddr
函数功能：在指定的位置上显示指定的字符
入口参数：x-液晶显示的行数，范围0-1
			x = 0:在液晶的第一行
			x = 1:在液晶的第二行
		  y-液晶显示的列数，范围0-15
		    y = 0:在液晶的第一列
			y = 1:在液晶的第二列
			......
			y = 15:在液晶的第十六列
		  ucData-要显示的字符数据
返回值：无
备注：确保x,y的取值要在指定的范围内
*******************************************************************************/
void LCD1602_DisplayOneCharOnAddr(unsigned char x,unsigned char y,unsigned char ucData)
{
	LCD1602_MoveToPosition(x,y);   //光标位置
	LCD1602_WriteInformation(ucData,1);	  //写入数据
}

/******************************************************************************
函数名称：LCD1602_DisplayString
函数功能：显示字符串
入口参数：ucStr-字符串的首地址
返回值：无
备注：无
*******************************************************************************/
void LCD1602_DisplayString(unsigned char *ucStr)	
{
	while(*ucStr != '\0')	   //字符串结束之前，循环显示
	{
		 LCD1602_WriteInformation(*ucStr,1);	 //依次写入每一个字符
		 ucStr++;								 //指针增加
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
函数名称：Delay
函数功能：延时函数
入口参数：uiCount-延时参数，每加1增加0.5ms
返回值：无
备注：无
*******************************************************************************/
void Delay(unsigned int uiCount)
{
	unsigned char j = 244;
	for(;uiCount > 0;uiCount--) while(--j);	
}


/******************************************************************/
// 以下为HJ-C52复位函数
/******************************************************************/
    void hj_c52()
    {
    LCD1602=0;//关掉LCD1602 以免影响点阵管IO口电评
    DS1302=0;//关掉DS1302 以免影响点阵管IO口电评
    CS88=0;     //关掉8X8点阵显示
    WE=1;  
    P0=0Xff;
    WE=0;        //关掉数码管显示
    CS88=0;      //关掉8X8点阵显示
    }
/******************************************************************/  

#endif