#include <reg52.h>

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;
//u8 code line2_data[]="Welcome to SDU!";//要显示的第二行字符
//定义接收 数组
u8 Buffer[15];
u8 SetTime[14];
u8 k=0,j=0,i=0;
sbit LCD_RS=P2^6;	//定义数据/命令选择位为P1.7引脚  
sbit LCD_RW=P2^5;	//定义读/写选择位为P1.6引脚  
sbit LCD_EN=P2^7;	//定义使能信号位为P1.5引脚 

void delay_n40us(u16 n)
{
	u16 i;
	u8 j;
	for(i=n;i>0;i--)
	{
		for(j=0;j<2;j++);
	}
}

//写命令子程序
void lcd_wcmd(u8 cmd)
{
	LCD_RS=0;	 //选择指令寄存器
	LCD_RW=0;	 //读写选择位置低电平，写入指令
	P0=cmd;		 //向P2口写入指令
	LCD_EN=1;	 //使能端置高电平
	LCD_EN=0;	 //使能端置低电平，产生下降沿，则液晶模块执行命令
	delay_n40us(1);
}

//写数据子程序
void lcd_wdat(u8 dat)
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
	u8 i;
	lcd_init();
	for(i=0;i<15;i++)
	{
		lcd_wdat(Buffer[i]);	
	}
} 

/*******************************************************************************
* 函数名         :UsartInit()
* 函数功能		   :设置串口
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void UsartInit()
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=0XF3;				//计数器初始值设置，注意波特率是4800的
	TL1=0XF3;
	ES=1;						//打开接收中断
	EA=1;						//打开总中断
	TR1=1;					//打开计数器
}

/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	UsartInit();  //	串口初始化
	lcd_init();
	while(1);	
}
//中断函数
void receive() interrupt 4
{
	if(RI)
	{ 
		Buffer[k]=SBUF;
		RI=0;
		SBUF=Buffer[k];
		while(!TI);
		TI=0;
		k++;
		if(k>=15)
		{
			k=0;
			charDisplay();
			for(i=0;i<14;i++)SetTime[i]=Buffer[i+1];
			lcd_wcmd(0xc0);
			for(i=0;i<14;i++)lcd_wdat(SetTime[i]);
		}
	}	
}
