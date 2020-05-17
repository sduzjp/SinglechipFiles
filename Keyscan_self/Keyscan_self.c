//矩阵键盘扫描自我完成实验：根据点击不同按键完成彩色旋转灯要求
#include "reg51.h"
#include "intrins.h"
#define uint unsigned int
#define uchar unsigned char

uchar code KEY_TABLE[]={0x22,0x12,0x21,0x11};//按键键值表,后四位哪位为1表示按键按下在哪一行
											//前四位哪位为1表示按键按下在哪一列
uchar code T_TABLE[]={200,160,120,80,40};//调用延时子程序次数
//uchar code Out_TABLE[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//旋转控制发光二极管亮灭
uchar code Sout_TABLE[]={0x0F,0x17,0x1B,0x1D,0x1E};//档位输出表

//延时子程序
void delays()
{
	uchar t,ms;
	ms=5;	//延时5ms
	while(ms--)
	for(t=0;t<120;t++);
}
//或者可以采用内嵌汇编
void delays_asm()
{
	#pragma asm
	MOV R2,#50	//5ms延时子程序
DL1:MOV R1,#48
DL2:DJNZ R1,DL2	  //内循环100us
	NOP 
	DJNZ R2,DL1	  //中循环10ms
	#pragma endasm
}

void main()
{
	uchar temp,key;
	uchar keycode,i,j;
	uint DIR=1;		//方向控制变量
	uint m=0;		//旋转控制变量
	uint Speed=0;	  	//速度档位变量
	uchar t=T_TABLE[m];		//延时次数控制变量
	uchar out=0xfe;		 //旋转输出变量，控制发光二极管亮灭情况
	P1=0xff;			 //初始化P1口全置1,发光二极管全熄灭
	P0=Sout_TABLE[Speed];	  //初始化档位设置为1挡
	while(1)
	{
		P3=0x30;		 //行置为0，列置为1，读按键按下的所在列值
		temp=P3;		 //将读取的值保存到temp中
		temp=temp&0x30;	  //保留有效位
		if(temp!=0x30)	  //如果temp!=0x30,则说明有按键按下
		{
			delays();
			delays();	//调用延时子程序，共延时10ms去抖动
			P3=0x30;	//重新判断是否有按键按下
			temp=P3;
			temp=temp&0x30;
			if(temp!=0x30)		//若有按键按下
			{
				key=temp;		//记下按键所在列
				temp=temp|0x03;		//线反转法，列输出行输入
				P3=temp;
				temp=P3&0x03;		//记录按键所在行
				key=temp|key;		//合并得到按下的按键的键码
				for(i=0;i<4;i++)	 //判断按键键号
				{
					if(key==KEY_TABLE[i])keycode=i;
				}	
				if(keycode==0)DIR=1;	//键号为0说明按键按下的是正转键
				else if(keycode==1)DIR=0;  //键号为1说明按键按下的是反转键
				else if(keycode==2)	   //键号为2说明按键按下的是加速键
				{
					Speed++;
					if(Speed==5)Speed=4;  //加速最快只能到五档
				}
				else if(keycode==3)		//键号为3说明按键按下的是减速键
				{
					if(Speed==0)Speed++;   //减速最小只能到一档
					Speed--;
				}
			}	
		}
		m=Speed;
		t=T_TABLE[m]/8;		 //取得延时次数
		if(DIR==1)			 //当正转时
		{
			for(j=8;j>0;j--)
			{
				P1=out;
				for(i=0;i<t;i++)delays();  //每一个发光二极管亮的时间
				out=_crol_(out,1);	  //循环左移，之后下一个发光二极管亮
			}
		}
		else
		{					   //当反转时
			for(j=8;j>0;j--)
			{
				P1=out;
				for(i=0;i<t;i++)delays(); //每一个发光二极管亮的时间
				out=_cror_(out,1);	  //循环左移，之后下一个发光二极管亮
			}
		}
		P0=Sout_TABLE[Speed];	 //设置档位相应的发光二极管亮
		P1=0xff;	   //在下一轮循环间隔之间设置发光二极管均熄灭等待下一轮旋转
	}
}
