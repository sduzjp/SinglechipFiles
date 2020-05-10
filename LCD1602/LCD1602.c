//变量定义
#include "reg51.h"
#include "intrins.h"//包含_nop_()函数
#define uint unsigned int
#define uchar unsigned char
uchar code line1_data[]="Hello Everyone";//要显示的第一行字符
uchar code line2_data[]="Welcome to SDU!";//要显示的第二行字符
sbit LCD_RS=P1^7;	  //定义控制引脚
sbit LCD_RW=P1^6;
sbit LCD_EN=P1^5;
uchar flag=0;

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
	bit result;
	LCD_RS=0;
	LCD_RW=1;
	LCD_EN=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	result=(bit)(P2&0X80);
	LCD_EN=0;
	return result;
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
	P2=cmd;
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
	P2=dat;
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
	delay_ms(15);//等待LCD电源稳定
	lcd_wcmd(0x38);//功能设定指令中DL=1,N=1,F=0，8位数据宽度，16*2显示，5*7点阵字符
	delay_ms(5);
	lcd_wcmd(0x0c);//显示开关控制指令中D=1,C=0,B=0，显示开，关光标，不闪烁
	delay_ms(5);
	lcd_wcmd(0x06);//进入模式设置指令中I/D=1,S=0,地址自动增加
	delay_ms(5);
	lcd_wcmd(0x01);//清除LCD显示内容，清屏指令DB7-DB0部分为01H
	delay_ms(5);
}

//闪烁子程序
void flash()
{
	delay_ms(1000);
	lcd_wcmd(0x08);//显示开关控制指令中D=0,C=0,B=0,显示关，关光标，不闪烁
	delay_ms(500);
	lcd_wcmd(0x0c);//显示开，关光标，不闪烁
	delay_ms(500);
	lcd_wcmd(0x08);
	delay_ms(500);
	lcd_wcmd(0x0c);
	delay_ms(500);
	lcd_wcmd(0x08);
	delay_ms(500);
	lcd_wcmd(0x0c);
	delay_ms(500);
}

//主程序
void main()
{
	uchar i;
	EA=1;	//打开中断总开关
	EX0=1;	//打开外部中断0
	IT0=1;	//设置中断触发方式为下降沿触发方式
	EX1=1;
	IT1=1;
	delay_ms(10);
	lcd_init();		//初始化
	lcd_clr();		//清屏
	delay_ms(5);
	while(1)
	{
		if(flag==1)
		{
			lcd_wcmd(0x00|0x80);	//DDRAM地址设置指令，显示位置为第1行第0列
			i=0;
			while(line1_data[i]!='\0')
			{
				lcd_wdat(line1_data[i]);	//加载第1行字符
				delay_ms(100);
				i++;
			}
			lcd_wcmd(0x40|0x80);	//设置显示位置为第2行第0列
			i=0;
			while(line2_data[i]!='\0')
			{
				 lcd_wdat(line2_data[i]);	//加载第2行字符
				delay_ms(100);
				i++;
			}
			flash();
			flag=0;
		}
		if(flag==2)
		{
			lcd_clr();		//清屏
			delay_ms(5);
			flag=0;
		}
	}
}				

//外部中断0子程序
void INT_0() interrupt 0
{
	flag=1;
}
//外部中断1子程序
void INT_1() interrupt 1
{
	flag=2;
}