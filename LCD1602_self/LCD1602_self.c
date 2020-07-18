//LCD1602自我完成实验：利用LCD1602和16个按键实现简单的十进制数加减乘除四则混合运算
//KEY0-KEY9分别代表数字0-9，KEY10-KEY13分别代表运算符“+，-，*，/”，KEY15代表“=”
//KEY14代表清除命令，不管何时按下清除按键计算过程停止，两个输入变量都清零，屏幕清屏
//LCD1602第一行显示输入的两个计算数和运算符，第二行显示计算结果，结果允许为负数
//输入数范围为0-32767，除数保证不为0，除数时可以同时显示商与余数

#include "reg51.h"
#include "intrins.h"
#define uchar unsigned char
#define	uint unsigned int
//定义控制引脚
sbit LCD_RS=P1^7;	//定义数据/命令选择位为P1.7引脚  
sbit LCD_RW=P1^6;	//定义读/写选择位为P1.6引脚  
sbit LCD_EN=P1^5;	//定义使能信号位为P1.5引脚 

uchar code TABLE[]={0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0}; //按键对应的数字值
uchar code TABLE1[]={0,1,2,3,4,5,6,7,8,9,0x2b-0x30,0x2d-0x30,
0x2a-0x30,0x2f-0x30,0x01-0x30,0x3d-0x30};//按键对应的字符
char i,j,temp,keynum,keynumtemp;//temp作为判断按键键码时的中间变量，keynum作为键号变量,keynumtemp暂存keynum中值 
long num1,num2,ans;  //num1代表输入的第一个数，num2代表输入的第二个数，ans代表运算结果数
uchar flag,symbol;//flag作为输入的数分界标志变量，symbol作为运算符号变量

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
	result=(bit)(P2&0x80);	 //返回数据最高位代表是否忙，result为1代表忙，为0代表空闲
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

//写数据子程序
void lcd_wdat(uchar dat)
{
	P2=0;
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
	keynum=0;	   //num存放KEY0-KEY15按键键号，初始化置-1
	delay_ms(15);//延时15ms，等待LCD电源稳定
	lcd_wcmd(0x38);//功能设定指令中DL=1,N=1,F=0，8位数据宽度，16*2显示2行，5*7点阵字符
	delay_ms(5);   //延时5ms
	lcd_wcmd(0x0c);//显示开关控制指令中D=1,C=0,B=0，显示开，关光标，不闪烁
	delay_ms(5);   //延时5ms
	lcd_wcmd(0x06);//进入模式设置指令中I/D=1,S=0,写入新数据后地址自动增加且显示屏不移动
	delay_ms(5);   //延时5ms
	lcd_wcmd(0x01);//清除LCD显示内容，清屏指令D7-D0部分为01H
	delay_ms(5);   //延时5ms
	i=0; 
	j=0;		//j=0表示在第一次运算显示中，j=1表示进行下一次运算的显示
	num1=0; 		//输入的第一个数置0
	num2=0; 		//输入的第二个数置0
	ans=0; 		//ans存放运算结果
	flag=0; 	//flag表示输入的数分界标志变量，flag为0表示正在输入第一个数，为1表示正在输入第二个数
	symbol=0;	//symbol表示运算符，1表示“+”，2表示“-”，3表示“*”，4表示“/”
}

void keyscan() 
{
	P3=0xfe; 		 //第0行置0，其余行置1，列全置1
	if(P3!=0xfe) 	//扫描第0行
	{
		delay_ms(20);  //延时20ms去抖动
		if(P3!=0xfe) 
		{
			temp=P3&0xf0; //保留有效位
			switch(temp) 
			{
				case 0xe0:keynum=0; break;	 //说明按下的按键在第0行第0列，键号为0
				case 0xd0:keynum=1; break;  //说明按下的按键在第0行第1列，键号为1		
				case 0xb0:keynum=2; break;	 //说明按下的按键在第0行第2列，键号为2			
				case 0x70:keynum=3; break;  //说明按下的按键在第0行第3列，键号为3
			}
		}
		while(P3!=0xfe);   //等待按键释放
		if(keynum==0||keynum==1||keynum==2||keynum==3) 
		{
			if(j!=0)	 //j标志位：第二次计算时，把前面的显示清除 
			{
				keynumtemp=keynum; //keynumtemp暂存keynum值
				lcd_wcmd(0x01);	  //清屏
				lcd_init();
				keynum=keynumtemp;//恢复keynum值
				j=0; 	 
			}
			if(flag==0) num1=num1*10+TABLE[keynum]; //计算出第一个数 
			else num2=num2*10+TABLE[keynum];   //计算出第二个数
		}
		i=TABLE1[keynum];  
		lcd_wdat(0x30+i); //在液晶上显示按键对应的字符
	}
	P3=0xfd; 	  //第1行置0，其余行置1，列全置1,
	if(P3!=0xfd)  //扫描第1行
	{
		delay_ms(20);  //延时20ms去抖动
		if(P3!=0xfd) 
		{
			temp=P3&0xf0; 
			switch(temp) 
			{
				case 0xe0:keynum=4; break;	 //按下的按键在第1行第0列，键号为4
				case 0xd0:keynum=5; break;	 //按下的按键在第1行第1列，键号为5
				case 0xb0:keynum=6; break;	 //按下的按键在第1行第2列，键号为6			
				case 0x70:keynum=7; break;  //按下的按键在第1行第3列，键号为7
			} 
		}
		while(P3!=0xfd);  //等待按键释放
		if(keynum==4||keynum==5||keynum==6||keynum==7) 
		{
			if(j!=0) 	  //第二次计算时将前面的计算显示清除
			{
				keynumtemp=keynum; //keynumtemp暂存keynum值
				lcd_wcmd(0x01);	  //清屏
				lcd_init();
				keynum=keynumtemp;//恢复keynum值
				j=0; 
			}
			if(flag==0) num1=num1*10+TABLE[keynum];   //计算出第一个数
			else num2=num2*10+TABLE[keynum];  		  //计算出第二个数
		}
	i=TABLE1[keynum]; 
	lcd_wdat(0x30+i); 	//在液晶上显示字符
	}
	P3=0xfb; 	  //第2行置0，其余行置1，列全置1
	if(P3!=0xfb) 	//扫描第2行
	{
		delay_ms(20);	//延时20ms去抖动 
		if(P3!=0xfb) 
		{
			temp=P3&0xf0; 
			switch(temp) 
			{
				case 0xe0:keynum=8; break;	 //按下的按键在第2行第0列，键号为8
				case 0xd0:keynum=9; break;	 //按下的按键在第2行第1列，键号为9
				case 0xb0:keynum=10; break;   //按下的按键在第2行第2列，键号为10				
				case 0x70:keynum=11; break;   //按下的按键在第2行第3列，键号为11
			}
		}
		while(P3!=0xfb);   //等待按键释放
		if(keynum==8||keynum==9) 	 //键号为8或者9表示数字字符
		{
			if(j!=0) 		 //下一次计算时将上一次计算显示清除
			{
				keynumtemp=keynum; //keynumtemp暂存keynum值
				lcd_wcmd(0x01);	  //清屏
				lcd_init();
				keynum=keynumtemp;//恢复keynum值
				j=0; 	 
			}
			if(flag==0) num1=num1*10+TABLE[keynum]; 
			else num2=num2*10+TABLE[keynum];
		}
		if(keynum==10) 		//键号为10表示运算符“+” 
		{
			flag=1; 	   //flag置1则之后输入的数是第二个数
			symbol=1; 	   //symbol置1表示“+”
		}
		if(keynum==11) 		//键号为11表示运算符“-” 
		{
			flag=1; 	  //flag置1则之后输入的数是第二个数
			symbol=2; 	  //symbol置2表示“-”
		} 
		i=TABLE1[keynum]; 
		lcd_wdat(0x30+i);  //在液晶上显示字符
	}
	P3=0xf7; 	  //第3行置0，其余行置1，列全置1
	if(P3!=0xf7)   //扫描第三行
	{
		delay_ms(20);	//延时20ms去抖动 
		if(P3!=0xf7) 
		{	
			temp=P3&0xf0; 
			switch(temp) 
			{
				case 0xe0:keynum=12; break;	//按下的按键在第3行第0列，键号为12				
				case 0xd0:keynum=13; break;	//按下的按键在第3行第1列，键号为13			
				case 0xb0:keynum=14; break;	//按下的按键在第3行第2列，键号为14			
				case 0x70:keynum=15; break; //按下的按键在第3行第3列，键号为15
			} 
		}  
		while(P3!=0xf7);  //等待按键释放
		switch(keynum) 
		{
			case 12:	 //键号为12表示运算符“*”
				{
					lcd_wdat(0x30+TABLE1[keynum]); 
					flag=1; 
					symbol=3;  //symbol为3表示“*”
				} 
				break;
			case 13:	//键号为13表示运算符“/” 
				{ 
					lcd_wdat(0x30+TABLE1[keynum]); 
					flag=1; 
					symbol=4;  //symbol为4表示“*”
				} 
				break;
			case 15:	//键号为15表示运算符“=”，运算出计算结果 
				{
					j=1;
					if(num2==0)
					{
						lcd_wcmd(0x80+0x4f); //从最后一个数的地址开始显示
						lcd_wcmd(0x04); //写入新数据后地址寄存器内容加1
						while(num1!=0)
						{
							lcd_wdat(0x30+num1%10); //除10取余法获得每一位数字
							num1=num1/10; 
						}
						lcd_wdat(0x3d);
					}
					if(symbol==1)	//symbol为1表示加法运算
					{
						lcd_wcmd(0x80+0x4f); //从最后一个数的地址开始显示
						lcd_wcmd(0x04); //写入新数据后地址寄存器内容加1
						ans=num1+num2; 						
						while(ans!=0) 
						{
							lcd_wdat(0x30+ans%10); //除10取余法获得每一位数字
							ans=ans/10; 
						}
						lcd_wdat(0x3d); //输出显示“=” 
						j=1;
						num1=0;	   //输入数和分界标志变量以及运算符均清零
						num2=0;
						flag=0;
						symbol=0; 
					}
					else if(symbol==2)	//symbol为2表示减法运算
					{
						lcd_wcmd(0x80+0x4f);  //从最后一个数的地址开始显示
						lcd_wcmd(0x04); 
						if(num1-num2>0) ans=num1-num2; //减法运算
						else ans=num2-num1; 						
						while(ans!=0) 
						{
							lcd_wdat(0x30+ans%10); //除10取余法得到每一位
							ans=ans/10; 
						}
						if(num1-num2<0)lcd_wdat(0x2d);//结果为负数时符号为负 “-” 
						lcd_wdat(0x3d); //输出显示“=”
						j=1;
						num1=0;	   //输入数和分界标志变量以及运算符均清零
						num2=0;
						flag=0;
						symbol=0; 
					}
					else if(symbol==3)	//symbol为3表示进行乘法运算
					{
						lcd_wcmd(0x80+0x4f);  //从最后一个数的地址开始显示
						lcd_wcmd(0x04); 
						ans=num1*num2; 	 //乘法运算						
						while(ans!=0)
						{
							lcd_wdat(0x30+ans%10); 	 //除10取余法得到每一位
							ans=ans/10; 
						}
						lcd_wdat(0x3d);	//输出等于符号
						j=1;				 
						num1=0;	   //输入数和分界标志变量以及运算符均清零
						num2=0;
						flag=0;
						symbol=0; 
					}
					else if(symbol==4)
					{
						lcd_wcmd(0x80+0x4f); //从最后一个数的地址开始显示
						lcd_wcmd(0x04); 
						i=0; 
						if(num2==0)//如果除数是0，则显示error 
						{ 
							lcd_wdat('R'); 
							lcd_wdat('O'); 
							lcd_wdat('R'); 
							lcd_wdat('R'); 
							lcd_wdat('E'); 
						} 
						else
						{ 
							ans=num1/num2; 	 //得到商
							ans=num1-num2*ans;	 //判断余数
							while(ans!=0) 	 //存在余数
							{
								lcd_wdat(0x30+ans%10);//输出余数 
								ans=ans/10; 
								i++; 							
							}
							if(i!=0)   //存在余数时输出三个小数点
							{
								for(i=3;i>0;i--)
								{
									lcd_wdat(0x2e);//输出显示小数点 
								}
								ans=num1/num2;
								if(ans==0) lcd_wdat(0x30);//0
								while(ans!=0)
								{
									lcd_wdat(0x30+ans%10);//输出商
									ans=ans/10;
								}	
							}
							else	 //整除情况
							{
								ans=num1/num2;
								if(ans==0)	//num1=0的情况输出0
								{
									lcd_wdat(0x30);
								}
								while(ans!=0)
								{
									lcd_wdat(0x30+ans%10);//输出商
									ans=ans/10;
								}	
							}
							lcd_wdat(0x3d);
						}
						j=1;
						num1=0;	  //输入数和分界标志变量以及运算符均清零
						num2=0;
						flag=0;
						symbol=0; 
					} 
				} 
				break;
			case 14: //键号为14表示清除显示 
				{
					lcd_wcmd(0x01);
					j=1;		//进行下一次计算显示
					num1=0;		 //输入数和分界标志变量以及运算符均清零
					num2=0;
					flag=0;
					symbol=0;
				} 
				break; 
		} 
	} 
}

void main() 
{ 
	lcd_init();   //LCD1602初始化
	while(1)
	{
		keyscan(); 	 //调用键盘扫描和显示子程序显示结果
	} 
}
	 