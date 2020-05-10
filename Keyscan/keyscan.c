//变量定义
#include "reg51.h"
#define uint unsigned int
#define uchar unsigned char
uchar code KEY_TABLE[]={0x11,0x21,0x41,0x81,0x12,0x22,0x42,0x82,0x14,
						0x24,0x44,0x84,0x18,0x28,0x48,0x88};	 //按键键值表
						//按键值表中前四位与按键所在列有关，后四位与按键所在行有关
						//四位二进制数中为1的那一位所在的位置就是按键所在的行或列的位置
						//比如按键0x24即0010 0100表示在第二行第一列;
						//而0x82即1000 0010表示在第一行第三列
uchar code TABLE[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
					0x77,0x7c,0x39,0x5e,0x79,0x71};//显示码表

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
	uchar temp,key,num,i;
	P1=0x0;
	while(1)
	{
		P3=0xf0;		  //行置为1，列置为1，读列值
		if(P3!=0XF0)
		{
			delays();	  //去抖动
			P3=0xf0;
			if(P3!=0XF0)
			{
				temp=P3;
				P3=0x0f;
				key=temp|P3;
				key=0xff-key;  //按位取反
				for(i=0;i<16;i++)
				{
					if(key==KEY_TABLE[i])
					{
						num=i;
						break;
					}
				}
				P1=TABLE[num];
				delayl();
			}	
		}
	}
}