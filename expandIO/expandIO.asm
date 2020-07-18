//扩展并行I/O口实验：通过开关SW1-SW4的开关状态切换控制D1-D4发光二极管的亮灭
ORG 0000H
AJMP MAIN	 //跳转到主程序
ORG 0030H

MAIN:		 //主程序
	MOV SP,#60H		 //栈指针初始化
	MOV DPTR,#0FE00H  //DPTR初始化
LOOP:
	MOVX A,@DPTR	  //将扩展I/O口的数据送入累加器A
	MOVX @DPTR,A	  //将累加器A中的数据送入扩展I/O口输出
	CLR P2^1
	LCALL DELAY1S
	SETB P2^1
	LCALL DELAY1S
	SJMP LOOP		  //执行上述循环
DELAY1S:				//1s延时子程序,((248*2+1+1+2)*200+1+2)*10+1=1s
	DL1_0:MOV R7,#10
	DL1_1:MOV R6,#200
	DL1_2:MOV R5,#248
	DL1_3:DJNZ R5,DL1_3	 //内循环
		NOP
		DJNZ R6,DL1_2	 //中循环
		DJNZ R7,DL1_1 	 //外循环
	RET
END
