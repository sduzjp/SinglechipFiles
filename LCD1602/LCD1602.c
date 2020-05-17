//LCD1602��ʾʵ�飺�������KEY1��LCD1602��һ����ʾ��Hello Everyone��,
//�ڶ�����ʾ��Welcome to SDU��,����˸3�Ρ��������KEY2��LCD1602����
//��������
#include "reg51.h"
#include "intrins.h"//����_nop_()����
#define uint unsigned int		//���������������Ͷ���
#define uchar unsigned char
uchar code line1_data[]="Hello Everyone";//Ҫ��ʾ�ĵ�һ���ַ�
uchar code line2_data[]="Welcome to SDU!";//Ҫ��ʾ�ĵڶ����ַ�
//�����������
sbit LCD_RS=P1^7;	//��������/����ѡ��λΪP1.7����  
sbit LCD_RW=P1^6;	//�����/дѡ��λΪP1.6����  
sbit LCD_EN=P1^5;	//����ʹ���ź�λΪP1.5���� 
sbit int0=P3^2;
sbit int1=P3^3;
uchar flag=0;	 //������ʾ��־λ

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
	bit result;	   //����λ����result
	LCD_RS=0;	   //ѡ��ָ��Ĵ���
	LCD_RW=1;	   //��/дѡ��λ�øߵ�ƽ����æ�ź�
	LCD_EN=1;	   //ʹ���źŶ��øߵ�ƽ
	_nop_();
	_nop_();
	_nop_();
	_nop_();	  //�൱������nop��ָ��
	result=(bit)(P2&0X80);	 //�����������λ�����Ƿ�æ��resultΪ1����æ��Ϊ0�������
	LCD_EN=0;	  //ʹ���źŶ��õ͵�ƽ
	return result;	 
}

//д�����ӳ���
void lcd_wcmd(uchar cmd)
{
	while(lcd_busy());	//��æ�źţ���æ������ִ��
	LCD_RS=0;	 //ѡ��ָ��Ĵ���
	LCD_RW=0;	 //��дѡ��λ�õ͵�ƽ��д��ָ��
	LCD_EN=0;	 //��ʼ��ʹ�ܶ��õ͵�ƽ
	_nop_();
	_nop_();	 //������ָ��
	P2=cmd;		 //��P2��д��ָ��
	_nop_();
	_nop_();
	_nop_();
	_nop_();	 //������ָ��
	LCD_EN=1;	 //ʹ�ܶ��øߵ�ƽ
	_nop_();
	_nop_();
	_nop_();
	_nop_();	 //������ָ��
	LCD_EN=0;	 //ʹ�ܶ��õ͵�ƽ�������½��أ���Һ��ģ��ִ������
}

//LCD�����ӳ���
void lcd_clr()
{
	lcd_wcmd(0x01);	  //����LCDд����д������ָ�D7-D0����Ϊ01H
	delay_ms(2);	  //��ʱ2ms������ָ����ʱ1.64ms����
}

//д�����ӳ���
void lcd_wdat(uchar dat)
{
	while(lcd_busy());	 //��æ�źţ���æ������ִ��
	LCD_RS=1;	  //ѡ�����ݼĴ���
	LCD_RW=0;	  //��дѡ��λ�õ͵�ƽ����д����
	LCD_EN=0;	  //��ʼ��ʹ�ܶ��õ͵�ƽ
	_nop_();
	_nop_();	  //������ָ��
	P2=dat;		  //��P2��д������
	_nop_();
	_nop_();
	_nop_();
	_nop_();	  //������ָ��
	LCD_EN=1;	  //ʹ�ܶ��øߵ�ƽ
	_nop_();
	_nop_();
	_nop_();
	_nop_();	 //������ָ��
	LCD_EN=0;	 //ʹ�ܶ��õ͵�ƽ�������½��أ�Һ��ģ��ִ������
}

//��ʼ���ӳ���
void lcd_init()
{
	delay_ms(15);//��ʱ15ms���ȴ�LCD��Դ�ȶ�
	lcd_wcmd(0x38);//�����趨ָ����DL=1,N=1,F=0��8λ���ݿ�ȣ�16*2��ʾ2�У�5*7�����ַ�
	delay_ms(5);   //��ʱ5ms
	lcd_wcmd(0x0c);//��ʾ���ؿ���ָ����D=1,C=0,B=0����ʾ�����ع�꣬����˸
	delay_ms(5);   //��ʱ5ms
	lcd_wcmd(0x06);//����ģʽ����ָ����I/D=1,S=0,д�������ݺ��ַ�Զ���������ʾ�����ƶ�
	delay_ms(5);   //��ʱ5ms
	lcd_wcmd(0x01);//���LCD��ʾ���ݣ�����ָ��D7-D0����Ϊ01H
	delay_ms(5);   //��ʱ5ms
}

//��˸�ӳ���
void flash()
{
	delay_ms(1000);	//��ʱ1s
	lcd_wcmd(0x08);//��ʾ���ؿ���ָ����D=0,C=0,B=0,��ʾ�أ��ع�꣬����˸
	delay_ms(500);	//��ʱ0.5s
	lcd_wcmd(0x0c);//D=1,C=0,B=0����ʾ�����ع�꣬����˸��֮�����ο���ʾ����ʾ
	delay_ms(500);	//��ʱ0.5s
	lcd_wcmd(0x08);	
	delay_ms(500); //��ʱ0.5s
	lcd_wcmd(0x0c);
	delay_ms(500);	//��ʱ0.5s
	lcd_wcmd(0x08);
	delay_ms(500);	//��ʱ0.5s
	lcd_wcmd(0x0c);
	delay_ms(500);	//��ʱ0.5s
}

//������
void main()
{
	uchar i;
	EA=1;	//���ж��ܿ���
	EX0=1;	//���ⲿ�ж�0
	IT0=1;	//�����ⲿ�ж�0������ʽΪ�½��ش�����ʽ
	EX1=1;	//���ⲿ�ж�1
	IT1=1;	//�����ⲿ�ж�1������ʽΪ�½��ش�����ʽ
	delay_ms(10);	//��ʱ10ms
	lcd_init();		//��ʼ��
	lcd_clr();		//����
	delay_ms(5);	//��ʱ5ms
	while(1)
	{
		if(flag==1)
		{
			lcd_clr();		//����
			delay_ms(5);	//��ʱ5ms
			lcd_wcmd(0x00|0x80);	//DDRAM��ַ����ָ���ʾλ��Ϊ��1�е�0��
			i=0;
			while(line1_data[i]!='\0')
			{
				lcd_wdat(line1_data[i]);	//���ص�1���ַ�
				delay_ms(100);		 //��ʱ100ms
				i++;
			}
			lcd_wcmd(0x40|0x80);	//������ʾλ��Ϊ��2�е�0��
			i=0;
			while(line2_data[i]!='\0')
			{
				lcd_wdat(line2_data[i]);	//���ص�2���ַ�
				delay_ms(100);	   //��ʱ100ms
				i++;
			}
			flash();	 //������˸�ӳ���
			flag=0;		 //��ʾ���flag��0���ֵ�ǰ��ʾ״̬�ȴ���һ���ж�
		}
		if(flag==2)
		{
			lcd_clr();		//����
			delay_ms(5);	//��ʱ5ms
			flag=0;		  //�������flag��0���ֵ�ǰ��ʾ״̬�ȴ���һ���ж�
		}
	}
}				

//�ⲿ�ж�0�ӳ���
void INT_0() interrupt 0   //���KEY1�����ⲿ�ж�0�����ӳ��򣬽���Һ����ʾ
{
	flag=1;
}
//�ⲿ�ж�1�ӳ���		 //���KEY2�����ⲿ�ж�1�����ӳ��򣬽�������
void INT_1() interrupt 2
{
	flag=2;
}

//�ж�				�жϺ�		��ڵ�ַ
//�ⲿ�ж�0			0			0003H
//��ʱ/������0		1			000BH
//�ⲿ�ж�1			2			0013H
//��ʱ/������1		3			001BH
//���п�			4			0023H