//��չ����I/O��ʵ�飺ͨ������SW1-SW4�Ŀ���״̬�л�����D1-D4��������ܵ�����
ORG 0000H
AJMP MAIN	 //��ת��������
ORG 0030H

MAIN:		 //������
	MOV SP,#60H		 //ջָ���ʼ��
	MOV DPTR,#0FE00H  //DPTR��ʼ��
LOOP:
	MOVX A,@DPTR	  //����չI/O�ڵ����������ۼ���A
	MOVX @DPTR,A	  //���ۼ���A�е�����������չI/O�����
	SJMP LOOP		  //ִ������ѭ��
END