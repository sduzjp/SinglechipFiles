//�������ɨ��ʵ�飺�������KEYiʱ���������ʾ��Ӧ�ַ�i��i��0ȡ��F
//������
ORG 0000H
AJMP MAIN //��ת��������
ORG 0030H

MAIN:				 //������
	MOV SP,#60H		 //ջָ�����ó�ֵ
	MOV P1,#00H		 //P1����0�������Ϩ��
LOOP:
	MOV 30H,#0		 //30H��Ԫ������ް������µı�־��1��ʾ���£�0��ʾδ����
	MOV 31H,#0		 //31H��Ŷ�Ӧ��������
	LCALL KEYSCAN	 //���þ���ɨ���ӳ���ȷ���������µ�λ��
	MOV A,30H		 
	JZ LOOP			 //�жϰ����Ƿ��£�(A)=0��ʾ����δ������ת��LOOP��������(A)=1�������£����¼���ִ��
	MOV 32H,#0		 //32H��Ԫ��Ű����Ƿ����1��ʾ�д���0��ʾ�޴���
	MOV 33H,#0		 //33H��Ԫ��ż���
	LCALL SEARCHKEYNUM	 //����
	MOV A,32H		 
	JNZ LOOP		 //�жϰ����Ƿ��Ŵ���(A)=1��ʾ�д��󷵻ص�LOOP����������ִ��
	MOV DPTR,#TABLE2
	MOV A,33H
	MOVC A,@A+DPTR	  //���ݴ�ŵļ��Ų��õ���Ӧ����ʾ��
	MOV P1,A		  //����ʾ������P1���������ʾ
	LJMP LOOP		//��ת��LOOPѭ��

KEYSCAN:		  //���̾���ɨ���ӳ��򣬸���ɨ�跨ȷ�����µİ��������к���
	MOV P3,#0F0H	//����0������1�����µİ���������
	MOV A,P3
	ANL A,#0F0H		
	CJNE A,#0F0H,ASKEY	 //��(A)!=0F0H��˵���а�������
	SJMP KEYSCAN		//�����ת��KEYSCAN�ȴ���������
ASKEY:
	LCALL DELAY10MS	   //������ʱ10ms�ӳ���
	MOV P3,#0F0H
	MOV A,P3
	ANL A,#0F0H
	CJNE A,#0F0H,ISKEY	 //�ظ������ж����ް������£������������µ���ֵ
	MOV 31H,#0			//��ż��뵥Ԫ��0
	MOV 30H,#0			//������尴��������0��ʾ�ް�������
	LJMP TORET1
ISKEY:
	MOV 31H,A	   //���������µ���ֵ���Ŵ浽31H��Ԫ��
SEARCHLINE0:		 //���а��µİ�����ֵ���Ų�ѯ
	MOV P3,#0FEH	 //�жϵ�0��
	MOV A,P3
	ANL A,#0F0H
	CLR C		  //��λ/��λ��־λ����
	SUBB A,#0F0H	
	JZ SEARCHLINE1	//��(A)!=0F0H��˵�����µİ����ڵ�0�з��������1��ɨ��
	MOV A,#0EH		//���µİ����ڵ�0�У���0EH�����ۼ���A
	LJMP TOSTORE	//��ת��TOSTRE������ֵȷ��
SEARCHLINE1:	   //ɨ���1��
	MOV P3,#0FDH
	MOV A,P3
	ANL A,#0F0H
	CLR C
	SUBB A,#0F0H
	JZ SEARCHLINE2	//��(A)!=0F0H��˵�����µİ����ڵ�1�з��������2��ɨ��
	MOV A,#0DH		//���µİ����ڵ�1�У���0DH�����ۼ���A
	LJMP TOSTORE   //��ת��TOSTRE������ֵȷ��
SEARCHLINE2:	   //ɨ���2��
	MOV P3,#0FBH
	MOV A,P3
	ANL A,#0F0H
	CLR C
	SUBB A,#0F0H
	JZ SEARCHLINE3	  //��(A)!=0F0H��˵�����µİ����ڵ�2�з��������3��ɨ��
	MOV A,#0BH		  //���µİ����ڵ�2�У���0BH�����ۼ���A
	LJMP TOSTORE	  //��ת��TOSTRE������ֵȷ��
SEARCHLINE3:		  //ɨ���3��
	MOV P3,#0F7H
	MOV A,P3
	ANL A,#0F0H
	CLR C
	SUBB A,#0F0H
	JZ WRONGKEY	   //��(A)!=0F0H��˵�����µİ����ڵ�3�з�����ת��WRONGKEY
	MOV A,#07H	    //���µİ����ڵ�3�У���07H�����ۼ���A
	LJMP TOSTORE	//��ת��TOSTRE������ֵȷ��
WRONGKEY:		   //����İ�����ֵ����
	MOV 31H,#0	   //����Ű�������ĵ�Ԫֵ��0
	MOV 30H,#0	   //��������ް������µı�־λ��0
	SJMP TORET1	   //��ת��TORET1���ӳ��򷵻�
TOSTORE:		   //������ֵȷ��
	ORL A,31H	   //��31H��Ԫ��ֵ���ۼ�����ֵ���л����㣬����ֵ����ֵ����λ��Ϊ0
	CPL A		  //ȡ���õ���ֵ����ֵ����λ��Ϊ1������λΪ0
	MOV 31H,A	  //�����µİ��������С��м�������31H
	MOV 30H,#1	  //30H��Ԫ����1��־�а�������
TORET1:
	RET			 //�ӳ��򷵻�

SEARCHKEYNUM: 		//����ȷ���ӳ��򣬸���31H��Ԫ�İ�������ֵȷ����TABLE1�еļ���
	MOV DPTR,#TABLE1
	MOV A,33H
	MOVC A,@A+DPTR	 //���Ȼ����31H��Ԫ�ڵ�ֵһһ�Ƚ�
	CLR C
	SUBB A,31H
	JZ ENDSEARCH   //����鵽�ı�ֵ��31H��Ԫֵ��ͬ��������
	INC 33H		   //��������1
	MOV A,33H
	CJNE A,#10H,SEARCHKEYNUM //��(A)!=10H������ת��SEARCHKEYNUM�����жϼ���
	MOV 32H,#1		 //����û���ҵ���ͬ�ļ��룬32H��Ԫ��ֵ��1˵���Ǵ��󰴼����簴˫��
	SJMP TORET2		 //��ת��TORET2����
ENDSEARCH:
	MOV 32H,#0	 //�������Ų�ѯ��32H��Ԫ��ֵ��0��ʾ�����޴���
TORET2:
	RET		  //�ӳ��򷵻�

DELAY10MS:		//��ʱ10ms�ӳ���		(48*2+1+1+2)*100+1~=10ms
	DL0:MOV R2,#100
	DL1:MOV R1,#48
	DL2:DJNZ R1,DL2		 //��ѭ��
		NOP 
		DJNZ R2,DL1		 //��ѭ��
		RET

TABLE1:DB 11H,21H,41H,81H,12H,22H,42H,82H,14H,24H,44H,84H,18H,28H,48H,88H  //���������
TABLE2:DB 3FH,06H,5BH,4FH,66H,6DH,7DH,07H,7FH,6FH,77H,7CH,39H,5EH,79H,71H  //�����������ʾ��0-F
END