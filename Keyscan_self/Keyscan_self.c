//�������ɨ���������ʵ�飺���ݵ����ͬ������ɲ�ɫ��ת��Ҫ��
#include "reg51.h"
#include "intrins.h"
#define uint unsigned int
#define uchar unsigned char

uchar code KEY_TABLE[]={0x22,0x12,0x21,0x11};//������ֵ��,����λ��λΪ1��ʾ������������һ��
											//ǰ��λ��λΪ1��ʾ������������һ��
uchar code T_TABLE[]={200,160,120,80,40};//������ʱ�ӳ������
//uchar code Out_TABLE[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};//��ת���Ʒ������������
uchar code Sout_TABLE[]={0x0F,0x17,0x1B,0x1D,0x1E};//��λ�����

//��ʱ�ӳ���
void delays()
{
	uchar t,ms;
	ms=5;	//��ʱ5ms
	while(ms--)
	for(t=0;t<120;t++);
}
//���߿��Բ�����Ƕ���
void delays_asm()
{
	#pragma asm
	MOV R2,#50	//5ms��ʱ�ӳ���
DL1:MOV R1,#48
DL2:DJNZ R1,DL2	  //��ѭ��100us
	NOP 
	DJNZ R2,DL1	  //��ѭ��10ms
	#pragma endasm
}

void main()
{
	uchar temp,key;
	uchar keycode,i,j;
	uint DIR=1;		//������Ʊ���
	uint m=0;		//��ת���Ʊ���
	uint Speed=0;	  	//�ٶȵ�λ����
	uchar t=T_TABLE[m];		//��ʱ�������Ʊ���
	uchar out=0xfe;		 //��ת������������Ʒ���������������
	P1=0xff;			 //��ʼ��P1��ȫ��1,���������ȫϨ��
	P0=Sout_TABLE[Speed];	  //��ʼ����λ����Ϊ1��
	while(1)
	{
		P3=0x30;		 //����Ϊ0������Ϊ1�����������µ�������ֵ
		temp=P3;		 //����ȡ��ֵ���浽temp��
		temp=temp&0x30;	  //������Чλ
		if(temp!=0x30)	  //���temp!=0x30,��˵���а�������
		{
			delays();
			delays();	//������ʱ�ӳ��򣬹���ʱ10msȥ����
			P3=0x30;	//�����ж��Ƿ��а�������
			temp=P3;
			temp=temp&0x30;
			if(temp!=0x30)		//���а�������
			{
				key=temp;		//���°���������
				temp=temp|0x03;		//�߷�ת���������������
				P3=temp;
				temp=P3&0x03;		//��¼����������
				key=temp|key;		//�ϲ��õ����µİ����ļ���
				for(i=0;i<4;i++)	 //�жϰ�������
				{
					if(key==KEY_TABLE[i])keycode=i;
				}	
				if(keycode==0)DIR=1;	//����Ϊ0˵���������µ�����ת��
				else if(keycode==1)DIR=0;  //����Ϊ1˵���������µ��Ƿ�ת��
				else if(keycode==2)	   //����Ϊ2˵���������µ��Ǽ��ټ�
				{
					Speed++;
					if(Speed==5)Speed=4;  //�������ֻ�ܵ��嵵
				}
				else if(keycode==3)		//����Ϊ3˵���������µ��Ǽ��ټ�
				{
					if(Speed==0)Speed++;   //������Сֻ�ܵ�һ��
					Speed--;
				}
			}	
		}
		m=Speed;
		t=T_TABLE[m]/8;		 //ȡ����ʱ����
		if(DIR==1)			 //����תʱ
		{
			for(j=8;j>0;j--)
			{
				P1=out;
				for(i=0;i<t;i++)delays();  //ÿһ���������������ʱ��
				out=_crol_(out,1);	  //ѭ�����ƣ�֮����һ�������������
			}
		}
		else
		{					   //����תʱ
			for(j=8;j>0;j--)
			{
				P1=out;
				for(i=0;i<t;i++)delays(); //ÿһ���������������ʱ��
				out=_cror_(out,1);	  //ѭ�����ƣ�֮����һ�������������
			}
		}
		P0=Sout_TABLE[Speed];	 //���õ�λ��Ӧ�ķ����������
		P1=0xff;	   //����һ��ѭ�����֮�����÷�������ܾ�Ϩ��ȴ���һ����ת
	}
}
