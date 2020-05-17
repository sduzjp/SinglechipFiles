//静态LED显示自我完成实验：当KEY_LOAD按键按下时加载倒计时初值(10s),当按下KEY_START
//按键时，开始倒计时，每过1s计时器减1，直到减到00为止。当减到00时使P3.0引脚上的LED
//按10HZ频率进行闪烁，直到再次按下KEY_LOAD按键才重新加载初值，并熄灭LED。再次按下
//KEY_START按键又一次开始倒计时，如此反复。

ORG 0000H
AJMP MAIN	 //跳转到主程序
ORG 0030H

MAIN:		//主程序
	MOV SP,#60H		//栈指针初始化
	MOV R0,#0		//设置初值寄存器R0，记录个位
	MOV R1,#1		//设置初值寄存器R1，记录十位
	SETB P3.0		//P3.0置1，发光二极管灭
	CLR F0			//用户标志位F0清零，F0当做计时开始/结束标志
LOOP:
	JB P1.1,LOOP2	//若P1.1=1，跳转到LOOP2处判断P1.0是否为1
LOOP1:				//否则P1.1=0，即KEY_LOAD按键按下则装载初值，刷新显示，并关闭发光二极管
	CLR F0			//F0清零，计时开始
	MOV 30H,R0		//30H单元存放个位初值
	MOV 31H,R1		//31H单元存放十位初值
	SETB P3.0		//P3.0置1，熄灭发光二极管
	LCALL DISPLAY	//调用显示子程序刷新显示
LOOP2:
	JB P1.0,LOOP	//若P1.0=1，则跳转回LOOP处继续等待按键按下
LOOP3:				//否则P1.0=0，即KEY_START按键按下，则向下执行
	LCALL DISPLAY	//调用显示子程序刷新显示
	LCALL DELAY1S	//调用延时1s子程序
	LCALL ADJUST	//调用调整显示子程序，倒计时显示
	JB F0,LOOP4		//若F0即计时寄存位=1，说明倒计时结束则跳转到LOOP4处实现发光二极管闪烁
	LJMP LOOP3		//否则F0=0跳转回LOOP3处继续倒计时显示
LOOP4:				//实现P3.0闪烁，频率为10HZ，即100ms
	CLR P3.0		//P3.0清零，发光二极管亮
	LCALL DELAY1S  //调用延时100ms子程序看不到闪烁，改用延时1s子程序
	SETB P3.0		  //P3.0置1，熄灭发光二极管
	LCALL DELAY1S	//调用延时100ms子程序看不到闪烁，改用延时1s子程序
	JB P1.1,LOOP4	  //若P1.1=1，则跳转到LOOP4处继续循环
	LJMP LOOP1		  //否则P1.1=0则跳转回LOOP1处

DISPLAY:			 //显示子程序
	MOV A,30H		 //将个位值送入累加器A
	MOV DPTR,#TABLE
	MOVC A,@A+DPTR	 //查表，查个位值对应的显示码
	MOV DPTR,#D1ADD	  //将个位显示数码管地址送入DPTR
	MOVX @DPTR,A	 //将显示码送入数码管输出
	MOV A,31H		  //将十位值送入累加器A
	MOV DPTR,#TABLE
	MOVC A,@A+DPTR	  //查表，查十位值对应的显示码
	MOV DPTR,#D10ADD   //将十位显示数码管地址送入DPTR
	MOVX @DPTR,A	   //将显示码送入数码管输出
	RET			 //子程序返回

ADJUST:			//数码管显示调整子程序，倒计时显示且显示数字为0-9
	DEC 30H		//将个位值自减1  
	MOV A,30H	//将个位值送入累加器A
	CJNE A,#-1,GOTORET	 //如果(A)!=-1，则跳转到GOTORET返回
	MOV 30H,#9	  //否则说明开始时个位值为0，下一次显示应为9，因此将个位值置9
	DEC 31H		//将十位值自减1
	MOV A,31H	//将十位值送入累加器A
	CJNE A,#-1,GOTORET	//如果(A)!=-1，说明开始时值不为0则跳转到GOTORET返回
	SETB F0		 //否则说明已经减到00，倒计时结束，F0即计时寄存位置1
	RET
GOTORET:RET

DELAY1S:				//1s延时子程序,((248*2+1+1+2)*200+1+2)*10+1~=1s
	DL1_0:MOV R7,#10
	DL1_1:MOV R6,#200
	DL1_2:MOV R5,#248
	DL1_3:DJNZ R5,DL1_3	 //内循环
		NOP
		DJNZ R6,DL1_2	 //中循环
		DJNZ R7,DL1_1 	 //外循环
	RET

DELAY100MS:				//100ms延时子程序,(248*2+1+1+2)*200+1~=100ms
	DL1:MOV R6,#200
	DL2:MOV R5,#248
	DL3:DJNZ R5,DL3	 //内循环
		NOP
		DJNZ R6,DL1	 //外循环
	RET

DELAY10MS:				//10ms延时子程序,(48*2+1+1+2)*100+1~=100ms
	DL_1:MOV R6,#100
	DL_2:MOV R5,#48
	DL_3:DJNZ R5,DL_3	 //内循环
		NOP
		DJNZ R6,DL_1	 //外循环
	RET

TABLE:
	DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H	 //7段共阳显示码
	D1ADD EQU 0FE00H	//U2的锁存地址
	D10ADD EQU 0FD00H	//U3的锁存地址
END