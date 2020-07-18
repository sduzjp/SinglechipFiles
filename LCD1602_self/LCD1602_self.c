//LCD1602�������ʵ�飺����LCD1602��16������ʵ�ּ򵥵�ʮ�������Ӽ��˳�����������
//KEY0-KEY9�ֱ��������0-9��KEY10-KEY13�ֱ�����������+��-��*��/����KEY15����=��
//KEY14�������������ܺ�ʱ������������������ֹͣ������������������㣬��Ļ����
//LCD1602��һ����ʾ�������������������������ڶ�����ʾ���������������Ϊ����
//��������ΧΪ0-32767��������֤��Ϊ0������ʱ����ͬʱ��ʾ��������

#include "reg51.h"
#include "intrins.h"
#define uchar unsigned char
#define	uint unsigned int
//�����������
sbit LCD_RS=P1^7;	//��������/����ѡ��λΪP1.7����  
sbit LCD_RW=P1^6;	//�����/дѡ��λΪP1.6����  
sbit LCD_EN=P1^5;	//����ʹ���ź�λΪP1.5���� 

uchar code TABLE[]={0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0}; //������Ӧ������ֵ
uchar code TABLE1[]={0,1,2,3,4,5,6,7,8,9,0x2b-0x30,0x2d-0x30,
0x2a-0x30,0x2f-0x30,0x01-0x30,0x3d-0x30};//������Ӧ���ַ�
char i,j,temp,keynum,keynumtemp;//temp��Ϊ�жϰ�������ʱ���м������keynum��Ϊ���ű���,keynumtemp�ݴ�keynum��ֵ 
long num1,num2,ans;  //num1��������ĵ�һ������num2��������ĵڶ�������ans������������
uchar flag,symbol;//flag��Ϊ��������ֽ��־������symbol��Ϊ������ű���

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
	result=(bit)(P2&0x80);	 //�����������λ�����Ƿ�æ��resultΪ1����æ��Ϊ0�������
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

//д�����ӳ���
void lcd_wdat(uchar dat)
{
	P2=0;
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
	keynum=0;	   //num���KEY0-KEY15�������ţ���ʼ����-1
	delay_ms(15);//��ʱ15ms���ȴ�LCD��Դ�ȶ�
	lcd_wcmd(0x38);//�����趨ָ����DL=1,N=1,F=0��8λ���ݿ�ȣ�16*2��ʾ2�У�5*7�����ַ�
	delay_ms(5);   //��ʱ5ms
	lcd_wcmd(0x0c);//��ʾ���ؿ���ָ����D=1,C=0,B=0����ʾ�����ع�꣬����˸
	delay_ms(5);   //��ʱ5ms
	lcd_wcmd(0x06);//����ģʽ����ָ����I/D=1,S=0,д�������ݺ��ַ�Զ���������ʾ�����ƶ�
	delay_ms(5);   //��ʱ5ms
	lcd_wcmd(0x01);//���LCD��ʾ���ݣ�����ָ��D7-D0����Ϊ01H
	delay_ms(5);   //��ʱ5ms
	i=0; 
	j=0;		//j=0��ʾ�ڵ�һ��������ʾ�У�j=1��ʾ������һ���������ʾ
	num1=0; 		//����ĵ�һ������0
	num2=0; 		//����ĵڶ�������0
	ans=0; 		//ans���������
	flag=0; 	//flag��ʾ��������ֽ��־������flagΪ0��ʾ���������һ������Ϊ1��ʾ��������ڶ�����
	symbol=0;	//symbol��ʾ�������1��ʾ��+����2��ʾ��-����3��ʾ��*����4��ʾ��/��
}

void keyscan() 
{
	P3=0xfe; 		 //��0����0����������1����ȫ��1
	if(P3!=0xfe) 	//ɨ���0��
	{
		delay_ms(20);  //��ʱ20msȥ����
		if(P3!=0xfe) 
		{
			temp=P3&0xf0; //������Чλ
			switch(temp) 
			{
				case 0xe0:keynum=0; break;	 //˵�����µİ����ڵ�0�е�0�У�����Ϊ0
				case 0xd0:keynum=1; break;  //˵�����µİ����ڵ�0�е�1�У�����Ϊ1		
				case 0xb0:keynum=2; break;	 //˵�����µİ����ڵ�0�е�2�У�����Ϊ2			
				case 0x70:keynum=3; break;  //˵�����µİ����ڵ�0�е�3�У�����Ϊ3
			}
		}
		while(P3!=0xfe);   //�ȴ������ͷ�
		if(keynum==0||keynum==1||keynum==2||keynum==3) 
		{
			if(j!=0)	 //j��־λ���ڶ��μ���ʱ����ǰ�����ʾ��� 
			{
				keynumtemp=keynum; //keynumtemp�ݴ�keynumֵ
				lcd_wcmd(0x01);	  //����
				lcd_init();
				keynum=keynumtemp;//�ָ�keynumֵ
				j=0; 	 
			}
			if(flag==0) num1=num1*10+TABLE[keynum]; //�������һ���� 
			else num2=num2*10+TABLE[keynum];   //������ڶ�����
		}
		i=TABLE1[keynum];  
		lcd_wdat(0x30+i); //��Һ������ʾ������Ӧ���ַ�
	}
	P3=0xfd; 	  //��1����0����������1����ȫ��1,
	if(P3!=0xfd)  //ɨ���1��
	{
		delay_ms(20);  //��ʱ20msȥ����
		if(P3!=0xfd) 
		{
			temp=P3&0xf0; 
			switch(temp) 
			{
				case 0xe0:keynum=4; break;	 //���µİ����ڵ�1�е�0�У�����Ϊ4
				case 0xd0:keynum=5; break;	 //���µİ����ڵ�1�е�1�У�����Ϊ5
				case 0xb0:keynum=6; break;	 //���µİ����ڵ�1�е�2�У�����Ϊ6			
				case 0x70:keynum=7; break;  //���µİ����ڵ�1�е�3�У�����Ϊ7
			} 
		}
		while(P3!=0xfd);  //�ȴ������ͷ�
		if(keynum==4||keynum==5||keynum==6||keynum==7) 
		{
			if(j!=0) 	  //�ڶ��μ���ʱ��ǰ��ļ�����ʾ���
			{
				keynumtemp=keynum; //keynumtemp�ݴ�keynumֵ
				lcd_wcmd(0x01);	  //����
				lcd_init();
				keynum=keynumtemp;//�ָ�keynumֵ
				j=0; 
			}
			if(flag==0) num1=num1*10+TABLE[keynum];   //�������һ����
			else num2=num2*10+TABLE[keynum];  		  //������ڶ�����
		}
	i=TABLE1[keynum]; 
	lcd_wdat(0x30+i); 	//��Һ������ʾ�ַ�
	}
	P3=0xfb; 	  //��2����0����������1����ȫ��1
	if(P3!=0xfb) 	//ɨ���2��
	{
		delay_ms(20);	//��ʱ20msȥ���� 
		if(P3!=0xfb) 
		{
			temp=P3&0xf0; 
			switch(temp) 
			{
				case 0xe0:keynum=8; break;	 //���µİ����ڵ�2�е�0�У�����Ϊ8
				case 0xd0:keynum=9; break;	 //���µİ����ڵ�2�е�1�У�����Ϊ9
				case 0xb0:keynum=10; break;   //���µİ����ڵ�2�е�2�У�����Ϊ10				
				case 0x70:keynum=11; break;   //���µİ����ڵ�2�е�3�У�����Ϊ11
			}
		}
		while(P3!=0xfb);   //�ȴ������ͷ�
		if(keynum==8||keynum==9) 	 //����Ϊ8����9��ʾ�����ַ�
		{
			if(j!=0) 		 //��һ�μ���ʱ����һ�μ�����ʾ���
			{
				keynumtemp=keynum; //keynumtemp�ݴ�keynumֵ
				lcd_wcmd(0x01);	  //����
				lcd_init();
				keynum=keynumtemp;//�ָ�keynumֵ
				j=0; 	 
			}
			if(flag==0) num1=num1*10+TABLE[keynum]; 
			else num2=num2*10+TABLE[keynum];
		}
		if(keynum==10) 		//����Ϊ10��ʾ�������+�� 
		{
			flag=1; 	   //flag��1��֮����������ǵڶ�����
			symbol=1; 	   //symbol��1��ʾ��+��
		}
		if(keynum==11) 		//����Ϊ11��ʾ�������-�� 
		{
			flag=1; 	  //flag��1��֮����������ǵڶ�����
			symbol=2; 	  //symbol��2��ʾ��-��
		} 
		i=TABLE1[keynum]; 
		lcd_wdat(0x30+i);  //��Һ������ʾ�ַ�
	}
	P3=0xf7; 	  //��3����0����������1����ȫ��1
	if(P3!=0xf7)   //ɨ�������
	{
		delay_ms(20);	//��ʱ20msȥ���� 
		if(P3!=0xf7) 
		{	
			temp=P3&0xf0; 
			switch(temp) 
			{
				case 0xe0:keynum=12; break;	//���µİ����ڵ�3�е�0�У�����Ϊ12				
				case 0xd0:keynum=13; break;	//���µİ����ڵ�3�е�1�У�����Ϊ13			
				case 0xb0:keynum=14; break;	//���µİ����ڵ�3�е�2�У�����Ϊ14			
				case 0x70:keynum=15; break; //���µİ����ڵ�3�е�3�У�����Ϊ15
			} 
		}  
		while(P3!=0xf7);  //�ȴ������ͷ�
		switch(keynum) 
		{
			case 12:	 //����Ϊ12��ʾ�������*��
				{
					lcd_wdat(0x30+TABLE1[keynum]); 
					flag=1; 
					symbol=3;  //symbolΪ3��ʾ��*��
				} 
				break;
			case 13:	//����Ϊ13��ʾ�������/�� 
				{ 
					lcd_wdat(0x30+TABLE1[keynum]); 
					flag=1; 
					symbol=4;  //symbolΪ4��ʾ��*��
				} 
				break;
			case 15:	//����Ϊ15��ʾ�������=��������������� 
				{
					j=1;
					if(num2==0)
					{
						lcd_wcmd(0x80+0x4f); //�����һ�����ĵ�ַ��ʼ��ʾ
						lcd_wcmd(0x04); //д�������ݺ��ַ�Ĵ������ݼ�1
						while(num1!=0)
						{
							lcd_wdat(0x30+num1%10); //��10ȡ�෨���ÿһλ����
							num1=num1/10; 
						}
						lcd_wdat(0x3d);
					}
					if(symbol==1)	//symbolΪ1��ʾ�ӷ�����
					{
						lcd_wcmd(0x80+0x4f); //�����һ�����ĵ�ַ��ʼ��ʾ
						lcd_wcmd(0x04); //д�������ݺ��ַ�Ĵ������ݼ�1
						ans=num1+num2; 						
						while(ans!=0) 
						{
							lcd_wdat(0x30+ans%10); //��10ȡ�෨���ÿһλ����
							ans=ans/10; 
						}
						lcd_wdat(0x3d); //�����ʾ��=�� 
						j=1;
						num1=0;	   //�������ͷֽ��־�����Լ������������
						num2=0;
						flag=0;
						symbol=0; 
					}
					else if(symbol==2)	//symbolΪ2��ʾ��������
					{
						lcd_wcmd(0x80+0x4f);  //�����һ�����ĵ�ַ��ʼ��ʾ
						lcd_wcmd(0x04); 
						if(num1-num2>0) ans=num1-num2; //��������
						else ans=num2-num1; 						
						while(ans!=0) 
						{
							lcd_wdat(0x30+ans%10); //��10ȡ�෨�õ�ÿһλ
							ans=ans/10; 
						}
						if(num1-num2<0)lcd_wdat(0x2d);//���Ϊ����ʱ����Ϊ�� ��-�� 
						lcd_wdat(0x3d); //�����ʾ��=��
						j=1;
						num1=0;	   //�������ͷֽ��־�����Լ������������
						num2=0;
						flag=0;
						symbol=0; 
					}
					else if(symbol==3)	//symbolΪ3��ʾ���г˷�����
					{
						lcd_wcmd(0x80+0x4f);  //�����һ�����ĵ�ַ��ʼ��ʾ
						lcd_wcmd(0x04); 
						ans=num1*num2; 	 //�˷�����						
						while(ans!=0)
						{
							lcd_wdat(0x30+ans%10); 	 //��10ȡ�෨�õ�ÿһλ
							ans=ans/10; 
						}
						lcd_wdat(0x3d);	//������ڷ���
						j=1;				 
						num1=0;	   //�������ͷֽ��־�����Լ������������
						num2=0;
						flag=0;
						symbol=0; 
					}
					else if(symbol==4)
					{
						lcd_wcmd(0x80+0x4f); //�����һ�����ĵ�ַ��ʼ��ʾ
						lcd_wcmd(0x04); 
						i=0; 
						if(num2==0)//���������0������ʾerror 
						{ 
							lcd_wdat('R'); 
							lcd_wdat('O'); 
							lcd_wdat('R'); 
							lcd_wdat('R'); 
							lcd_wdat('E'); 
						} 
						else
						{ 
							ans=num1/num2; 	 //�õ���
							ans=num1-num2*ans;	 //�ж�����
							while(ans!=0) 	 //��������
							{
								lcd_wdat(0x30+ans%10);//������� 
								ans=ans/10; 
								i++; 							
							}
							if(i!=0)   //��������ʱ�������С����
							{
								for(i=3;i>0;i--)
								{
									lcd_wdat(0x2e);//�����ʾС���� 
								}
								ans=num1/num2;
								if(ans==0) lcd_wdat(0x30);//0
								while(ans!=0)
								{
									lcd_wdat(0x30+ans%10);//�����
									ans=ans/10;
								}	
							}
							else	 //�������
							{
								ans=num1/num2;
								if(ans==0)	//num1=0��������0
								{
									lcd_wdat(0x30);
								}
								while(ans!=0)
								{
									lcd_wdat(0x30+ans%10);//�����
									ans=ans/10;
								}	
							}
							lcd_wdat(0x3d);
						}
						j=1;
						num1=0;	  //�������ͷֽ��־�����Լ������������
						num2=0;
						flag=0;
						symbol=0; 
					} 
				} 
				break;
			case 14: //����Ϊ14��ʾ�����ʾ 
				{
					lcd_wcmd(0x01);
					j=1;		//������һ�μ�����ʾ
					num1=0;		 //�������ͷֽ��־�����Լ������������
					num2=0;
					flag=0;
					symbol=0;
				} 
				break; 
		} 
	} 
}

void main() 
{ 
	lcd_init();   //LCD1602��ʼ��
	while(1)
	{
		keyscan(); 	 //���ü���ɨ�����ʾ�ӳ�����ʾ���
	} 
}
	 