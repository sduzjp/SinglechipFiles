//基本并行I/O口自我完成实验：当按键SW1按下时，D1-D8轮流点亮，点亮时间为100ms；
//当按键停下后，停止轮换，再次按下后继续轮换
ORG 0000H
AJMP MAIN
ORG 0030H

MAIN:			//主程序
	MOV SP,#60H	//栈指针初始化
	MOV P2,#0FFH ;P2口所有位置高电平，复位时P2口所有位自动为高电平
	MOV A,#0FEH
	MOV 20H,A	//将显示缓冲单元20H初始化为FEH
LOOP:
	JNB P1.0,LOOP1	 //若P1.0=0，则跳转到LOOP1执行循环显示
	SJMP LOOP		 //否则继续等待
LOOP1:
	MOV A,20H		 //将显示缓冲区的数据送P2口
	MOV P2,A
	LCALL DELAY100MS  //调用100ms延时子程序
	//LCALL DELAY10MS  ;调用10ms延时子程序
	//LCALL DELAY1S  ;调用1s延时子程序
	RL A			  //将显示缓冲区的数据循环左移后再送回显示缓冲区
	MOV 20H,A
	JB P1.0,LOOP	  //若P1.0=1即按键松开后则跳转到LOOP处重复上述操作
	LJMP LOOP1		  //P1.0=0即按键一直按下则执行LOOP1循环

DELAY10MS:			 	//10ms延时子程序,(48*2+1+1+2)*100+1=10ms
	DL0:MOV R2,#100
	DL1:MOV R1,#48
	DL2:DJNZ R1,DL2	   //内循环
		NOP
		DJNZ R2,DL1	   //外循环
	RET
DELAY100MS:	 			//100ms延时子程序,(248*2+1+1+2)*200+1=100ms
	DL_0:MOV R2,#200
	DL_1:MOV R1,#248
	DL_2:DJNZ R1,DL_2	   //内循环
		NOP
		DJNZ R2,DL_1	   //外循环
	RET				   //子程序返回
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
