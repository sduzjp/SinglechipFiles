#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int

uchar SetTime[14];
uchar SetTimeIndex=0,flag=0;
sbit LCD_RS=P2^6;	//定义数据/命令选择位为P1.7引脚  
sbit LCD_RW=P2^5;	//定义读/写选择位为P1.6引脚  
sbit LCD_EN=P2^7;	//定义使能信号位为P1.5引脚 
//延时子程序
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

void InitUART(void)
{
   	SCON = 0x50; 	// SCON: 模式1, 8-bit UART, 使能接收 
	TMOD |= 0x20;
	TH1=0xfd; 		//波特率9600 初值
	TL1=0xfd;
	TR1= 1;
	EA = 1;	    //开总中断
	ES= 1; 		//打开串口中断
}

void charDisplay(uchar end)
{
	uchar i;
	lcd_init();
	for(i=0;i<end;i++)
	{
		lcd_wdat(SetTime[i]+0x30);	
	}
}

void main(void)
{
    InitUART();
	while(1);
}
/*void SendOneByte(uchar c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}*/	 

void UART_SER () interrupt 4
{
	uint n; 	//定义临时变量
	if(RI) 		//判断是接收中断产生
	{
		RI=0; 	//标志位清零
		n=SBUF;
		if(SetTimeIndex<14)
		{
			SetTime[SetTimeIndex]=n;
			SetTimeIndex++;
		}
		else
		{
			SBUF=0x00;
			while(!TI);
    		TI = 0;
		}
		charDisplay(SetTimeIndex);
	}
}	  

 
 