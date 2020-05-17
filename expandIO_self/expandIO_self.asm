//扩展并行I/O口实验：仿真实现交通信号灯控制功能，控制顺序为
//(1)南北绿灯亮，同时东西红灯亮10s
//(2)南北黄灯亮，同时东西红灯亮2s
//(3)南北红灯亮，同时东西绿灯亮10s
//(4)南北黄灯亮，同时东西绿灯亮2s
//(5)重复(1)--(4)

ORG 0000H
AJMP MAIN	 //跳转到主程序
ORG 0030H

MAIN:		//主程序
	MOV SP,#60H	//栈指针初始化
	MOV A,#0FFH	//将两个373输出口所有位置1
	MOV DPTR,#0FE00H
	MOVX @DPTR,A	//U4输出口全置1，U4来设置红灯和黄灯的亮灭	
	MOV DPTR,#0FD00H
	MOVX @DPTR,A   //U5输出口全置1，U5的低四位来设置绿灯的亮灭	
STAT1:		//状态1,南北绿灯亮，东西红灯亮10s
	MOV A,#0F3H
	MOV DPTR,#0FE00H
	MOVX @DPTR,A  //U4设置南北绿灯亮
	MOV A,#0CH
	MOV DPTR,#0FD00H
	MOVX @DPTR,A  //U5设置东西红灯亮
	LCALL DELAY10S
STAT2:			//状态2：南北黄灯亮，东西红灯亮2s
	MOV A,#0C3H
	MOV DPTR,#0FE00H
	MOVX @DPTR,A	//设置南北黄灯亮，东西红灯亮
	MOV A,#0FH
	MOV DPTR,#0FD00H
	MOVX @DPTR,A	//设置绿灯全灭
	LCALL DELAY2S
STAT3:	   			//状态3：南北红灯亮，东西绿灯亮10s
	MOV A,#0FCH
	MOV DPTR,#0FE00H
	MOVX @DPTR,A	 //设置南北红灯亮
	MOV A,#03H
	MOV DPTR,#0FD00H
	MOVX @DPTR,A	//设置东西绿灯亮
	LCALL DELAY10S
STAT4:				//状态4：南北红灯亮，东西黄灯亮2s
	MOV A,#3CH
	MOV DPTR,#0FE00H
	MOVX @DPTR,A	//设置南北红灯亮，东西黄灯亮
	MOV A,#0FH
	MOV DPTR,#0FD00H
	MOVX @DPTR,A   //设置绿灯全灭
	LCALL DELAY2S
	LJMP STAT1		//重复上述状态循环

DELAY2S:				//2s延时子程序,((248*2+1+1+2)*200+1+2)*20+1~=2s
	DL1_0:MOV R7,#20
	DL1_1:MOV R6,#200
	DL1_2:MOV R5,#248
	DL1_3:DJNZ R5,DL1_3	 //内循环
		NOP
		DJNZ R6,DL1_2	 //中循环
		DJNZ R7,DL1_1 	 //外循环
	RET

DELAY10S:				//10s延时子程序,((248*2+1+1+2)*200+1+2)*100+1~=10s
	DL0:MOV R7,#100
	DL1:MOV R6,#200
	DL2:MOV R5,#248
	DL3:DJNZ R5,DL3	 //内循环
		NOP
		DJNZ R6,DL2	 //中循环
		DJNZ R7,DL1 	 //外循环
	RET
END



	