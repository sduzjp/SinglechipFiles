//��̬LED��ʾʵ�飺ʵ��һ������ʱ�����Ķ�����ܵļ�ʱ��λ�ֱ�Ϊ10s��1s��0.1s��0.01s
//��ͣ����������������ͣ������㰴����������㣬���������ָʾ����״̬
ORG 0000H
AJMP MAIN  //��ת��������
ORG 0030H

MAIN:
	MOV SP,#60H	 //ջָ���ʼ��
	CLR F0	  //�û���־λF0����
	SETB P1.2  //P1.2��1�������������
	LCALL DISCLR  //������ʾ�����ӳ����������������
LOOP:
	SETB P1.1	 //P1.1��1
	JB P1.1,GOON   //��P1.1=1��CLEAR�����ɿ�����ת��GOON
WAITO:			//����P1.1=0��CLEAR�������£�����ִ��
	SETB P1.1	//P1.1��1
	JNB P1.1,WAITO	//P1.1=0,��ת��WAITO,�ȴ�CLEAR�����ͷ�
	LCALL DISCLR   //������ʾ�����ӳ���
	CLR F0	 //F0����
GOON:
	SETB P1.0	//P1.0��1
	JB P1.0,NEXT1	//��P1.0=1��Start/Stop���ɿ�����ת��NEXT1
WAIT1:				//����P1.0=0��Start/Stop�����£�����ִ��
	SETB P1.0	   //P1.0��1
	JNB P1.0,WAIT1	//��P1.0=0����ת��WAIT1���ȴ�Start/Stop�����ͷ�
	CPL F0		 //F0ȡ��
NEXT1:
	JNB F0,NEXT	  //��F0=0����ת��NEXT
	CLR P1.2	  //����F0=1,P1.2���㣬ִ������ʱ���ܣ��ҷ����������
	LCALL DISPLAY	 //������ʾ�ӳ���
	LCALL DELAY10ms	 //������ʱ10ms�ӳ���
	LCALL ADJUST	 //���������ʾ�����ӳ���
	SJMP LOOP		 //��ת��LOOP��ѭ��
NEXT:
	SETB P1.2		 //P1.2��1
	SJMP LOOP		 //��ת��LOOP��ѭ��

DELAY10ms:		//10ms��ʱ�ӳ���(48*2+1+1+2)*100+1~=10ms
	DL0:MOV R2,#100
	DL1:MOV R1,#48
	DL2:DJNZ R1,DL2	  //��ѭ��100us
		NOP
		DJNZ R2,DL1	  //��ѭ��10ms
		RET			  //�ӳ��򷵻�

ADJUST:			   //�������������ӳ��������ʾ������Ϊ0-9
	INC 30H		   //30H��Ԫ��ֵ����1
	MOV A,30H	   //��30H��Ԫ��ֵ�����ۼ���A
	CJNE A,#10,GOTORET	//��A��ֵ������10������ת��GOTORET��
	MOV 30H,#0	   //��(A)=10,��30H��Ԫ��ֵ��0
	INC 31H		   //31H��Ԫ��ֵ����1
	MOV A,31H	   //��31H��Ԫ��ֵ�����ۼ���A
	CJNE A,#10,GOTORET //��A��ֵ������10������ת��GOTORET��
	MOV 31H,#0	   //��(A)=10,��31H��Ԫ��ֵ��0
	INC 32H		   //32H��Ԫ��ֵ����1
	MOV A,32H	   //��32H��Ԫ��ֵ�����ۼ���A
	CJNE A,#10,GOTORET	//��A��ֵ������10������ת��GOTORET��
	MOV 32H,#0	   //��(A)=10,��32H��Ԫ��ֵ��0
	INC 33H		  //33H��Ԫ��ֵ����1
	MOV A,33H	  //��33H��Ԫ��ֵ�����ۼ���A
	CJNE A,#10,GOTORET	//��A��ֵ������10������ת��GOTORET��
	MOV 33H,#0	  //��(A)=10,��33H��Ԫ��ֵ��0
GOTORET:RET		 //�ӳ��򷵻�

DISCLR:			//��ʾ�����ӳ���
	MOV 30H,#0	 //�ĸ��������ʾ����
	MOV 31H,#0
	MOV 32H,#0
	MOV 33H,#0
	CLR A	  //�ۼ���A����
	MOV DPTR,#TABLE	  
	MOVC A,@A+DPTR	  //���
	MOV DPTR,#D10msADD	 //ȨֵΪ10ms����ֵ��Ԫ��ַ��DPTR
	MOVX @DPTR,A		 //����
	MOV DPTR,#D100msADD	 //ȨֵΪ100ms����ֵ��Ԫ��ַ��DPTR
	MOVX @DPTR,A		 //����
	MOV DPTR,#D1sADD	 //ȨֵΪ1s����ֵ��Ԫ��ַ��DPTR
	MOVX @DPTR,A		 //����
	MOV DPTR,#D10sADD	 //ȨֵΪ10s����ֵ��Ԫ��ַ��DPTR
	MOVX @DPTR,A
	SETB P1.2  //P1.2��1
	RET		   //�ӳ��򷵻�

DISPLAY:	   //��ʾ�ӳ���
	MOV A,30H		//30H��Ԫ��ֵ���ۼ���A
	MOV DPTR,#TABLE
	MOVC A,@A+DPTR	//�ֶ���ʾ���� 
	MOV DPTR,#D10msADD //ȨֵΪ10ms����ֵ��Ԫ��ַ��DPTR
	MOVX @DPTR,A	//��ʾx10ms����ܵ���ֵ 
	MOV A,31H		//31H��Ԫ��ֵ���ۼ���A
	MOV DPTR,#TABLE
	MOVC A,@A+DPTR	//�ֶ���ʾ���� 
	MOV DPTR,#D100msADD	 //ȨֵΪ100ms����ֵ��Ԫ��ַ��DPTR
	MOVX @DPTR,A	//��ʾx100ms����ܵ���ֵ
	MOV A,32H		//32H��Ԫ��ֵ���ۼ���A
	MOV DPTR,#TABLE
	MOVC A,@A+DPTR	//�ֶ���ʾ���� 
	MOV DPTR,#D1sADD //ȨֵΪ1s����ֵ��Ԫ��ַ��DPTR
	MOVX @DPTR,A	//��ʾx1s����ܵ���ֵ
	MOV A,33H		//33H��Ԫ��ֵ���ۼ���A
	MOV DPTR,#TABLE
	MOVC A,@A+DPTR	//�ֶ���ʾ����
	MOV DPTR,#D10sADD //ȨֵΪ10s����ֵ��Ԫ��ַ��DPTR
	MOVX @DPTR,A	 //��ʾx10s����ܵ���ֵ
	RET				//�ӳ��򷵻�

TABLE:
	DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H	//7��LED������ʾ��
	D10msADD EQU 0FE00H	  //x0.01s��ʾ�����ַ
	D100msADD EQU 0FD00H  //x0.1s��ʾ�����ַ
	D1sADD EQU 0FB00H	  //x1s��ʾ�����ַ
	D10sADD EQU 0F700H	  //x10s��ʾ�����ַ
END

