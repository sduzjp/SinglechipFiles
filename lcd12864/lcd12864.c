#include <reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
#define DATA P0
sbit RS=P2^2;
sbit RW=P2^1;
sbit EN=P2^0;
sbit cs1=P2^3;
sbit cs2=P2^4;
/****************************定义字库*********************************/
uchar code Hzk[]={
0x00,0x80,0x40,0x20,0x18,0x06,0x80,0x00,0x07,0x18,0x20,0x40,0x80,0x00,0x00,0x00,
0x01,0x00,0x20,0x70,0x28,0x26,0x21,0x20,0x20,0x24,0x38,0x60,0x00,0x01,0x01,0x00,

/*--  文字:  历  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=16x16   --*/
0x00,0x00,0xFE,0x02,0x42,0x42,0x42,0x42,0xFA,0x42,0x42,0x42,0x42,0xC2,0x02,0x00,
0x80,0x60,0x1F,0x80,0x40,0x20,0x18,0x06,0x01,0x00,0x40,0x80,0x40,0x3F,0x00,0x00,};
/**********************************延时子程序**********************/
void delay(uint xms)
{
uint i,j;
for(i=xms;i>0;i--)
for(j=110;j>0;j--);
}
/**********************************LCD忙检查子程序**********************/
void CheckState()
{
uchar dat;
RS=0;
RW=1;
do {
DATA=0x00;
EN=1;
_nop_();
dat=DATA;
EN=0;
dat=0x80&dat; //检查忙信号
}while(!(dat==0x00)); //当忙信号为0时才可继续操作
}
/**********************************写命令子程序**********************/
void SendCommandToLCD(uchar com)
{
CheckState();
RS=0; //写命令
RW=0;
DATA=com;
EN=1; //利用EN 下降沿完成命令写操作
_nop_();
_nop_();
EN=0;
}
void SetLine(uchar page) //设置页码，页码为0~7
{
page=0xb8|page;
SendCommandToLCD(page);
}
void SetStartLine(uchar startline) //设置起始行，行号为0~63
{
startline=0xc0|startline;
SendCommandToLCD(startline);
}
void SetColumn(uchar column) //设置列，列号为0~63
{
column=column&0x3f;
column=0x40|column;
SendCommandToLCD(column);
}
void SetOnOff(uchar onoff) //开关显示屏，onoff只能为0或1
{
onoff=0x3e|onoff;
SendCommandToLCD(onoff);
}
void WriteByte(uchar dat) //写数据
{
CheckState();
RS=1;
RW=0;
DATA=dat;
EN=1;
_nop_();
_nop_();
EN=0;
}
void SelectScreen(uchar screen) //选屏，screen=0,1,2
{
switch(screen)
{
case 0: cs1=0; //全屏显示
_nop_();
_nop_();
_nop_();
cs2=0;
_nop_();
_nop_();
_nop_();
break;
case 1: cs1=0; //左屏显示
_nop_();
_nop_();
_nop_();
cs2=1;
_nop_();
_nop_();
_nop_();
break;
case 2: cs1=1; //右屏显示
_nop_();
_nop_();
_nop_();
cs2=0;
_nop_();
_nop_();
_nop_();
break;
}
}
void ClearScreen(uchar screen) //清屏，screen=0,1,2
{
uchar i,j;
SelectScreen(screen);
for (i=0;i<8;i++)
{
SetLine(i);
SetColumn(0);
for(j=0;j<64;j++)
{WriteByte(0x00); //写数据列地址将自动加1
}
}
}
/**********************************初始化子程序**********************/
void InitLCD()
{
CheckState();
SelectScreen(0);
SetOnOff(0); //关屏
SelectScreen(0);
SetOnOff(1); //开屏
SelectScreen(0);
ClearScreen(0); //清屏
SetStartLine(0); //开始行为0
}
/************************显示全角汉字**********************/
void Display(uchar ss,uchar page,uchar column,uchar number)
{
int i;
SelectScreen(ss); //ss为屏号
column=column&0x3f; //column为列号
SetLine(page); //page为页号，显示第number个汉字的上半部分，
//page可理解为要显示的汉字位于屏幕的第page行
SetColumn(column);
for(i=0;i<16;i++) //i为一个字里面的各个列
{
WriteByte(Hzk[i+32*number]); //number为字号，
//取第number个汉字的第i列数据编码值
}
SetLine(page+1); //显示第number个汉字的下半部分
SetColumn(column);
for(i=0;i<16;i++)
{
WriteByte(Hzk[i+32*number+16]);//取第number个汉字的下半部分
}}
void main()
{
uint i;
InitLCD();
ClearScreen(0);
while(1)
{
//for (i=0;i<128;i++)
//{
SetStartLine(i);
Display(1,0,0*16,0); //显示第0号字，"山"
Display(1,0,1*16,1); //显示第1号字，"大"
/*Display(1,0,2*16,2); //显示第2号字，"电"
Display(1,0,3*16,3); //显示第3号字，"工"
Display(2,0,4*16,4); //显示第4号字，"电"
Display(2,0,5*16,5); //显示第5号字，"子"
Display(2,0,6*16,6); //显示第6号字，"中"
Display(2,0,7*16,7); //显示第7号字，"心"*/
SelectScreen(0);
delay(50);
//}
}
}