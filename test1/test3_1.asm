//�����ܣ�ʵ�ֽ�R0��Ԫ�ڵ���λBCD��𿪲�ת����ASCII�룬����RAM������Ԫ��R2���λ
//R1���λ
ORG 0000H
AJMP MAIN
ORG 0030H

MAIN:
	MOV A,R0
	ANL A,#0FH
	ADD A,#30H
	MOV R1,A
	MOV A,R0
	SWAP A
	ANL A,#0FH
	ADD A,#30H
	MOV R2,A
	SJMP $
END
