//��չ����I/O��ʵ�飺����ʵ�ֽ�ͨ�źŵƿ��ƹ��ܣ�����˳��Ϊ
//(1)�ϱ��̵�����ͬʱ���������10s
//(2)�ϱ��Ƶ�����ͬʱ���������2s
//(3)�ϱ��������ͬʱ�����̵���10s
//(4)�ϱ��Ƶ�����ͬʱ�����̵���2s
//(5)�ظ�(1)--(4)

ORG 0000H
AJMP MAIN	 //��ת��������
ORG 0030H

MAIN:		//������
	MOV SP,#60H	//ջָ���ʼ��
	MOV A,#0FFH	//������373���������λ��1
	MOV DPTR,#0FE00H
	MOVX @DPTR,A	//U4�����ȫ��1��U4�����ú�ƺͻƵƵ�����	
	MOV DPTR,#0FD00H
	MOVX @DPTR,A   //U5�����ȫ��1��U5�ĵ���λ�������̵Ƶ�����	
STAT1:		//״̬1,�ϱ��̵��������������10s
	MOV A,#0F3H
	MOV DPTR,#0FE00H
	MOVX @DPTR,A  //U4�����ϱ��̵���
	MOV A,#0CH
	MOV DPTR,#0FD00H
	MOVX @DPTR,A  //U5���ö��������
	LCALL DELAY10S
STAT2:			//״̬2���ϱ��Ƶ��������������2s
	MOV A,#0C3H
	MOV DPTR,#0FE00H
	MOVX @DPTR,A	//�����ϱ��Ƶ��������������
	MOV A,#0FH
	MOV DPTR,#0FD00H
	MOVX @DPTR,A	//�����̵�ȫ��
	LCALL DELAY2S
STAT3:	   			//״̬3���ϱ�������������̵���10s
	MOV A,#0FCH
	MOV DPTR,#0FE00H
	MOVX @DPTR,A	 //�����ϱ������
	MOV A,#03H
	MOV DPTR,#0FD00H
	MOVX @DPTR,A	//���ö����̵���
	LCALL DELAY10S
STAT4:				//״̬4���ϱ�������������Ƶ���2s
	MOV A,#3CH
	MOV DPTR,#0FE00H
	MOVX @DPTR,A	//�����ϱ�������������Ƶ���
	MOV A,#0FH
	MOV DPTR,#0FD00H
	MOVX @DPTR,A   //�����̵�ȫ��
	LCALL DELAY2S
	LJMP STAT1		//�ظ�����״̬ѭ��

DELAY2S:				//2s��ʱ�ӳ���,((248*2+1+1+2)*200+1+2)*20+1~=2s
	DL1_0:MOV R7,#20
	DL1_1:MOV R6,#200
	DL1_2:MOV R5,#248
	DL1_3:DJNZ R5,DL1_3	 //��ѭ��
		NOP
		DJNZ R6,DL1_2	 //��ѭ��
		DJNZ R7,DL1_1 	 //��ѭ��
	RET

DELAY10S:				//10s��ʱ�ӳ���,((248*2+1+1+2)*200+1+2)*100+1~=10s
	DL0:MOV R7,#100
	DL1:MOV R6,#200
	DL2:MOV R5,#248
	DL3:DJNZ R5,DL3	 //��ѭ��
		NOP
		DJNZ R6,DL2	 //��ѭ��
		DJNZ R7,DL1 	 //��ѭ��
	RET
END



	