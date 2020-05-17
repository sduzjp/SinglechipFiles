//ADC0808信号采集自我完成实验：利用LCD1602和ADC0808实现简单的交流信号过零检测与频率分析
//要求信号幅度变化时(满量程的5%-95%)，不影响检测的结果。频率检测的结果通过LCD1602
//的第一行显示出来，信号过零时，能够通过P2.6输出一个脉冲宽度为5us的脉冲信号。
#include "reg51.h"
#include "intrins.h"
#define uint unsigned int
#define uchar unsigned char

sbit LCD_RS=P2^0;  //定义数据/命令选择位为P2.0引脚
sbit LCD_RW=P2^1;  //定义读/写选择位为P2.1引脚
sbit LCD_EN=P2^2;  //定义使能信号端为P2.2引脚
sbit BF=P1^7;	 //定义忙碌标志位，BF为1表示忙碌
sbit CLK=P2^3; 	//定义方波产生位为P2.3引脚
sbit Start=P2^4;  //定义开始标志位为P2.4引脚
sbit OE=P2^5;	//定义允许AD读写位为P2.5引脚
sbit Out_pulse=P2^6;  //定义脉冲输出位为P2.6引脚
sbit EOC=P2^7;	  //定义转换结束标志位，EOC为1表示AD转换结束

uchar n=0;	  //判断50ms过了多少次变量
uchar flag=0;	//时间间隔1s标志位

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
	bit result;	//忙标志位
	LCD_RS=0;	//数据/命令选择位置低电平
	LCD_RW=1;	//读/写选择位置高电平
	LCD_EN=1;	//使能信号端置高电平
	_nop_();
	_nop_();
	_nop_();
	_nop_();	//相当于四条nop空指令
	result=BF;
	LCD_EN=0;	//使能信号端置低电平，产生下降沿
	return result;
}

//写命令子程序
void lcd_wcmd(uchar cmd)
{
	while(lcd_busy());	//判断LCD1604是否忙
	LCD_RS=0;	//数据/命令选择位置低电平
	LCD_RW=0;	//读/写选择位置低电平
	LCD_EN=0;	//使能信号端置低电平
	_nop_();
	_nop_();
	P1=cmd;		//将数据送入P1口
	_nop_();
	_nop_();
	_nop_();
	_nop_();	//相当于四条nop空指令
	LCD_EN=1;	//使能信号端置高电平
	_nop_();
	_nop_();
	_nop_();
	_nop_();	//相当于四条nop空指令
	LCD_EN=0;	//使能信号端置低电平，产生下降沿
}

//写数据子程序
void lcd_wdat(uchar dat)
{
	while(lcd_busy());	//读忙信号，不忙则向下执行
	LCD_RS=1;	//选择数据寄存器
	LCD_RW=0;	//读写位置低电平，写入数据
	LCD_EN=0;	//使能信号端置低电平
	_nop_();
	_nop_();
	P1=dat;		//将数据送入P1口
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=1;	//使能信号端置高电平
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=0;	//使能信号端置低电平，产生下降沿
}

//LCD初始化子程序
void lcd_init()
{
	delay_ms(5);
	lcd_wcmd(0x01);//清除LCD显示内容
	delay_ms(5);
	lcd_wcmd(0x06);//移动光标,写入新数据后地址寄存器的内容减1
	delay_ms(5);
	lcd_wcmd(0x0c);//显示开，关光标
	delay_ms(5);		 
	lcd_wcmd(0x38);//16*4显示，5*7点阵
	delay_ms(15);//等待LCD电源稳定
}

//系统初始化子程序
void systeminit()
{
	TMOD=0x21;	//设定定时器T1工作在方式2，定时器T0工作在方式1
	TH1=0xEC;  //50KHZ，设置计数初值，利用T1中断产生CLK信号
	TL1=0xEC;
	EA=1;	//开总中断
	ET1=1;	//开定时器T1中断
	TR1=1;	//启动中断
	TH0=0x3C;	//50ms定时
	TL0=0xB0;
	ET0=1;	 //开定时器T0中断
	TR0=1;	 //启动中断
	Start=0;  //A/D转换启动信号，输入高电平有效
	OE=0;  //数据输出允许信号，输入高电平有效
}

//计数器0中断服务子程序
void time0() interrupt 1
{
	ET0=0; 	//关中断
	TH0=0x3C;//设置计时器初值 
	TL0=0xB0;//50MS定时 
	n++; 
	if(n==50)  //1s后flag置1，然后进行频率计算
	{
		flag=1;
		n=0;
	} 
	ET0=1; 	//开中断
}

//定时/计数器1中断服务子程序
void time1() interrupt 3 
{ 
	ET1=0; 	 //关中断
	CLK=~CLK;  //CLK信号取反
	ET1=1;	 //开中断
} 

//ADC0808数据采集
uchar ADC_sample()
{
	uchar temp;		//定义暂存采用数据变量 
	Start=1;	//启动A/D转换
	delay_ms(1);
	Start=0; 	//启动信号结束
	while(!EOC);	//等待AD转换结束  
	P0=0xff; 	//P0口全置1 
	delay_ms(1); 
	OE=1;	//打开ADC0808的输出使能允许数据输出
	temp=P0;	//读取P0口采集数据 
	OE=0;	//关闭ADC0808的输出使能
	delay_ms(4);
	return temp;	//返回采集数据 
} 

//字符显示子程序
void display(uint ch)
{
	uint num;	  //定义暂存字符各位变量
	lcd_wcmd(0x82);	  //设置显示位置为第1行第3列
	num=ch/10;
	lcd_wdat(0x30+num);	 //显示十位

	num=ch%10;
	lcd_wdat(0x30+num);	 //显示个位
}

//主程序
void main()
{
	uint temp1=0,temp2=0;//temp1暂存上次采样数据，temp2暂存本次采样数据
	uchar frequency=0;	//frequency存频率
	lcd_init();	//LCD初始化
	delay_ms(5);
	systeminit();		//系统初始化
	CLK=0;
	lcd_wcmd(0x80);	 //设置显示位置为第1行第0列
	lcd_wdat('f');	 //输出显示f:
	lcd_wdat(':');
	lcd_wcmd(0x84);	 //设置显示位置为第1行第6列
	lcd_wdat('H');	 //输出显示HZ
	lcd_wdat('z');
	while(1)
	{
		temp2=temp1;
		temp2=ADC_sample();	 //调用ADC0808数据采样子程序获取采用数据
		if(((temp1>=128)&&(temp2<=128))||((temp1<=128)&&(temp2>=128)))//判断采样数据是正数还是负数
		{
			frequency++;  //出现一次过零点，frequency自增1
			Out_pulse=1;
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();   
			Out_pulse=0;  //产生5us的脉冲信号
		}
		if(flag==1)	   //1s后计算频率
		{
			flag=0;
			frequency=frequency/2;		//频率为frequency/2
			display(frequency);	 //显示频率
			frequency=0;
		}
	}
}