//��������
#include "reg51.h"
#include "intrins.h"//����_nop_()����
#define uint unsigned int
#define uchar unsigned char
uchar code line1_data[]="Hello Everyone";//Ҫ��ʾ�ĵ�һ���ַ�
uchar code line2_data[]="Welcome to SDU!";//Ҫ��ʾ�ĵڶ����ַ�
sbit LCD_RS=P1^7;	  //�����������
sbit LCD_RW=P1^6;
sbit LCD_EN=P1^5;
uchar flag=0;

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
	bit result;
	LCD_RS=0;
	LCD_RW=1;
	LCD_EN=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	result=(bit)(P2&0X80);
	LCD_EN=0;
	return result;
}

//д�����ӳ���
void lcd_wcmd(uchar cmd)
{
	while(lcd_busy());
	LCD_RS=0;
	LCD_RW=0;
	LCD_EN=0;
	_nop_();
	_nop_();
	P2=cmd;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=0;
}

//LCD�����ӳ���
void lcd_clr()
{
	lcd_wcmd(0x01);
	delay_ms(2);
}

//д�����ӳ���
void lcd_wdat(uchar dat)
{
	while(lcd_busy());
	LCD_RS=1;
	LCD_RW=0;
	LCD_EN=0;
	_nop_();
	_nop_();
	P2=dat;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN=0;
}

//��ʼ���ӳ���
void lcd_init()
{
	delay_ms(15);//�ȴ�LCD��Դ�ȶ�
	lcd_wcmd(0x38);//�����趨ָ����DL=1,N=1,F=0��8λ���ݿ�ȣ�16*2��ʾ��5*7�����ַ�
	delay_ms(5);
	lcd_wcmd(0x0c);//��ʾ���ؿ���ָ����D=1,C=0,B=0����ʾ�����ع�꣬����˸
	delay_ms(5);
	lcd_wcmd(0x06);//����ģʽ����ָ����I/D=1,S=0,��ַ�Զ�����
	delay_ms(5);
	lcd_wcmd(0x01);//���LCD��ʾ���ݣ�����ָ��DB7-DB0����Ϊ01H
	delay_ms(5);
}

//��˸�ӳ���
void flash()
{
	delay_ms(1000);
	lcd_wcmd(0x08);//��ʾ���ؿ���ָ����D=0,C=0,B=0,��ʾ�أ��ع�꣬����˸
	delay_ms(500);
	lcd_wcmd(0x0c);//��ʾ�����ع�꣬����˸
	delay_ms(500);
	lcd_wcmd(0x08);
	delay_ms(500);
	lcd_wcmd(0x0c);
	delay_ms(500);
	lcd_wcmd(0x08);
	delay_ms(500);
	lcd_wcmd(0x0c);
	delay_ms(500);
}

//������
void main()
{
	uchar i;
	EA=1;	//���ж��ܿ���
	EX0=1;	//���ⲿ�ж�0
	IT0=1;	//�����жϴ�����ʽΪ�½��ش�����ʽ
	EX1=1;
	IT1=1;
	delay_ms(10);
	lcd_init();		//��ʼ��
	lcd_clr();		//����
	delay_ms(5);
	while(1)
	{
		if(flag==1)
		{
			lcd_wcmd(0x00|0x80);	//DDRAM��ַ����ָ���ʾλ��Ϊ��1�е�0��
			i=0;
			while(line1_data[i]!='\0')
			{
				lcd_wdat(line1_data[i]);	//���ص�1���ַ�
				delay_ms(100);
				i++;
			}
			lcd_wcmd(0x40|0x80);	//������ʾλ��Ϊ��2�е�0��
			i=0;
			while(line2_data[i]!='\0')
			{
				 lcd_wdat(line2_data[i]);	//���ص�2���ַ�
				delay_ms(100);
				i++;
			}
			flash();
			flag=0;
		}
		if(flag==2)
		{
			lcd_clr();		//����
			delay_ms(5);
			flag=0;
		}
	}
}				

//�ⲿ�ж�0�ӳ���
void INT_0() interrupt 0
{
	flag=1;
}
//�ⲿ�ж�1�ӳ���
void INT_1() interrupt 1
{
	flag=2;
}