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
/****************************�����ֿ�*********************************/
uchar code Hzk[]={
0x00,0x80,0x40,0x20,0x18,0x06,0x80,0x00,0x07,0x18,0x20,0x40,0x80,0x00,0x00,0x00,
0x01,0x00,0x20,0x70,0x28,0x26,0x21,0x20,0x20,0x24,0x38,0x60,0x00,0x01,0x01,0x00,

/*--  ����:  ��  --*/
/*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0xFE,0x02,0x42,0x42,0x42,0x42,0xFA,0x42,0x42,0x42,0x42,0xC2,0x02,0x00,
0x80,0x60,0x1F,0x80,0x40,0x20,0x18,0x06,0x01,0x00,0x40,0x80,0x40,0x3F,0x00,0x00,};
/**********************************��ʱ�ӳ���**********************/
void delay(uint xms)
{
uint i,j;
for(i=xms;i>0;i--)
for(j=110;j>0;j--);
}
/**********************************LCDæ����ӳ���**********************/
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
dat=0x80&dat; //���æ�ź�
}while(!(dat==0x00)); //��æ�ź�Ϊ0ʱ�ſɼ�������
}
/**********************************д�����ӳ���**********************/
void SendCommandToLCD(uchar com)
{
CheckState();
RS=0; //д����
RW=0;
DATA=com;
EN=1; //����EN �½����������д����
_nop_();
_nop_();
EN=0;
}
void SetLine(uchar page) //����ҳ�룬ҳ��Ϊ0~7
{
page=0xb8|page;
SendCommandToLCD(page);
}
void SetStartLine(uchar startline) //������ʼ�У��к�Ϊ0~63
{
startline=0xc0|startline;
SendCommandToLCD(startline);
}
void SetColumn(uchar column) //�����У��к�Ϊ0~63
{
column=column&0x3f;
column=0x40|column;
SendCommandToLCD(column);
}
void SetOnOff(uchar onoff) //������ʾ����onoffֻ��Ϊ0��1
{
onoff=0x3e|onoff;
SendCommandToLCD(onoff);
}
void WriteByte(uchar dat) //д����
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
void SelectScreen(uchar screen) //ѡ����screen=0,1,2
{
switch(screen)
{
case 0: cs1=0; //ȫ����ʾ
_nop_();
_nop_();
_nop_();
cs2=0;
_nop_();
_nop_();
_nop_();
break;
case 1: cs1=0; //������ʾ
_nop_();
_nop_();
_nop_();
cs2=1;
_nop_();
_nop_();
_nop_();
break;
case 2: cs1=1; //������ʾ
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
void ClearScreen(uchar screen) //������screen=0,1,2
{
uchar i,j;
SelectScreen(screen);
for (i=0;i<8;i++)
{
SetLine(i);
SetColumn(0);
for(j=0;j<64;j++)
{WriteByte(0x00); //д�����е�ַ���Զ���1
}
}
}
/**********************************��ʼ���ӳ���**********************/
void InitLCD()
{
CheckState();
SelectScreen(0);
SetOnOff(0); //����
SelectScreen(0);
SetOnOff(1); //����
SelectScreen(0);
ClearScreen(0); //����
SetStartLine(0); //��ʼ��Ϊ0
}
/************************��ʾȫ�Ǻ���**********************/
void Display(uchar ss,uchar page,uchar column,uchar number)
{
int i;
SelectScreen(ss); //ssΪ����
column=column&0x3f; //columnΪ�к�
SetLine(page); //pageΪҳ�ţ���ʾ��number�����ֵ��ϰ벿�֣�
//page�����ΪҪ��ʾ�ĺ���λ����Ļ�ĵ�page��
SetColumn(column);
for(i=0;i<16;i++) //iΪһ��������ĸ�����
{
WriteByte(Hzk[i+32*number]); //numberΪ�ֺţ�
//ȡ��number�����ֵĵ�i�����ݱ���ֵ
}
SetLine(page+1); //��ʾ��number�����ֵ��°벿��
SetColumn(column);
for(i=0;i<16;i++)
{
WriteByte(Hzk[i+32*number+16]);//ȡ��number�����ֵ��°벿��
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
Display(1,0,0*16,0); //��ʾ��0���֣�"ɽ"
Display(1,0,1*16,1); //��ʾ��1���֣�"��"
/*Display(1,0,2*16,2); //��ʾ��2���֣�"��"
Display(1,0,3*16,3); //��ʾ��3���֣�"��"
Display(2,0,4*16,4); //��ʾ��4���֣�"��"
Display(2,0,5*16,5); //��ʾ��5���֣�"��"
Display(2,0,6*16,6); //��ʾ��6���֣�"��"
Display(2,0,7*16,7); //��ʾ��7���֣�"��"*/
SelectScreen(0);
delay(50);
//}
}
}