/**************************************************************************************
*		              LCD1602Һ����ʾʵ��												  *
ʵ���������س�������LCD1602Һ���ڿ������ϣ�������ʾ
ע�������������õ���ģ����ʾ�����Խ�74HC595ģ���ϵ�JP595�̽�Ƭ�ε���																				  
***************************************************************************************/

#include "reg51.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "lcd.h"

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;
//������� ����
u8 Buffer[5]={0};
u8 Disp[]=" Pechin Science ";
u8 Disp1[]=" Hello  World!  ";

void display()
{
	u8 i;
	LcdInit();
	for(i=0;i<16;i++)
	{
		LcdWriteData(Disp[i]);	
	}
}

void display1()
{
	u8 i;
	LcdInit();
	for(i=0;i<16;i++)
	{
		LcdWriteData(Disp1[i]);	
	}
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

void SendOneByte(uchar c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}

void UART_SER () interrupt 4
{
	u16 n; 	//������ʱ����
	if(RI) 		//�ж��ǽ����жϲ���
	{
		RI=0; 	//��־λ����
		n=SBUF; //���뻺������ֵ
		switch(n)
		{
			case 1:	
					display();	
					SendOneByte(n);
					break;
			case 2:	
					display1();	
					SendOneByte(n);
					break;
		}
	}	
}
/*******************************************************************************
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void main(void)
{
	LcdInit();
	InitUART();
	while(1);				
}
