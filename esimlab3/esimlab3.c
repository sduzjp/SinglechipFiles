//程序功能：实现打印输出
#include<reg51.h>
#include<stdio.h>
#define uchar unsigned char	 //对无符号字符型数据类型进行重新定义
#define uint unsigned int 	 //对无符号整形数据类型进行重新定义

uchar data a[32] _at_ 0x30;//定义存放32个无符号字符型数据数组，并设定数组a的起始地址为30H
uint i _at_ 0x55;  //将变量i放在地址55H
							
//延时子程序
void DelayMS(uint ms){
	uchar t;
	while (ms--) for (t=0;t<120;t++);//循环次数ms*120 	      
}

//主程序
void main(){
	SP=0x60;   //设置堆栈指针位置
	SCON=0x52; //
	TMOD=0x20; //非门控T1选择方式2定时
	TH1=0xf3;  //设置初始值
	TR1=1;	//启动定时，此行及以上3行为PRINTF函数所必须
	for(i=0;i<32;i++){
		a[i]=1;	 //依次给数组元素置1
		printf("It is : %d now.\n",i);	//打印程序执行的信息
		DelayMS(20000);  //调用延时子程序             
	}
	while(1);
}


