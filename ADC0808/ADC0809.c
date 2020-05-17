//ADC0808信号采集实验：程序运行后自动轮询采集ADC0808四路输入的IN0,IN1,IN2,IN3的电压
//并在LCD1604上显示
//LCD1604为4*16字符的LCD显示器，和LCD1602的区别只是多了两行，其余控制命令和时序要求都一样
//定时器T1产生10KHZ的CLK方波信号
//变量定义
#include "reg51.h"
#include "intrins.h"//包含_nop_()函数，相当于一条nop空指令
#define uint unsigned int
#define uchar unsigned char
uchar data line_data[16];//要显示的一行字符
uchar code numchar[]={'0','1','2','3','4','5','6','7','8','9'};
//定义引脚
sbit LCD_RS=P3^0;  //定义数据/命令选择位为P3.0
sbit LCD_RW=P3^1;  //定义读/写选择位为P3.1
sbit LCD_EN=P3^4;  //定义使能信号端为P3.4
sbit AD_CLK=P3^5;  //定义方波CLK端为P3.5
sbit Start=P3^3;   //定义Start为P3.3
sbit OE=P3^7;	   //定义允许读A/D结果端为P3.7

uint var,tmpint;   //var为采样电压值，tmpint暂存采样电压值各位的值
long tmplong;  //暂存将采样数据转换为长整型的数据
uchar channel,samp_data,tmpchar,n; //channel存放通道值，samp_data存放采样的P0口数据，tmpchar暂存采样电压各位的值对应的字符，n为数组索引变量
bit end_samp;  //end_samp为1表示转换结束，为0表示开始转换

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
	bit flag;	//忙标志位
	LCD_RS=0;	//数据/命令选择位置低电平
	LCD_RW=1;	//读/写选择位置高电平
	LCD_EN=1;	//使能信号端置高电平
	_nop_();
	_nop_();
	_nop_();
	_nop_();	//相当于四条nop空指令
	flag=(bit)(P1&0x80);
	LCD_EN=0;	//使能信号端置低电平，产生下降沿
	return flag;
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

//LCD清屏子程序
void lcd_clr()
{
	lcd_wcmd(0x01);	   //清屏
	delay_ms(2);
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

//初始化子程序
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
		lcd_wdat(line_data[i]);//加载显示第m+1行字符
		delay_ms(5);   //延时5ms
		i++;
	}
}

//外部中断0服务子程序
void s_int0() interrupt 0	//ADC0808转换完毕时E0C输出高电平经反相器输出低电平到P3.2引脚，触发外部中断0
{
	P0=0xff;	  //P0口全置高电平，数字输出端全置1
	delay_ms(1);
	OE=1;		//打开ADC0808的输出使能，高电平有效
	samp_data=P0;	//采样P0口的数据
	OE=0;		//关闭ADC0808的输出使能
	end_samp=1;	   //表示转换结束
	EX0=0;		//关闭外部中断0
}

//定时/计数器1中断服务子程序
void s_timer1() interrupt 3		//ADC0808采集完毕
{
	AD_CLK=~AD_CLK;		//产生的方波信号取反
}

//ADC0808数据采集触发子程序
void sample(uchar ch)	 //ch存放通道值
{
	while(!end_samp);	//等待转换完成，end_samp为1时表示转换完成向下继续执行
	tmplong=(long)samp_data*5*1000;	 //将采样的P0口数据转换为长整型数据，乘以1000是使结果变成整数
	var=(uint)(tmplong/255); //转换为采样电压值
	if(var==0)		//当采样电压为0V时将需要显示的一行字符添加到数组中
	{
		n=0;
		line_data[n++]='C';	  //完成添加字符并指向数组下一个元素
		line_data[n++]='h';
		line_data[n++]='0'+ch;
		line_data[n++]=':';
		line_data[n++]='0';
		line_data[n++]='V';
	}
	else	   //采样电压值不为0V时
	{
		n=0;
		line_data[n++]='C';
		line_data[n++]='h';
		line_data[n++]='0'+ch;
		line_data[n++]=':';
		tmpint=var;			//tmpint暂存采样的电压值

		tmpchar=(uchar)(tmpint/1000);	//取得电压值的最高位即整数部分并转换为字符
		tmpint=tmpint-(uint)tmpchar*1000;  //tmpint暂存从次高位开始的电压值
		line_data[n++]=numchar[tmpchar];   //将最高位字符存入数组
		
		line_data[n++]='.';		//将小数点存入数组

		tmpchar=(uchar)(tmpint/100);
		tmpint=tmpint-(uint)tmpchar*100;
		line_data[n++]=numchar[tmpchar]; //将小数部分的十分位存入数组

		tmpchar=(uchar)(tmpint/10);
		tmpint=tmpint-(uint)tmpchar*10;
		line_data[n++]=numchar[tmpchar]; //将小数部分的百分位存入数组

		tmpchar=(uchar)(tmpint);
		line_data[n++]=numchar[tmpchar]; //将小数部分的千分位存入数组
		line_data[n++]='V';		//在电压值后面添加电压单位V显示字符
	}
	line_data[n]='\0';	  //显示字符结束标志
}

//主程序
void main()
{
	delay_ms(10); //延时10ms
	lcd_init();	//初始化
	lcd_clr();	//清屏
	delay_ms(2);  //延时2ms
	channel=0;	//计算状态通道为通道0
	var=0;	   //初始采样电压值置0
	line_data[n]='\0';	//显示字符开始标志
	AD_CLK=0;	//方波信号置低电平
	OE=0;	 //初始化关闭ADC0808的输出使能

	TMOD=0X20;	//定时器T1工作在方式2，具有自动重装载功能
	TH1=0XE7;	//10KHZ	   1/4个方波周期=25us，机器周期为1us，计数器初值为231=11100111B
	TL1=0XE7;	//TH保存初值不变，TL进行计数
	EA=1;		//打开中断总开关
	EX0=1;	   	//打开外部中断0
	IT0=1;	   	//设置中断触发方式为下降沿触发方式
	ET1=1;		//打开定时器1中断
	TR1=1;		//启动定时器
	while(1)
	{
		end_samp=0;	  //转换标志位置0
		EX0=1;	//打开外部中断允许位
		P2=channel;	 //将当前状态送入P2口
		Start=1;	//启动AD转换
		delay_ms(2);
		Start=0;	//启动信号结束
		sample(channel);	//调用数据采集触发子程序
		showstring(channel);	//调用字符显示子程序输出各个通道的采样电压值
		channel++;	//通道变更
		if(channel==4)		  //LCD1604最多可现实4个通道的采样电压值
		{
			channel=0;
		}
	}


}