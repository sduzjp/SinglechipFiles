//ADC0808�źŲɼ�ʵ�飺�������к��Զ���ѯ�ɼ�ADC0808��·�����IN0,IN1,IN2,IN3�ĵ�ѹ
//����LCD1604����ʾ
//LCD1604Ϊ4*16�ַ���LCD��ʾ������LCD1602������ֻ�Ƕ������У�������������ʱ��Ҫ��һ��
//��ʱ��T1����10KHZ��CLK�����ź�
//��������
#include "reg51.h"
#include "intrins.h"//����_nop_()�������൱��һ��nop��ָ��
#define uint unsigned int
#define uchar unsigned char
uchar data line_data[16];//Ҫ��ʾ��һ���ַ�
uchar code numchar[]={'0','1','2','3','4','5','6','7','8','9'};
//��������
sbit LCD_RS=P3^0;  //��������/����ѡ��λΪP3.0
sbit LCD_RW=P3^1;  //�����/дѡ��λΪP3.1
sbit LCD_EN=P3^4;  //����ʹ���źŶ�ΪP3.4
sbit AD_CLK=P3^5;  //���巽��CLK��ΪP3.5
sbit Start=P3^3;   //����StartΪP3.3
sbit OE=P3^7;	   //���������A/D�����ΪP3.7

uint var,tmpint;   //varΪ������ѹֵ��tmpint�ݴ������ѹֵ��λ��ֵ
long tmplong;  //�ݴ潫��������ת��Ϊ�����͵�����
uchar channel,samp_data,tmpchar,n; //channel���ͨ��ֵ��samp_data��Ų�����P0�����ݣ�tmpchar�ݴ������ѹ��λ��ֵ��Ӧ���ַ���nΪ������������
bit end_samp;  //end_sampΪ1��ʾת��������Ϊ0��ʾ��ʼת��

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
	bit flag;	//æ��־λ
	LCD_RS=0;	//����/����ѡ��λ�õ͵�ƽ
	LCD_RW=1;	//��/дѡ��λ�øߵ�ƽ
	LCD_EN=1;	//ʹ���źŶ��øߵ�ƽ
	_nop_();
	_nop_();
	_nop_();
	_nop_();	//�൱������nop��ָ��
	flag=(bit)(P1&0x80);
	LCD_EN=0;	//ʹ���źŶ��õ͵�ƽ�������½���
	return flag;
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

//LCD�����ӳ���
void lcd_clr()
{
	lcd_wcmd(0x01);	   //����
	delay_ms(2);
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

//��ʼ���ӳ���
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
		lcd_wdat(line_data[i]);//������ʾ��m+1���ַ�
		delay_ms(5);   //��ʱ5ms
		i++;
	}
}

//�ⲿ�ж�0�����ӳ���
void s_int0() interrupt 0	//ADC0808ת�����ʱE0C����ߵ�ƽ������������͵�ƽ��P3.2���ţ������ⲿ�ж�0
{
	P0=0xff;	  //P0��ȫ�øߵ�ƽ�����������ȫ��1
	delay_ms(1);
	OE=1;		//��ADC0808�����ʹ�ܣ��ߵ�ƽ��Ч
	samp_data=P0;	//����P0�ڵ�����
	OE=0;		//�ر�ADC0808�����ʹ��
	end_samp=1;	   //��ʾת������
	EX0=0;		//�ر��ⲿ�ж�0
}

//��ʱ/������1�жϷ����ӳ���
void s_timer1() interrupt 3		//ADC0808�ɼ����
{
	AD_CLK=~AD_CLK;		//�����ķ����ź�ȡ��
}

//ADC0808���ݲɼ������ӳ���
void sample(uchar ch)	 //ch���ͨ��ֵ
{
	while(!end_samp);	//�ȴ�ת����ɣ�end_sampΪ1ʱ��ʾת��������¼���ִ��
	tmplong=(long)samp_data*5*1000;	 //��������P0������ת��Ϊ���������ݣ�����1000��ʹ����������
	var=(uint)(tmplong/255); //ת��Ϊ������ѹֵ
	if(var==0)		//��������ѹΪ0Vʱ����Ҫ��ʾ��һ���ַ���ӵ�������
	{
		n=0;
		line_data[n++]='C';	  //�������ַ���ָ��������һ��Ԫ��
		line_data[n++]='h';
		line_data[n++]='0'+ch;
		line_data[n++]=':';
		line_data[n++]='0';
		line_data[n++]='V';
	}
	else	   //������ѹֵ��Ϊ0Vʱ
	{
		n=0;
		line_data[n++]='C';
		line_data[n++]='h';
		line_data[n++]='0'+ch;
		line_data[n++]=':';
		tmpint=var;			//tmpint�ݴ�����ĵ�ѹֵ

		tmpchar=(uchar)(tmpint/1000);	//ȡ�õ�ѹֵ�����λ���������ֲ�ת��Ϊ�ַ�
		tmpint=tmpint-(uint)tmpchar*1000;  //tmpint�ݴ�Ӵθ�λ��ʼ�ĵ�ѹֵ
		line_data[n++]=numchar[tmpchar];   //�����λ�ַ���������
		
		line_data[n++]='.';		//��С�����������

		tmpchar=(uchar)(tmpint/100);
		tmpint=tmpint-(uint)tmpchar*100;
		line_data[n++]=numchar[tmpchar]; //��С�����ֵ�ʮ��λ��������

		tmpchar=(uchar)(tmpint/10);
		tmpint=tmpint-(uint)tmpchar*10;
		line_data[n++]=numchar[tmpchar]; //��С�����ֵİٷ�λ��������

		tmpchar=(uchar)(tmpint);
		line_data[n++]=numchar[tmpchar]; //��С�����ֵ�ǧ��λ��������
		line_data[n++]='V';		//�ڵ�ѹֵ������ӵ�ѹ��λV��ʾ�ַ�
	}
	line_data[n]='\0';	  //��ʾ�ַ�������־
}

//������
void main()
{
	delay_ms(10); //��ʱ10ms
	lcd_init();	//��ʼ��
	lcd_clr();	//����
	delay_ms(2);  //��ʱ2ms
	channel=0;	//����״̬ͨ��Ϊͨ��0
	var=0;	   //��ʼ������ѹֵ��0
	line_data[n]='\0';	//��ʾ�ַ���ʼ��־
	AD_CLK=0;	//�����ź��õ͵�ƽ
	OE=0;	 //��ʼ���ر�ADC0808�����ʹ��

	TMOD=0X20;	//��ʱ��T1�����ڷ�ʽ2�������Զ���װ�ع���
	TH1=0XE7;	//10KHZ	   1/4����������=25us����������Ϊ1us����������ֵΪ231=11100111B
	TL1=0XE7;	//TH�����ֵ���䣬TL���м���
	EA=1;		//���ж��ܿ���
	EX0=1;	   	//���ⲿ�ж�0
	IT0=1;	   	//�����жϴ�����ʽΪ�½��ش�����ʽ
	ET1=1;		//�򿪶�ʱ��1�ж�
	TR1=1;		//������ʱ��
	while(1)
	{
		end_samp=0;	  //ת����־λ��0
		EX0=1;	//���ⲿ�ж�����λ
		P2=channel;	 //����ǰ״̬����P2��
		Start=1;	//����ADת��
		delay_ms(2);
		Start=0;	//�����źŽ���
		sample(channel);	//�������ݲɼ������ӳ���
		showstring(channel);	//�����ַ���ʾ�ӳ����������ͨ���Ĳ�����ѹֵ
		channel++;	//ͨ�����
		if(channel==4)		  //LCD1604������ʵ4��ͨ���Ĳ�����ѹֵ
		{
			channel=0;
		}
	}


}