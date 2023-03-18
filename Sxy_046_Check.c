#include<reg52.h>
#include"LCD.h"

sbit LED2=P1^2;
sbit LED3=P1^3;
sbit LED4=P1^4;
sbit LED5=P1^5;
sbit LED6=P1^6;
sbit LED7=P1^7;

sbit SOUND=P3^6;//�ж�����
sbit HUMAN=P3^7;//����������
sbit IN=P3^2;   //��������

sbit K1=P3^4;   //��������һ
sbit K2=P3^5;   //�������ض�

void init();    //��ʼ������
void Control(); //�������ƺ���ʵ�ֳ���ʱ���Ĳ�������

int wait_time=5;         //����ʱ��
int wait_time_buf=5;     //�������������ݴ棬ͬʱʵ�ֵ���ʱ������ص�ԭֵ
int human_coming=0;      //��⵽�����־λ
int i=0;
void init()
{
	//LED��ʼ����
	LED2=1;LED3=1;LED4=1;LED5=1;LED6=1;LED7=1;
	
	//�жϳ�ʼ��
	IT0=0;               //�ⲿ�ж�0�͵�ƽINT0�����ڹ��������жϰ�����ҹ
	EA=1;                //ȫ��  =0���ж�
	EX0=1;               //�����ⲿ�ж�0�����ж�
	//��ʱ���ж�
	TMOD=0x01;           //0_0_00__0_0_01B ʹ��T0��ʱ���������ⲿ�жϣ���ʱ��ģʽ��ģʽ1
	TH0=0x4C;               //��ֵ0
	TL0=0x00;               //11/12*2^16*17=1026765ԼΪ1��
	ET0=1;               //����ʱ��0�ж�
	TR0=1;               //��ʼ����
	
	hj_c52();            //HJ-C52��λ����
	LCD1602_Init();	     //Һ����ʼ��
}
void main()
{
	init();
	while(1)
	{ //��������LED��˸
		LED6=K1;
		LED7=K2;	
		Control();
		//�����Ӧģ��ָʾ
		LED3=1;
		LED3=HUMAN;
	 
		if(HUMAN==1)
		{  
			 LED5=0; //����ͨ��ָʾ
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
			LCD1602_MoveToPosition(0,0);	//��ʾλ���ƶ���ָ��λ��
	    LCD1602_DisplayString("oooooook");
			
			
			LCD1602_MoveToPosition(1,0);	//��ʾλ���ƶ���ָ��λ��
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

void HJ_KEY(void) interrupt 0 using 1	  //�жϺ���
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
			LCD1602_MoveToPosition(0,0);	//��ʾλ���ƶ���ָ��λ��
	    LCD1602_DisplayString("warnning");
			LCD1602_MoveToPosition(1,0);	//��ʾλ���ƶ���ָ��λ��
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