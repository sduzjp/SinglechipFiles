//��������I/O���������ʵ�飺������SW1����ʱ��D1-D8��������������ʱ��Ϊ100ms��
//������ͣ�º�ֹͣ�ֻ����ٴΰ��º�����ֻ�
ORG 0000H
AJMP MAIN
ORG 0030H

MAIN:			//������
	MOV SP,#60H	//ջָ���ʼ��
	MOV P2,#0FFH ;P2������λ�øߵ�ƽ����λʱP2������λ�Զ�Ϊ�ߵ�ƽ
	MOV A,#0FEH
	MOV 20H,A	//����ʾ���嵥Ԫ20H��ʼ��ΪFEH
LOOP:
	JNB P1.0,LOOP1	 //��P1.0=0������ת��LOOP1ִ��ѭ����ʾ
	SJMP LOOP		 //��������ȴ�
LOOP1:
	MOV A,20H		 //����ʾ��������������P2��
	MOV P2,A
	LCALL DELAY100MS  //����100ms��ʱ�ӳ���
	//LCALL DELAY10MS  ;����10ms��ʱ�ӳ���
	//LCALL DELAY1S  ;����1s��ʱ�ӳ���
	RL A			  //����ʾ������������ѭ�����ƺ����ͻ���ʾ������
	MOV 20H,A
	JB P1.0,LOOP	  //��P1.0=1�������ɿ�������ת��LOOP���ظ���������
	LJMP LOOP1		  //P1.0=0������һֱ������ִ��LOOP1ѭ��

DELAY10MS:			 	//10ms��ʱ�ӳ���,(48*2+1+1+2)*100+1=10ms
	DL0:MOV R2,#100
	DL1:MOV R1,#48
	DL2:DJNZ R1,DL2	   //��ѭ��
		NOP
		DJNZ R2,DL1	   //��ѭ��
	RET
DELAY100MS:	 			//100ms��ʱ�ӳ���,(248*2+1+1+2)*200+1=100ms
	DL_0:MOV R2,#200
	DL_1:MOV R1,#248
	DL_2:DJNZ R1,DL_2	   //��ѭ��
		NOP
		DJNZ R2,DL_1	   //��ѭ��
	RET				   //�ӳ��򷵻�
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
