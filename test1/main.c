#include<reg51.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int 

uchar data a[32] _at_ 0x30;
uint i _at_ 0x55;
							
//延时子程序
void DelayMS(uint ms){
	uchar t;
	while (ms--) for (t=0;t<120;t++);	      
}

//主程序
void main(){
	SP=0x60;
	SCON=0x52;
	TMOD=0x20;
	TH1=0xf3;
	TR1=1;
	for(i=0;i<32;i++){
		a[i]=1;
		printf("It is : %d now.\n",i);
		DelayMS(20000);               
	}
	while(1);
}


