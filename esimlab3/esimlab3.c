//�����ܣ�ʵ�ִ�ӡ���
#include<reg51.h>
#include<stdio.h>
#define uchar unsigned char	 //���޷����ַ����������ͽ������¶���
#define uint unsigned int 	 //���޷��������������ͽ������¶���

uchar data a[32] _at_ 0x30;//������32���޷����ַ����������飬���趨����a����ʼ��ַΪ30H
uint i _at_ 0x55;  //������i���ڵ�ַ55H
							
//��ʱ�ӳ���
void DelayMS(uint ms){
	uchar t;
	while (ms--) for (t=0;t<120;t++);//ѭ������ms*120 	      
}

//������
void main(){
	SP=0x60;   //���ö�ջָ��λ��
	SCON=0x52; //
	TMOD=0x20; //���ſ�T1ѡ��ʽ2��ʱ
	TH1=0xf3;  //���ó�ʼֵ
	TR1=1;	//������ʱ�����м�����3��ΪPRINTF����������
	for(i=0;i<32;i++){
		a[i]=1;	 //���θ�����Ԫ����1
		printf("It is : %d now.\n",i);	//��ӡ����ִ�е���Ϣ
		DelayMS(20000);  //������ʱ�ӳ���             
	}
	while(1);
}


