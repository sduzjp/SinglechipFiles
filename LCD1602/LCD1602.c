//LCD1602显示实验：点击按键KEY1在LCD1602第一行显示“Hello Everyone”,
//第二行显示“Welcome to SDU”,并闪烁3次。点击按键KEY2将LCD1602清屏
//变量定义
#include "reg51.h"
#include "intrins.h"//包含_nop_()函数
#define uint unsigned int		//重新命名数据类型定义
#define uchar unsigned char
uchar code line1_data[]="Hello Everyone";//要显示的第一行字符
uchar code line2_data[]="Welcome to SDU!";//要显示的第二行字符
//定义控制引脚
sbit LCD_RS=P1^7;	//定义数据/命令选择位为P1.7引脚  
sbit LCD_RW=P1^6;	//定义读/写选择位为P1.6引脚  
sbit LCD_EN=P1^5;	//定义使能信号位为P1.5引脚 
sbit int0=P3^2;
sbit int1=P3^3;
uchar flag=0;	 //定义显示标志位

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
	bit result;	   //定义位变量result
	LCD_RS=0;	   //选择指令寄存器
	LCD_RW=1;	   //读/写选择位置高电平，读忙信号
	LCD_EN=1;	   //使能信号端置高电平
	_nop_();
	_nop_();
	_nop_();
	_nop_();	  //相当于四条nop空指令
	result=(bit)(P2&0X80);	 //返回数据最高位代表是否忙，result为1代表忙，为0代表空闲
	LCD_EN=0;	  //使能信号端置低电平
	return result;	 
}

//写命令子程序
void lcd_wcmd(uchar cmd)
{
	while(lcd_busy());	//读忙信号，不忙则向下执行
	LCD_RS=0;	 //选择指令寄存器
	LCD_RW=0;	 //读写选择位置低电平，写入指令
	LCD_EN=0;	 //初始化使能端置低电平
	_nop_();
	_nop_();	 //两条空指令
	P2=cmd;		 //向P2口写入指令
	_nop_();
	_nop_();
	_nop_();
	_nop_();	 //四条空指令
	LCD_EN=1;	 //使能端置高电平
	_nop_();
	_nop_();
	_nop_();
	_nop_();	 //四条空指令
	LCD_EN=0;	 //使能端置低电平，产生下降沿，则液晶模块执行命令
}

//LCD清屏子程序
void lcd_clr()
{
	lcd_wcmd(0x01);	  //调用LCD写命令写入清零指令，D7-D0部分为01H
	delay_ms(2);	  //延时2ms，清屏指令延时1.64ms以上
}

//写数据子程序
void lcd_wdat(uchar dat)
{
	while(lcd_busy());	 //读忙信号，不忙则向下执行
	LCD_RS=1;	  //选择数据寄存器
	LCD_RW=0;	  //读写选择位置低电平进行写操作
	LCD_EN=0;	  //初始化使能端置低电平
	_nop_();
	_nop_();	  //两条空指令
	P2=dat;		  //向P2口写入数据
	_nop_();
	_nop_();
	_nop_();
	_nop_();	  //四条空指令
	LCD_EN=1;	  //使能端置高电平
	_nop_();
	_nop_();
	_nop_();
	_nop_();	 //四条空指令
	LCD_EN=0;	 //使能端置低电平，产生下降沿，液晶模块执行命令
}

//初始化子程序
void lcd_init()
{
	delay_ms(15);//延时15ms，等待LCD电源稳定
	lcd_wcmd(0x38);//功能设定指令中DL=1,N=1,F=0，8位数据宽度，16*2显示2行，5*7点阵字符
	delay_ms(5);   //延时5ms
	lcd_wcmd(0x0c);//显示开关控制指令中D=1,C=0,B=0，显示开，关光标，不闪烁
	delay_ms(5);   //延时5ms
	lcd_wcmd(0x06);//进入模式设置指令中I/D=1,S=0,写入新数据后地址自动增加且显示屏不移动
	delay_ms(5);   //延时5ms
	lcd_wcmd(0x01);//清除LCD显示内容，清屏指令D7-D0部分为01H
	delay_ms(5);   //延时5ms
}

//闪烁子程序
void flash()
{
	delay_ms(1000);	//延时1s
	lcd_wcmd(0x08);//显示开关控制指令中D=0,C=0,B=0,显示关，关光标，不闪烁
	delay_ms(500);	//延时0.5s
	lcd_wcmd(0x0c);//D=1,C=0,B=0，显示开，关光标，不闪烁，之后依次开显示关显示
	delay_ms(500);	//延时0.5s
	lcd_wcmd(0x08);	
	delay_ms(500); //延时0.5s
	lcd_wcmd(0x0c);
	delay_ms(500);	//延时0.5s
	lcd_wcmd(0x08);
	delay_ms(500);	//延时0.5s
	lcd_wcmd(0x0c);
	delay_ms(500);	//延时0.5s
}

//主程序
void main()
{
	uchar i;
	EA=1;	//打开中断总开关
	EX0=1;	//打开外部中断0
	IT0=1;	//设置外部中断0触发方式为下降沿触发方式
	EX1=1;	//打开外部中断1
	IT1=1;	//设置外部中断1触发方式为下降沿触发方式
	delay_ms(10);	//延时10ms
	lcd_init();		//初始化
	lcd_clr();		//清屏
	delay_ms(5);	//延时5ms
	while(1)
	{
		if(flag==1)
		{
			lcd_clr();		//清屏
			delay_ms(5);	//延时5ms
			lcd_wcmd(0x00|0x80);	//DDRAM地址设置指令，显示位置为第1行第0列
			i=0;
			while(line1_data[i]!='\0')
			{
				lcd_wdat(line1_data[i]);	//加载第1行字符
				delay_ms(100);		 //延时100ms
				i++;
			}
			lcd_wcmd(0x40|0x80);	//设置显示位置为第2行第0列
			i=0;
			while(line2_data[i]!='\0')
			{
				lcd_wdat(line2_data[i]);	//加载第2行字符
				delay_ms(100);	   //延时100ms
				i++;
			}
			flash();	 //调用闪烁子程序
			flag=0;		 //显示完后flag置0保持当前显示状态等待下一次中断
		}
		if(flag==2)
		{
			lcd_clr();		//清屏
			delay_ms(5);	//延时5ms
			flag=0;		  //清屏完后flag置0保持当前显示状态等待下一次中断
		}
	}
}				

//外部中断0子程序
void INT_0() interrupt 0   //点击KEY1调用外部中断0服务子程序，进行液晶显示
{
	flag=1;
}
//外部中断1子程序		 //点击KEY2调用外部中断1服务子程序，进行清屏
void INT_1() interrupt 2
{
	flag=2;
}

//中断				中断号		入口地址
//外部中断0			0			0003H
//定时/计数器0		1			000BH
//外部中断1			2			0013H
//定时/计数器1		3			001BH
//串行口			4			0023H