#include<reg52.h>
#include"LCD.h"

sbit LED2=P1^2;
sbit LED3=P1^3;
sbit LED4=P1^4;
sbit LED5=P1^5;
sbit LED6=P1^6;
sbit LED7=P1^7;

sbit SOUND=P3^6;//判断声音
sbit HUMAN=P3^7;//红外检测人体
sbit IN=P3^2;   //光敏电阻

sbit K1=P3^4;   //独立开关一
sbit K2=P3^5;   //独立开关二

void init();    //初始化函数
void Control(); //按键控制函数实现持续时长的步进输入

int wait_time=5;         //持续时长
int wait_time_buf=5;     //按键输入数据暂存，同时实现倒计时结束后回到原值
int human_coming=0;      //检测到人体标志位
int i=0;
void init()
{
	//LED初始化；
	LED2=1;LED3=1;LED4=1;LED5=1;LED6=1;LED7=1;
	
	//中断初始化
	IT0=0;               //外部中断0低电平INT0，用于光敏电阻判断白天或黑夜
	EA=1;                //全开  =0关中断
	EX0=1;               //允许外部中断0进入中断
	//定时器中断
	TMOD=0x01;           //0_0_00__0_0_01B 使用T0定时器，不用外部中断，定时器模式，模式1
	TH0=0x4C;               //初值0
	TL0=0x00;               //11/12*2^16*17=1026765约为1秒
	ET0=1;               //开定时器0中断
	TR0=1;               //开始运行
	
	hj_c52();            //HJ-C52复位函数
	LCD1602_Init();	     //液晶初始化
}
void main()
{
	init();
	while(1)
	{ //按键输入LED闪烁
		LED6=K1;
		LED7=K2;	
		Control();
		//人体感应模块指示
		LED3=1;
		LED3=HUMAN;
	 
		if(HUMAN==1)
		{  
			 LED5=0; //有人通过指示
		   while(HUMAN);
			 human_coming=1;
		}
		if(SOUND==0)
		{
			human_coming=1;
			LED2=~LED2;
		}
		if(human_coming==0)	
		{
			LCD1602_MoveToPosition(0,0);	//显示位置移动到指定位置
	    LCD1602_DisplayString("oooooook");
			
			
			LCD1602_MoveToPosition(1,0);	//显示位置移动到指定位置
	    DispDecint(wait_time,3);wait_time=wait_time_buf;
		}

	  
	}
}
void Control()
{
	if(K1==0)
		{
		 Delay(100);
			if(K1==0)
			{wait_time_buf+=1;}
		} 
		if(K2==0)
		{
			Delay(100);
			if(K2==0)
			{wait_time_buf-=1;}
		}
}

void HJ_KEY(void) interrupt 0 using 1	  //中断函数
	{
	  LED4=0;
		while(!IN)
		{	
	    LED3=HUMAN;
		}
	} 

//1s	
void T0_delay(void) interrupt 1
{
	i++;
	TR0=0x00;
	TH0=0x4c;
	TL0=0;
	if(i==17)
	{ 
		i=0;
	
		if(wait_time>=0&&human_coming==1)
		{
			LCD1602_MoveToPosition(0,0);	//显示位置移动到指定位置
	    LCD1602_DisplayString("warnning");
			LCD1602_MoveToPosition(1,0);	//显示位置移动到指定位置
	    DispDecint(wait_time,3);
			LED5=0;
			wait_time-=1;
		}
		if(wait_time<0)
		{
			human_coming=0;
			LED5=1;
		}

		
	}
	TR0=1;
}