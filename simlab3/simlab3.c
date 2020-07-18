//程序功能：实现按键按下数码管显示，第一次按下显示0，往后按下依次增1显示，0-F循环显示
#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int
uchar code LED_CODES[]=				//0-F的显示码
{0xc0,0xf9,0xa4,0xb0,0x99,	//0-4
0x92,0x82,0xf8,0x80,0x90,	//5-9
0x88,0x83,0xc6,0xa1,0x86,0x8e}; //A-F

void main(){
	uchar i=0;	  //i存放P1口数据			
	uchar j=0;	 //j存放显示的数字
	P1=255;	   //初始化，P1口全置1，为输入做准备
	P0=255;	   //初始化，P0口全置1，使输出7段数码管不亮
	while(1){
		do{
			i=P1&1;	  //按键，读P1口数据
		}while(i==1);	   //等待按键按下
		P0=LED_CODES[j];   //查输出码，送入P0口进行数码管显示
		do{
			i=P1&1;
		}while(i==0);	  //等待按键释放
		j++;			 //按下按键一次自增1
		if(j==16) j=0;	  //将显示变量限制在0-15之间
	}
}