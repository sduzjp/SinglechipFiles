#include <reg51.h>
#define uchar unsigned char
#define uint unsigned int

uchar SetTime[14];
uchar SetTimeIndex=0,flag=0;
sbit LCD_RS=P2^6;	//��������/����ѡ��λΪP1.7����  
sbit LCD_RW=P2^5;	//�����/дѡ��λΪP1.6����  
sbit LCD_EN=P2^7;	//����ʹ���ź�λΪP1.5���� 
//��ʱ�ӳ���
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

void InitUART(void)
{
   	SCON = 0x50; 	// SCON: ģʽ1, 8-bit UART, ʹ�ܽ��� 
	TMOD |= 0x20;
	TH1=0xfd; 		//������9600 ��ֵ
	TL1=0xfd;
	TR1= 1;
	EA = 1;	    //�����ж�
	ES= 1; 		//�򿪴����ж�
}

void charDisplay(uchar end)
{
	uchar i;
	lcd_init();
	for(i=0;i<end;i++)
	{
		lcd_wdat(SetTime[i]+0x30);	
	}
}

void main(void)
{
    InitUART();
	while(1);
}
/*void SendOneByte(uchar c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}*/	 

void UART_SER () interrupt 4
{
	uint n; 	//������ʱ����
	if(RI) 		//�ж��ǽ����жϲ���
	{
		RI=0; 	//��־λ����
		n=SBUF;
		if(SetTimeIndex<14)
		{
			SetTime[SetTimeIndex]=n;
			SetTimeIndex++;
		}
		else
		{
			SBUF=0x00;
			while(!TI);
    		TI = 0;
		}
		charDisplay(SetTimeIndex);
	}
}	  

 
 