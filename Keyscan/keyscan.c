//��������
#include "reg51.h"
#define uint unsigned int
#define uchar unsigned char
uchar code KEY_TABLE[]={0x11,0x21,0x41,0x81,0x12,0x22,0x42,0x82,0x14,
						0x24,0x44,0x84,0x18,0x28,0x48,0x88};	 //������ֵ��
						//����ֵ����ǰ��λ�밴���������йأ�����λ�밴���������й�
						//��λ����������Ϊ1����һλ���ڵ�λ�þ��ǰ������ڵ��л��е�λ��
						//���簴��0x24��0010 0100��ʾ�ڵڶ��е�һ��;
						//��0x82��1000 0010��ʾ�ڵ�һ�е�����
uchar code TABLE[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
					0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ���

//����ʱ�ӳ����������������ʱ
void delayl()
{
	uint n=50000;
	while(n--);
}
//����ʱ�ӳ���������ȥ��������
void delays()
{
	uint n=10000;
	while(n--);
}

//������
void main()
{
	uchar temp,key,num,i;
	P1=0x0;
	while(1)
	{
		P3=0xf0;		  //����Ϊ1������Ϊ1������ֵ
		if(P3!=0XF0)
		{
			delays();	  //ȥ����
			P3=0xf0;
			if(P3!=0XF0)
			{
				temp=P3;
				P3=0x0f;
				key=temp|P3;
				key=0xff-key;  //��λȡ��
				for(i=0;i<16;i++)
				{
					if(key==KEY_TABLE[i])
					{
						num=i;
						break;
					}
				}
				P1=TABLE[num];
				delayl();
			}	
		}
	}
}