;�������ɨ������������飺���ݵ����ͬ������ɲ�ɫ��ת��Ҫ��

ORG 0000H
AJMP MAIN	;��ת��������
ORG 0030H

MAIN:	
	MOV SP,#60H	   ;ջָ���ʼ��
	MOV 30H,#1	   ;30H�����ת����DIR
	MOV 31H,#0	   ;31H�����ת�ٶ�Speed�Լ���Ӧ����
	MOV 32H,#0FEH	;32H��Ԫ�����ת��ɫ��������
	MOV A,32H
	MOV P1,A	 ;��32H��Ԫ��ֵ��P1��
	MOV 33H,#0	;������ް������µı�־,��Ϊ0��ʾ�ް������£�1��ʾ�а�������
	MOV 34H,#0	;��Ű��µİ����ļ���
LOOP:
	MOV 33H,#0
	LCALL KEYSCAN	  ;���ü���ɨ���ӳ���
	MOV A,33H		 
	JZ LOOP1	  ;�ж����ް������£���(A)=0˵���ް���������ת��LOOP1������������в��������ӳ���
	LCALL Changeref	  ;�������в��������ӳ���
LOOP1:
	MOV P1,32H	  ;����ת��ɫ�����������ʾ������P1��
	MOV A,31H	  ;����ת�ٶ������ۼ���A
	MOV DPTR,#T_TABLE
	MOVC A,@A+DPTR	;�������ת����
	MOV R1,A	 ;����ת���ڱ����üĴ���R1��
	MOV A,31H	 ;����ת�ٶ��ٴ������ۼ���A
	MOV DPTR,#Sout_TABLE
	MOVC A,@A+DPTR	 ;����õ�λ�����
	MOV P0,A	  ;����λ���������P0���ö�Ӧ��λ�����������
	MOV A,30H	  ;��DIRֵ�ж���ת����
	JNZ LOOP3	  ;��(A)=1��ת����ת
LOOP2:			 ;ѭ��������ʾ�������ת��ɫ�Ʒ�ת
	MOV A,32H
	RR A
	MOV 32H,A
	LJMP LOOP4
LOOP3:		   ;ѭ��������ʾ�������ת��ɫ����ת
	MOV A,32H
	RL A
	MOV 32H,A
LOOP4:			;������ת��ɫ�Ƶ���ʾ
	MOV P1,A	;����ʾ������P1��
	MOV A,R1	;����ת���������ۼ���A
	MOV B,#08H
	DIV AB		;�����ھ��ֳ�8�ݣ�ÿһ�ݱ�ʾһ��LED����ʾ��ʱ��
	MOV R1,A
DELAY:		   ;����ÿ���Ƶ���ʾ
	LCALL DELAY5MS
	DJNZ R1,DELAY
	LJMP LOOP

DELAY100MS:					   //(248*2+1+1+2)*200+1~=100MS
	DL_0:MOV R6,#200
	DL_1:MOV R5,#248
	DL_2:DJNZ R5,DL_2	  ;��ѭ��
		NOP
		DJNZ R6,DL_1	  ;��ѭ��
	RET
	
DELAY5MS:	   	;��ʱ5ms�ӳ���		(23*2+1+1+2)*100+1~=5ms
	DL0:MOV R7,#100
	DL1:MOV R6,#23
	DL2:DJNZ R6,DL2	  ;��ѭ��
		NOP
		DJNZ R7,DL1	  ;��ѭ��
	RET

KEYSCAN:		   ;����ɨ���ӳ���
	MOV P3,#3CH		;//����0����1�������µİ�������ֵ
	MOV A,P3
	ANL A,#30H		;//������Чλ
	CJNE A,#30H,ASKEY	;//��(A)!=30H˵���а�����������ת��ASKEY�������ж�һ��
	LJMP TORET	   ;//������ת��TORET������
ASKEY:			   ;//�����ж��Ƿ��а�������
	LCALL DELAY100MS ;//��ʱ15msȥ����
	MOV P3,#3CH
	MOV A,P3
	ANL A,#30H
	CJNE A,#30H,ISKEY	;//�ظ������жϣ���(A)!=30H˵���а�����������ת��ISKEY�����м���ȷ��
	MOV 33H,#0	   ;//����33H��0˵���ް�������
	MOV 34H,#0	   ;//34H��Ԫ��ż�����0
	LJMP TORET	   ;//��ת��TORET������
ISKEY:
	ORL A,#0C0H	   ;//���е�����λ��1
	MOV 34H,A	   ;//����ֵ���浽34H��Ԫ��ż���
SEARCHLINE0:	   ;//ɨ����̵�0��
	MOV P3,#3EH
	MOV A,P3
	ANL A,#30H
	CLR C
	SUBB A,#30H	   
	JNZ SAVELINE0	;//��(A)!=30H˵�����µİ����ڵ�0������ת��SAVELINE0������ֵ
SEARCHLINE1:		;//�������ɨ���1��
	MOV P3,#3DH
	MOV A,P3
	ANL A,#30H
	CLR C
	SUBB A,#30H
	JNZ SAVELINE1	;//��(A)!=30H˵�����µİ����ڵ�1������ת��SAVELINE1������ֵ
	MOV 33H,#0		;//����Ϊ��Ч������33H��Ԫ��0
	MOV 34H,#0		//���������ŵ�Ԫ��0
	LJMP TORET		//��ת��TORET������
SAVELINE0:			//�������ڵ�0��ʱ�����0�м�ֵ
	MOV A,#0EH
	SJMP KEYNUM
SAVELINE1:		  //�������ڵ�1��ʱ�����1�м�ֵ
	MOV A,#0DH
KEYNUM:
	ORL A,34H	 //�ϲ����м�ֵ
	CPL A
	MOV 34H,A	//�����µİ�����ֵ��ŵ�34H��Ԫ	
	MOV 33H,#1	//33H��Ԫ��1˵���а�������
TORET:RET		//�ӳ��򷵻�

Changeref:		//��ת�Ʋ��������ӳ���
	MOV A,34H	   //��������ֵ�����ۼ���A
	CJNE A,#11H,KEYCODE1	 //����ֵ��Ϊ11H�������һ����ֵ�ж�
	MOV 30H,#1	   //˵������Ϊ��ת��DIR=1
	LJMP TORET1
KEYCODE1:
	CJNE A,#21H,KEYCODE2   //����ֵ��Ϊ21H�������һ����ֵ�ж�
	MOV 30H,#0		 //˵������Ϊ��ת��DIR=0
	LJMP TORET1
KEYCODE2:
	CJNE A,#12H,KEYCODE3  //����ֵ��Ϊ12H�������һ����ֵ�ж�
	INC 31H		 //˵������Ϊ������Speed����1
	MOV A,31H
	CJNE A,#5,TORET1	//��Ϊ��ߵ�Ϊ5����Speed���ֵΪ4
	MOV 31H,#4
	LJMP TORET1
KEYCODE3:
	CJNE A,#22H,WRONGKEY  //����ֵ��Ϊ22H��Ϊ���󰴼���ֵ��ת��WRONGKEY
	MOV A,31H		  //˵������Ϊ����
	CJNE A,#0,NEXT1	   //��Ϊ��͵�Ϊ1��������Speed��СֵΪ0
	INC 31H
NEXT1:
	DEC 31H
	LJMP TORET1
WRONGKEY:			 //˵��Ϊ���󰴼�����33H��34H��Ԫ����0
	MOV 33H,#0
	MOV 34H,#0
TORET1:
	RET

T_TABLE:DB 0C8H,0A0H,78H,50H,28H ;//��ת���ڿ��Ʊ��ֱ��Ӧ1��-5��
KEY_TABLE:DB 22H,12H,21H,11H	 ;//����������ֱ��Ӧ��ת����ת�����١����ٰ���
Sout_TABLE:DB 0FH,17H,1BH,1DH,1EH 	;//��λ���ֱ���1��-5��
END
