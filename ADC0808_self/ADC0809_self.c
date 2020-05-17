//ADC0808�źŲɼ��������ʵ�飺����LCD1602��ADC0808ʵ�ּ򵥵Ľ����źŹ�������Ƶ�ʷ���
//Ҫ���źŷ��ȱ仯ʱ(�����̵�5%-95%)����Ӱ����Ľ����Ƶ�ʼ��Ľ��ͨ��LCD1602
//�ĵ�һ����ʾ�������źŹ���ʱ���ܹ�ͨ��P2.6���һ��������Ϊ5us�������źš�
#include "reg51.h"
#include "intrins.h"
#define uint unsigned int
#define uchar unsigned char

sbit LCD_RS=P2^0;  //��������/����ѡ��λΪP2.0����
sbit LCD_RW=P2^1;  //�����/дѡ��λΪP2.1����
sbit LCD_EN=P2^2;  //����ʹ���źŶ�ΪP2.2����
sbit BF=P1^7;	 //����æµ��־λ��BFΪ1��ʾæµ
sbit CLK=P2^3; 	//���巽������λΪP2.3����
sbit Start=P2^4;  //���忪ʼ��־λΪP2.4����
sbit OE=P2^5;	//��������AD��дλΪP2.5����
sbit Out_pulse=P2^6;  //�����������λΪP2.6����
sbit EOC=P2^7;	  //����ת��������־λ��EOCΪ1��ʾADת������

uchar n=0;	  //�ж�50ms���˶��ٴα���
uchar flag=0;	//ʱ����1s��־λ

//��ʱ�ӳ���
void delay_ms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
	{
		for(j=110;j>0;j--);
	}
}

//LCDæ����ӳ���
bit lcd_busy()
{
	bit result;	//æ��־λ
	LCD_RS=0;	//����/����ѡ��λ�õ͵�ƽ
	LCD_RW=1;	//��/дѡ��λ�øߵ�ƽ
	LCD_EN=1;	//ʹ���źŶ��øߵ�ƽ
	_nop_();
	_nop_();
	_nop_();
	_nop_();	//�൱������nop��ָ��
	result=BF;
	LCD_EN=0;	//ʹ���źŶ��õ͵�ƽ�������½���
	return result;
}

//д�����ӳ���
void lcd_wcmd(uchar cmd)
{
	while(lcd_busy());	//�ж�LCD1604�Ƿ�æ
	LCD_RS=0;	//����/����ѡ��λ�õ͵�ƽ
	LCD_RW=0;	//��/дѡ��λ�õ͵�ƽ
	LCD_EN=0;	//ʹ���źŶ��õ͵�ƽ
	_nop_();
	_nop_();
	P1=cmd;		//����������P1��
	_nop_();
	_nop_();
	_nop_();
	_nop_();	//�൱������nop��ָ��
	LCD_EN=1;	//ʹ���źŶ��øߵ�ƽ
	_nop_();
	_nop_();
	_nop_();
	_nop_();	//�൱������nop��ָ��
	LCD_EN=0;	//ʹ���źŶ��õ͵�ƽ�������½���
}

//д�����ӳ���
void lcd_wdat(uchar dat)
{
	while(lcd_busy());	//��æ�źţ���æ������ִ��
	LCD_RS=1;	//ѡ�����ݼĴ���
	LCD_RW=0;	//��дλ�õ͵�ƽ��д������
	LCD_EN=0;	//ʹ���źŶ��õ͵�ƽ
	_nop_();
	_nop_();
	P1=dat;		//����������P1��
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=1;	//ʹ���źŶ��øߵ�ƽ
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=0;	//ʹ���źŶ��õ͵�ƽ�������½���
}

//LCD��ʼ���ӳ���
void lcd_init()
{
	delay_ms(5);
	lcd_wcmd(0x01);//���LCD��ʾ����
	delay_ms(5);
	lcd_wcmd(0x06);//�ƶ����,д�������ݺ��ַ�Ĵ��������ݼ�1
	delay_ms(5);
	lcd_wcmd(0x0c);//��ʾ�����ع��
	delay_ms(5);		 
	lcd_wcmd(0x38);//16*4��ʾ��5*7����
	delay_ms(15);//�ȴ�LCD��Դ�ȶ�
}

//ϵͳ��ʼ���ӳ���
void systeminit()
{
	TMOD=0x21;	//�趨��ʱ��T1�����ڷ�ʽ2����ʱ��T0�����ڷ�ʽ1
	TH1=0xEC;  //50KHZ�����ü�����ֵ������T1�жϲ���CLK�ź�
	TL1=0xEC;
	EA=1;	//�����ж�
	ET1=1;	//����ʱ��T1�ж�
	TR1=1;	//�����ж�
	TH0=0x3C;	//50ms��ʱ
	TL0=0xB0;
	ET0=1;	 //����ʱ��T0�ж�
	TR0=1;	 //�����ж�
	Start=0;  //A/Dת�������źţ�����ߵ�ƽ��Ч
	OE=0;  //������������źţ�����ߵ�ƽ��Ч
}

//������0�жϷ����ӳ���
void time0() interrupt 1
{
	ET0=0; 	//���ж�
	TH0=0x3C;//���ü�ʱ����ֵ 
	TL0=0xB0;//50MS��ʱ 
	n++; 
	if(n==50)  //1s��flag��1��Ȼ�����Ƶ�ʼ���
	{
		flag=1;
		n=0;
	} 
	ET0=1; 	//���ж�
}

//��ʱ/������1�жϷ����ӳ���
void time1() interrupt 3 
{ 
	ET1=0; 	 //���ж�
	CLK=~CLK;  //CLK�ź�ȡ��
	ET1=1;	 //���ж�
} 

//ADC0808���ݲɼ�
uchar ADC_sample()
{
	uchar temp;		//�����ݴ�������ݱ��� 
	Start=1;	//����A/Dת��
	delay_ms(1);
	Start=0; 	//�����źŽ���
	while(!EOC);	//�ȴ�ADת������  
	P0=0xff; 	//P0��ȫ��1 
	delay_ms(1); 
	OE=1;	//��ADC0808�����ʹ�������������
	temp=P0;	//��ȡP0�ڲɼ����� 
	OE=0;	//�ر�ADC0808�����ʹ��
	delay_ms(4);
	return temp;	//���زɼ����� 
} 

//�ַ���ʾ�ӳ���
void display(uint ch)
{
	uint num;	  //�����ݴ��ַ���λ����
	lcd_wcmd(0x82);	  //������ʾλ��Ϊ��1�е�3��
	num=ch/10;
	lcd_wdat(0x30+num);	 //��ʾʮλ

	num=ch%10;
	lcd_wdat(0x30+num);	 //��ʾ��λ
}

//������
void main()
{
	uint temp1=0,temp2=0;//temp1�ݴ��ϴβ������ݣ�temp2�ݴ汾�β�������
	uchar frequency=0;	//frequency��Ƶ��
	lcd_init();	//LCD��ʼ��
	delay_ms(5);
	systeminit();		//ϵͳ��ʼ��
	CLK=0;
	lcd_wcmd(0x80);	 //������ʾλ��Ϊ��1�е�0��
	lcd_wdat('f');	 //�����ʾf:
	lcd_wdat(':');
	lcd_wcmd(0x84);	 //������ʾλ��Ϊ��1�е�6��
	lcd_wdat('H');	 //�����ʾHZ
	lcd_wdat('z');
	while(1)
	{
		temp2=temp1;
		temp2=ADC_sample();	 //����ADC0808���ݲ����ӳ����ȡ��������
		if(((temp1>=128)&&(temp2<=128))||((temp1<=128)&&(temp2>=128)))//�жϲ����������������Ǹ���
		{
			frequency++;  //����һ�ι���㣬frequency����1
			Out_pulse=1;
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();   
			Out_pulse=0;  //����5us�������ź�
		}
		if(flag==1)	   //1s�����Ƶ��
		{
			flag=0;
			frequency=frequency/2;		//Ƶ��Ϊfrequency/2
			display(frequency);	 //��ʾƵ��
			frequency=0;
		}
	}
}