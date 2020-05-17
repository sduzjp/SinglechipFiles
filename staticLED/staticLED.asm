//静态LED显示实验：实现一个秒表计时器，四段数码管的计时单位分别为10s，1s，0.1s，0.01s
//起停按键用于启动和暂停秒表，清零按键将秒表清零，发光二极管指示秒表的状态
ORG 0000H
AJMP MAIN  //跳转到主程序
ORG 0030H

MAIN:
	MOV SP,#60H	 //栈指针初始化
	CLR F0	  //用户标志位F0清零
	SETB P1.2  //P1.2置1，发光二极管灭
	LCALL DISCLR  //调用显示清零子程序，清零所有数码管
LOOP:
	SETB P1.1	 //P1.1置1
	JB P1.1,GOON   //若P1.1=1，CLEAR按键松开，跳转到GOON
WAITO:			//否则P1.1=0，CLEAR按键按下，向下执行
	SETB P1.1	//P1.1置1
	JNB P1.1,WAITO	//P1.1=0,跳转到WAITO,等待CLEAR按键释放
	LCALL DISCLR   //调用显示清零子程序
	CLR F0	 //F0清零
GOON:
	SETB P1.0	//P1.0置1
	JB P1.0,NEXT1	//若P1.0=1，Start/Stop键松开，跳转到NEXT1
WAIT1:				//否则P1.0=0，Start/Stop键按下，向下执行
	SETB P1.0	   //P1.0置1
	JNB P1.0,WAIT1	//若P1.0=0，跳转到WAIT1，等待Start/Stop按键释放
	CPL F0		 //F0取反
NEXT1:
	JNB F0,NEXT	  //若F0=0，跳转到NEXT
	CLR P1.2	  //否则F0=1,P1.2清零，执行秒表计时功能，且发光二极管亮
	LCALL DISPLAY	 //调用显示子程序
	LCALL DELAY10ms	 //调用延时10ms子程序
	LCALL ADJUST	 //调用秒表显示调整子程序
	SJMP LOOP		 //跳转到LOOP处循环
NEXT:
	SETB P1.2		 //P1.2置1
	SJMP LOOP		 //跳转到LOOP处循环

DELAY10ms:		//10ms延时子程序，(48*2+1+1+2)*100+1~=10ms
	DL0:MOV R2,#100
	DL1:MOV R1,#48
	DL2:DJNZ R1,DL2	  //内循环100us
		NOP
		DJNZ R2,DL1	  //中循环10ms
		RET			  //子程序返回

ADJUST:			   //秒表计数器调整子程序，秒表显示数字需为0-9
	INC 30H		   //30H单元内值自增1
	MOV A,30H	   //将30H单元内值送入累加器A
	CJNE A,#10,GOTORET	//若A中值不等于10，则跳转到GOTORET处
	MOV 30H,#0	   //若(A)=10,则30H单元内值置0
	INC 31H		   //31H单元内值自增1
	MOV A,31H	   //将31H单元内值送入累加器A
	CJNE A,#10,GOTORET //若A中值不等于10，则跳转到GOTORET处
	MOV 31H,#0	   //若(A)=10,则31H单元内值置0
	INC 32H		   //32H单元内值自增1
	MOV A,32H	   //将32H单元内值送入累加器A
	CJNE A,#10,GOTORET	//若A中值不等于10，则跳转到GOTORET处
	MOV 32H,#0	   //若(A)=10,则32H单元内值置0
	INC 33H		  //33H单元内值自增1
	MOV A,33H	  //将33H单元内值送入累加器A
	CJNE A,#10,GOTORET	//若A中值不等于10，则跳转到GOTORET处
	MOV 33H,#0	  //若(A)=10,则33H单元内值置0
GOTORET:RET		 //子程序返回

DISCLR:			//显示清零子程序
	MOV 30H,#0	 //四个数码管显示清零
	MOV 31H,#0
	MOV 32H,#0
	MOV 33H,#0
	CLR A	  //累加器A清零
	MOV DPTR,#TABLE	  
	MOVC A,@A+DPTR	  //查表
	MOV DPTR,#D10msADD	 //权值为10ms的数值单元地址送DPTR
	MOVX @DPTR,A		 //清零
	MOV DPTR,#D100msADD	 //权值为100ms的数值单元地址送DPTR
	MOVX @DPTR,A		 //清零
	MOV DPTR,#D1sADD	 //权值为1s的数值单元地址送DPTR
	MOVX @DPTR,A		 //清零
	MOV DPTR,#D10sADD	 //权值为10s的数值单元地址送DPTR
	MOVX @DPTR,A
	SETB P1.2  //P1.2置1
	RET		   //子程序返回

DISPLAY:	   //显示子程序
	MOV A,30H		//30H单元的值送累加器A
	MOV DPTR,#TABLE
	MOVC A,@A+DPTR	//字段显示码查表 
	MOV DPTR,#D10msADD //权值为10ms的数值单元地址送DPTR
	MOVX @DPTR,A	//显示x10ms数码管的数值 
	MOV A,31H		//31H单元的值送累加器A
	MOV DPTR,#TABLE
	MOVC A,@A+DPTR	//字段显示码查表 
	MOV DPTR,#D100msADD	 //权值为100ms的数值单元地址送DPTR
	MOVX @DPTR,A	//显示x100ms数码管的数值
	MOV A,32H		//32H单元的值送累加器A
	MOV DPTR,#TABLE
	MOVC A,@A+DPTR	//字段显示码查表 
	MOV DPTR,#D1sADD //权值为1s的数值单元地址送DPTR
	MOVX @DPTR,A	//显示x1s数码管的数值
	MOV A,33H		//33H单元的值送累加器A
	MOV DPTR,#TABLE
	MOVC A,@A+DPTR	//字段显示码查表
	MOV DPTR,#D10sADD //权值为10s的数值单元地址送DPTR
	MOVX @DPTR,A	 //显示x10s数码管的数值
	RET				//子程序返回

TABLE:
	DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H	//7段LED共阳显示码
	D10msADD EQU 0FE00H	  //x0.01s显示锁存地址
	D100msADD EQU 0FD00H  //x0.1s显示锁存地址
	D1sADD EQU 0FB00H	  //x1s显示锁存地址
	D10sADD EQU 0F700H	  //x10s显示锁存地址
END

