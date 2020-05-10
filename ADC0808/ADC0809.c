//程序运行后自动轮询采集ADC0808四路输入In0、In1、In2、In3的电压，并在LCD1604上显示
//变量定义
#include "reg51.h"
#include "intrins.h"//包含_nop_()函数
#define uint unsigned int
#define uchar unsigned char
uchar data line_data[16];//要显示的一行字符
uchar code numchar[]={'0','1','2','3','4','5','6','7','8','9'};
sbit LCD_RS=P3^0;
sbit LCD_RW=P3^1;
sbit LCD_EN=P3^4;
sbit AD_CLK=P3^5;
sbit Start=P3^3;
sbit OE=P3^7;
uint var,tmpint;
long tmplong;
uchar channel,samp_data,tmpchar,n;
bit end_samp;

//延时子程序
void delay_ms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
	{
		for(j=110;j>0;j--);
	}
}

//LCD忙检查子程序
bit lcd_busy()
{
	bit flag;
	LCD_RS=0;
	LCD_RW=1;
	LCD_EN=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	flag=(bit)(P1&0X80);
	LCD_EN=0;
	return flag;
}

//写命令子程序
void lcd_wcmd(uchar cmd)
{
	while(lcd_busy());
	LCD_RS=0;
	LCD_RW=0;
	LCD_EN=0;
	_nop_();
	_nop_();
	P1=cmd;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=0;
	
}

//LCD清屏子程序
void lcd_clr()
{
	lcd_wcmd(0x01);
	delay_ms(2);
}

//写数据子程序
void lcd_wdat(uchar dat)
{
	while(lcd_busy());
	LCD_RS=1;
	LCD_RW=0;
	LCD_EN=0;
	_nop_();
	_nop_();
	P1=dat;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=0;
}

//初始化子程序
void lcd_init()
{
	delay_ms(5);
	lcd_wcmd(0x01);//清除LCD显示内容
	delay_ms(5);
	lcd_wcmd(0x06);//移动光标
	delay_ms(5);
	lcd_wcmd(0x0c);//显示开，关光标
	delay_ms(5);		 
	lcd_wcmd(0x38);//16*4显示，5*7点阵
	delay_ms(15);//等待LCD电源稳定
}

//字符显示子程序
void showstring(uchar m)
{
	uchar i;
	switch(m)
	{
		case 0:
			lcd_wcmd(0x80);//设置显示位置为第1行第0列
			break;
		case 1:
			lcd_wcmd(0xc0);//设置显示位置为第2行第0列
			break;
		case 2:
			lcd_wcmd(0x90);//设置显示位置为第3行第0列
			break;
		case 3:
			lcd_wcmd(0xd0);//设置显示位置为第4行第0列
	}
	i=0;
	while(line_data[i]!='\0')
	{
		lcd_wdat(line_data[i]);//加载显示字符
		delay_ms(5);
		i++;
	}
}

//外部中断0服务子程序
void s_int0() interrupt 0	//ADC0808采集完毕
{
	P0=0XFF;
	delay_ms(1);
	OE=1;		//打开ADC0808的输出功能
	samp_data=P0;
	OE=0;		//关闭ADC0808的输出功能
	end_samp=1;
	EX0=0;		//关闭外部中断0
}

//外部中断0服务子程序
void s_timer1() interrupt 3		//ADC0808采集完毕
{
	AD_CLK=~AD_CLK;
}

//ADC0808数据采集触发子程序
void sample(uchar ch)
{
	while(!end_samp);	//等待转换完成
	tmplong=(long)samp_data*5*1000;
	var=(uint)(tmplong/255);
	if(var==0)
	{
		n=0;
		line_data[n++]='C';
		line_data[n++]='h';
		line_data[n++]='0'+ch;
		line_data[n++]=':';
		line_data[n++]='0';
		line_data[n++]='V';
	}
	else
	{
		n=0;
		line_data[n++]='C';
		line_data[n++]='h';
		line_data[n++]='0'+ch;
		line_data[n++]=':';
		tmpint=var;

		tmpchar=(uchar)(tmpint/1000);
		tmpint=tmpint-(uint)tmpchar*1000;
		line_data[n++]=numchar[tmpchar];

		line_data[n++]='.';

		tmpchar=(uchar)(tmpint/100);
		tmpint=tmpint-(uint)tmpchar*100;
		line_data[n++]=numchar[tmpchar];

		tmpchar=(uchar)(tmpint/10);
		tmpint=tmpint-(uint)tmpchar*10;
		line_data[n++]=numchar[tmpchar];

		tmpchar=(uchar)(tmpint);
		line_data[n++]=numchar[tmpchar];
		line_data[n++]='V';
	}
	line_data[n]='\0';
}

//主程序
void main()
{
	delay_ms(10);
	lcd_init();	//初始化
	lcd_clr();	//清屏
	delay_ms(2);
	channel=0;	//计算状态为状态0
	var=0;
	line_data[n]='\0';
	AD_CLK=0;
	OE=0;

	TMOD=0X20;	//T1工作在方式2
	TH1=0XE7;	//10KHZ
	TL1=0XE7;
	EA=1;		//打开中断总开关
	EX0=1;	   	//打开外部中断0
	IT0=1;	   	//设置中断触发方式为下降沿触发方式
	ET1=1;
	TR1=1;
	while(1)
	{
		end_samp=0;
		EX0=1;	//打开外部中断允许位
		P2=channel;
		Start=1;	//启动AD
		delay_ms(2);
		Start=0;	//启动信号结束
		sample(channel);	//采集
		showstring(channel);	//输出
		channel++;	//通道变更
		if(channel==4)
		{
			channel=0;
		}
	}


}