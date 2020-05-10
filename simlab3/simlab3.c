#include <reg51.h>
unsigned char code LED_CODES[]=	//0-F的显示码
{0xc0,0xf9,0xa4,0xb0,0x99,	//0-4
0x92,0x82,0xf8,0x80,0x90,	//5-9
0x88,0x83,0xc6,0xa1,0x86,0x8e}; //A-F

void main(){
	unsigned char i=0;				
	unsigned char j=0;
	P1=255;	   //为输入做准备
	P0=255;	   //使输出7段数码管不亮
	while(1){
		do{
			i=P1&1;	  //按键
		}while(i==1);	   //等待按键按下
		P0=LED_CODES[j];   //查输出码
		do{
			i=P1&1;
		}while(i==0);	  //等待按键释放
		j++;
		if(j==16) j=0;	  //将显示变量限制在0-15之间
	}
}