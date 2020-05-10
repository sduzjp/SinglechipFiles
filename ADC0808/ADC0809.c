//�������к��Զ���ѯ�ɼ�ADC0808��·����In0��In1��In2��In3�ĵ�ѹ������LCD1604����ʾ
//��������
#include "reg51.h"
#include "intrins.h"//����_nop_()����
#define uint unsigned int
#define uchar unsigned char
uchar data line_data[16];//Ҫ��ʾ��һ���ַ�
uchar code numchar[]={'0','1','2','3','4','5','6','7','8','9'};
sbit LCD_RS=P3^0;
sbit LCD_RW=P3^1;
sbit LCD_EN=P3^4;
sbit AD_CLK=P3^5;
sbit Start=P3^3;
sbit OE=P3^7;
uint var,tmpint;
long tmplong;
uchar channel,samp_data,tmpchar,n;
bit end_samp;

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
	bit flag;
	LCD_RS=0;
	LCD_RW=1;
	LCD_EN=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	flag=(bit)(P1&0X80);
	LCD_EN=0;
	return flag;
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
	P1=cmd;
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
	P1=dat;
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
	delay_ms(5);
	lcd_wcmd(0x01);//���LCD��ʾ����
	delay_ms(5);
	lcd_wcmd(0x06);//�ƶ����
	delay_ms(5);
	lcd_wcmd(0x0c);//��ʾ�����ع��
	delay_ms(5);		 
	lcd_wcmd(0x38);//16*4��ʾ��5*7����
	delay_ms(15);//�ȴ�LCD��Դ�ȶ�
}

//�ַ���ʾ�ӳ���
void showstring(uchar m)
{
	uchar i;
	switch(m)
	{
		case 0:
			lcd_wcmd(0x80);//������ʾλ��Ϊ��1�е�0��
			break;
		case 1:
			lcd_wcmd(0xc0);//������ʾλ��Ϊ��2�е�0��
			break;
		case 2:
			lcd_wcmd(0x90);//������ʾλ��Ϊ��3�е�0��
			break;
		case 3:
			lcd_wcmd(0xd0);//������ʾλ��Ϊ��4�е�0��
	}
	i=0;
	while(line_data[i]!='\0')
	{
		lcd_wdat(line_data[i]);//������ʾ�ַ�
		delay_ms(5);
		i++;
	}
}

//�ⲿ�ж�0�����ӳ���
void s_int0() interrupt 0	//ADC0808�ɼ����
{
	P0=0XFF;
	delay_ms(1);
	OE=1;		//��ADC0808���������
	samp_data=P0;
	OE=0;		//�ر�ADC0808���������
	end_samp=1;
	EX0=0;		//�ر��ⲿ�ж�0
}

//�ⲿ�ж�0�����ӳ���
void s_timer1() interrupt 3		//ADC0808�ɼ����
{
	AD_CLK=~AD_CLK;
}

//ADC0808���ݲɼ������ӳ���
void sample(uchar ch)
{
	while(!end_samp);	//�ȴ�ת�����
	tmplong=(long)samp_data*5*1000;
	var=(uint)(tmplong/255);
	if(var==0)
	{
		n=0;
		line_data[n++]='C';
		line_data[n++]='h';
		line_data[n++]='0'+ch;
		line_data[n++]=':';
		line_data[n++]='0';
		line_data[n++]='V';
	}
	else
	{
		n=0;
		line_data[n++]='C';
		line_data[n++]='h';
		line_data[n++]='0'+ch;
		line_data[n++]=':';
		tmpint=var;

		tmpchar=(uchar)(tmpint/1000);
		tmpint=tmpint-(uint)tmpchar*1000;
		line_data[n++]=numchar[tmpchar];

		line_data[n++]='.';

		tmpchar=(uchar)(tmpint/100);
		tmpint=tmpint-(uint)tmpchar*100;
		line_data[n++]=numchar[tmpchar];

		tmpchar=(uchar)(tmpint/10);
		tmpint=tmpint-(uint)tmpchar*10;
		line_data[n++]=numchar[tmpchar];

		tmpchar=(uchar)(tmpint);
		line_data[n++]=numchar[tmpchar];
		line_data[n++]='V';
	}
	line_data[n]='\0';
}

//������
void main()
{
	delay_ms(10);
	lcd_init();	//��ʼ��
	lcd_clr();	//����
	delay_ms(2);
	channel=0;	//����״̬Ϊ״̬0
	var=0;
	line_data[n]='\0';
	AD_CLK=0;
	OE=0;

	TMOD=0X20;	//T1�����ڷ�ʽ2
	TH1=0XE7;	//10KHZ
	TL1=0XE7;
	EA=1;		//���ж��ܿ���
	EX0=1;	   	//���ⲿ�ж�0
	IT0=1;	   	//�����жϴ�����ʽΪ�½��ش�����ʽ
	ET1=1;
	TR1=1;
	while(1)
	{
		end_samp=0;
		EX0=1;	//���ⲿ�ж�����λ
		P2=channel;
		Start=1;	//����AD
		delay_ms(2);
		Start=0;	//�����źŽ���
		sample(channel);	//�ɼ�
		showstring(channel);	//���
		channel++;	//ͨ�����
		if(channel==4)
		{
			channel=0;
		}
	}


}