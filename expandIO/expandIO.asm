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
	CLR P2^1
	LCALL DELAY1S
	SETB P2^1
	LCALL DELAY1S
	SJMP LOOP		  //ִ������ѭ��
DELAY1S:				//1s��ʱ�ӳ���,((248*2+1+1+2)*200+1+2)*10+1=1s
	DL1_0:MOV R7,#10
	DL1_1:MOV R6,#200
	DL1_2:MOV R5,#248
	DL1_3:DJNZ R5,DL1_3	 //��ѭ��
		NOP
		DJNZ R6,DL1_2	 //��ѭ��
		DJNZ R7,DL1_1 	 //��ѭ��
	RET
END
