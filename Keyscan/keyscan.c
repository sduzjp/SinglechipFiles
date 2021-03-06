//矩阵键盘扫描实验：当点击按键KEYi时，数码管显示数字i(i从0取到F)
//变量定义
#include "reg51.h"
#define uint unsigned int		//重新定义数据类型
#define uchar unsigned char
uchar code KEY_TABLE[]={0x11,0x21,0x41,0x81,0x12,0x22,0x42,0x82,0x14,
						0x24,0x44,0x84,0x18,0x28,0x48,0x88};	 //按键键值表
						//按键值表中前四位与按键所在列有关，后四位与按键所在行有关
						//四位二进制数中为1的那一位所在的位置就是按键所在的行或列的位置
						//比如按键0x24即0010 0100表示在第二行第一列;
						//而0x82即1000 0010表示在第一行第三列
uchar code TABLE[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
					0x77,0x7c,0x39,0x5e,0x79,0x71};//共阴数码管显示码表0-F

//长延时子程序，做点亮数码管延时
void delayl()
{
	uint n=50000;
	while(n--);
}

//短延时子程序，做键盘去抖动功能
void delays()
{
	uint n=10000;
	while(n--);
}

//主程序
void main()
{
	uchar temp,key,num,i;//定义无符号字符型变量
	P1=0x0;	   //P1口置0，数码管熄灭
	while(1)
	{					  //行值后四位显示，列值前四位显示，根据线反转法得到按键所在行和列
		P3=0xf0;		  //P3.0-P3.3置0，P3.4-P3.7置1，即行置为0，列置为1，确定按键所在列
		if(P3!=0XF0)
		{
			delays();	  //调用短延时子程序去抖动
			P3=0xf0;	  //重新行置0，列置1
			if(P3!=0XF0)
			{
				temp=P3;	
				P3=0x0f;	//P3.0-P3.3置1，P3.4-P3.7置0，即行置为1，列置为0，确定按键所在行
				key=temp|P3;	//按键所在行和列的位置为0，其余位置为1
				key=0xff-key;  //按位取反，则按键所在行和列的位置为1，其余位置为0
				for(i=0;i<16;i++)
				{
					if(key==KEY_TABLE[i])//查按键所在行、列
					{
						num=i;		 //设置数码管显示的数字
						break;		 //跳出循环
					}
				}
				P1=TABLE[num];		 //将显示码送入P1口
				delayl();			 //调用长延时子程序显示
			}	
		}
	}
}