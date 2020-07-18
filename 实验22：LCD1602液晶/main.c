/**************************************************************************************
*		              LCD1602液晶显示实验												  *
实现现象：下载程序后插上LCD1602液晶在开发板上，即可显示
注意事项：如果不想让点阵模块显示，可以将74HC595模块上的JP595短接片拔掉。																				  
***************************************************************************************/

#include "reg51.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include "lcd.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;
//定义接收 数组
u8 Buffer[5]={0};
u8 Disp[]=" Pechin Science ";
u8 Disp1[]=" Hello  World!  ";

void display()
{
	u8 i;
	LcdInit();
	for(i=0;i<16;i++)
	{
		LcdWriteData(Disp[i]);	
	}
}

void display1()
{
	u8 i;
	LcdInit();
	for(i=0;i<16;i++)
	{
		LcdWriteData(Disp1[i]);	
	}
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

void SendOneByte(uchar c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}

void UART_SER () interrupt 4
{
	u16 n; 	//定义临时变量
	if(RI) 		//判断是接收中断产生
	{
		RI=0; 	//标志位清零
		n=SBUF; //读入缓冲区的值
		switch(n)
		{
			case 1:	
					display();	
					SendOneByte(n);
					break;
			case 2:	
					display1();	
					SendOneByte(n);
					break;
		}
	}	
}
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main(void)
{
	LcdInit();
	InitUART();
	while(1);				
}
