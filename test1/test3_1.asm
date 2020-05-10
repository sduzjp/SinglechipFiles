//程序功能：实现将R0单元内的两位BCD码拆开并转换成ASCII码，存入RAM两个单元中R2存高位
//R1存低位
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
