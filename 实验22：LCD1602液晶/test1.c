//LCD1602��ʾʵ�飺�������KEY1��LCD1602��һ����ʾ��Hello Everyone��,
//�ڶ�����ʾ��Welcome to SDU��,����˸3�Ρ��������KEY2��LCD1602����
//��������
#include "reg51.h"
#define uint unsigned int		//���������������Ͷ���
#define uchar unsigned char
uchar code line1_data[]="Hello Everyone";//Ҫ��ʾ�ĵ�һ���ַ�
uchar code line2_data[]="Welcome to SDU!";//Ҫ��ʾ�ĵڶ����ַ�
//�����������
sbit LCD_RS=P2^6;	//��������/����ѡ��λΪP1.7����  
sbit LCD_RW=P2^5;	//�����/дѡ��λΪP1.6����  
sbit LCD_EN=P2^7;	//����ʹ���ź�λΪP1.5���� 

void delay_n40us(uint n)
{
	uint i;
	uchar j;
	for(i=n;i>0;i--)
	{
		for(j=0;j<2;j++);
	}
}

//д�����ӳ���
void lcd_wcmd(uchar cmd)
{
	LCD_RS=0;	 //ѡ��ָ��Ĵ���
	LCD_RW=0;	 //��дѡ��λ�õ͵�ƽ��д��ָ��
	P0=cmd;		 //��P2��д��ָ��
	LCD_EN=1;	 //ʹ�ܶ��øߵ�ƽ
	LCD_EN=0;	 //ʹ�ܶ��õ͵�ƽ�������½��أ���Һ��ģ��ִ������
	delay_n40us(1);
}

//д�����ӳ���
void lcd_wdat(uchar dat)
{
	LCD_RS=1;	  //ѡ�����ݼĴ���
	LCD_RW=0;	  //��дѡ��λ�õ͵�ƽ����д����
	P0=dat;		  //��P2��д������
	LCD_EN=1;	  //ʹ�ܶ��øߵ�ƽ
	LCD_EN=0;	 //ʹ�ܶ��õ͵�ƽ�������½��أ�Һ��ģ��ִ������
	delay_n40us(1);
}

//��ʼ���ӳ���
void lcd_init()
{
	lcd_wcmd(0x38);//�����趨ָ����DL=1,N=1,F=0��8λ���ݿ�ȣ�16*2��ʾ2�У�5*7�����ַ�
	lcd_wcmd(0x0c);//��ʾ���ؿ���ָ����D=1,C=0,B=0����ʾ�����ع�꣬����˸
	lcd_wcmd(0x06);//����ģʽ����ָ����I/D=1,S=0,д�������ݺ��ַ�Զ���������ʾ�����ƶ�
	lcd_wcmd(0x01);//���LCD��ʾ���ݣ�����ָ��D7-D0����Ϊ01H
	delay_n40us(100);
}

void charDisplay()
{
	uchar i;
	lcd_init();
	for(i=0;i<15;i++)
	{
		lcd_wdat(line2_data[i]);	
	}
}
//������
void main()
{
	lcd_init();		//��ʼ��
	charDisplay();
	lcd_wcmd(0xc0);
	lcd_wdat('!');
	lcd_wdat('z');
	while(1);
}				
