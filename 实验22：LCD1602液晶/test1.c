//LCD1602显示实验：点击按键KEY1在LCD1602第一行显示“Hello Everyone”,
//第二行显示“Welcome to SDU”,并闪烁3次。点击按键KEY2将LCD1602清屏
//变量定义
#include "reg51.h"
#define uint unsigned int		//重新命名数据类型定义
#define uchar unsigned char
uchar code line1_data[]="Hello Everyone";//要显示的第一行字符
uchar code line2_data[]="Welcome to SDU!";//要显示的第二行字符
//定义控制引脚
sbit LCD_RS=P2^6;	//定义数据/命令选择位为P1.7引脚  
sbit LCD_RW=P2^5;	//定义读/写选择位为P1.6引脚  
sbit LCD_EN=P2^7;	//定义使能信号位为P1.5引脚 

void delay_n40us(uint n)
{
	uint i;
	uchar j;
	for(i=n;i>0;i--)
	{
		for(j=0;j<2;j++);
	}
}

//写命令子程序
void lcd_wcmd(uchar cmd)
{
	LCD_RS=0;	 //选择指令寄存器
	LCD_RW=0;	 //读写选择位置低电平，写入指令
	P0=cmd;		 //向P2口写入指令
	LCD_EN=1;	 //使能端置高电平
	LCD_EN=0;	 //使能端置低电平，产生下降沿，则液晶模块执行命令
	delay_n40us(1);
}

//写数据子程序
void lcd_wdat(uchar dat)
{
	LCD_RS=1;	  //选择数据寄存器
	LCD_RW=0;	  //读写选择位置低电平进行写操作
	P0=dat;		  //向P2口写入数据
	LCD_EN=1;	  //使能端置高电平
	LCD_EN=0;	 //使能端置低电平，产生下降沿，液晶模块执行命令
	delay_n40us(1);
}

//初始化子程序
void lcd_init()
{
	lcd_wcmd(0x38);//功能设定指令中DL=1,N=1,F=0，8位数据宽度，16*2显示2行，5*7点阵字符
	lcd_wcmd(0x0c);//显示开关控制指令中D=1,C=0,B=0，显示开，关光标，不闪烁
	lcd_wcmd(0x06);//进入模式设置指令中I/D=1,S=0,写入新数据后地址自动增加且显示屏不移动
	lcd_wcmd(0x01);//清除LCD显示内容，清屏指令D7-D0部分为01H
	delay_n40us(100);
}

void charDisplay()
{
	uchar i;
	lcd_init();
	for(i=0;i<15;i++)
	{
		lcd_wdat(line2_data[i]);	
	}
}
//主程序
void main()
{
	lcd_init();		//初始化
	charDisplay();
	lcd_wcmd(0xc0);
	lcd_wdat('!');
	lcd_wdat('z');
	while(1);
}				
