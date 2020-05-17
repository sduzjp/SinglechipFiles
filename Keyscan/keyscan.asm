//矩阵键盘扫描实验：点击按键KEYi时，数码管显示对应字符i，i从0取到F
//汇编程序
ORG 0000H
AJMP MAIN //跳转到主程序
ORG 0030H

MAIN:				 //主程序
	MOV SP,#60H		 //栈指针设置初值
	MOV P1,#00H		 //P1口置0，数码管熄灭
LOOP:
	MOV 30H,#0		 //30H单元存放有无按键按下的标志，1表示按下，0表示未按下
	MOV 31H,#0		 //31H存放对应按键键码
	LCALL KEYSCAN	 //调用矩阵扫描子程序，确定按键按下的位置
	MOV A,30H		 
	JZ LOOP			 //判断按键是否按下，(A)=0表示按键未按下跳转到LOOP处，否则(A)=1按键按下，向下继续执行
	MOV 32H,#0		 //32H单元存放按键是否错误，1表示有错误，0表示无错误
	MOV 33H,#0		 //33H单元存放键号
	LCALL SEARCHKEYNUM	 //调用
	MOV A,32H		 
	JNZ LOOP		 //判断按键是否存放错误，(A)=1表示有错误返回到LOOP处否则向下执行
	MOV DPTR,#TABLE2
	MOV A,33H
	MOVC A,@A+DPTR	  //根据存放的键号查表得到对应的显示码
	MOV P1,A		  //将显示码送入P1，数码管显示
	LJMP LOOP		//跳转回LOOP循环

KEYSCAN:		  //键盘矩阵扫描子程序，根据扫描法确定按下的按键所在行和列
	MOV P3,#0F0H	//行置0，列置1读按下的按键所在列
	MOV A,P3
	ANL A,#0F0H		
	CJNE A,#0F0H,ASKEY	 //若(A)!=0F0H，说明有按键按下
	SJMP KEYSCAN		//否则调转回KEYSCAN等待按键按下
ASKEY:
	LCALL DELAY10MS	   //调用延时10ms子程序
	MOV P3,#0F0H
	MOV A,P3
	ANL A,#0F0H
	CJNE A,#0F0H,ISKEY	 //重复上述判断有无按键按下，读到按键按下的列值
	MOV 31H,#0			//存放键码单元置0
	MOV 30H,#0			//存放有五按键按下置0表示无按键按下
	LJMP TORET1
ISKEY:
	MOV 31H,A	   //将按键按下的列值键号存到31H单元处
SEARCHLINE0:		 //进行按下的按键行值键号查询
	MOV P3,#0FEH	 //判断第0行
	MOV A,P3
	ANL A,#0F0H
	CLR C		  //进位/借位标志位清零
	SUBB A,#0F0H	
	JZ SEARCHLINE1	//若(A)!=0F0H，说明按下的按键在第0行否则继续第1行扫描
	MOV A,#0EH		//按下的按键在第0行，将0EH送入累加器A
	LJMP TOSTORE	//跳转到TOSTRE进行行值确定
SEARCHLINE1:	   //扫描第1行
	MOV P3,#0FDH
	MOV A,P3
	ANL A,#0F0H
	CLR C
	SUBB A,#0F0H
	JZ SEARCHLINE2	//若(A)!=0F0H，说明按下的按键在第1行否则继续第2行扫描
	MOV A,#0DH		//按下的按键在第1行，将0DH送入累加器A
	LJMP TOSTORE   //跳转到TOSTRE进行行值确定
SEARCHLINE2:	   //扫描第2行
	MOV P3,#0FBH
	MOV A,P3
	ANL A,#0F0H
	CLR C
	SUBB A,#0F0H
	JZ SEARCHLINE3	  //若(A)!=0F0H，说明按下的按键在第2行否则继续第3行扫描
	MOV A,#0BH		  //按下的按键在第2行，将0BH送入累加器A
	LJMP TOSTORE	  //跳转到TOSTRE进行行值确定
SEARCHLINE3:		  //扫描第3行
	MOV P3,#0F7H
	MOV A,P3
	ANL A,#0F0H
	CLR C
	SUBB A,#0F0H
	JZ WRONGKEY	   //若(A)!=0F0H，说明按下的按键在第3行否则跳转到WRONGKEY
	MOV A,#07H	    //按下的按键在第3行，将07H送入累加器A
	LJMP TOSTORE	//跳转到TOSTRE进行行值确定
WRONGKEY:		   //存入的按键键值错误
	MOV 31H,#0	   //将存放按键键码的单元值置0
	MOV 30H,#0	   //将存放有无按键按下的标志位置0
	SJMP TORET1	   //跳转到TORET1处子程序返回
TOSTORE:		   //进行行值确定
	ORL A,31H	   //将31H单元的值与累加器中值进行或运算，则行值与列值所在位均为0
	CPL A		  //取反得到行值与列值所在位均为1，其他位为0
	MOV 31H,A	  //将按下的按键所在行、列键码送入31H
	MOV 30H,#1	  //30H单元送入1标志有按键按下
TORET1:
	RET			 //子程序返回

SEARCHKEYNUM: 		//键号确定子程序，根据31H单元的按键键码值确定在TABLE1中的键号
	MOV DPTR,#TABLE1
	MOV A,33H
	MOVC A,@A+DPTR	 //查表，然后与31H单元内的值一一比较
	CLR C
	SUBB A,31H
	JZ ENDSEARCH   //如果查到的表值与31H单元值相同则结束查表
	INC 33H		   //键号自增1
	MOV A,33H
	CJNE A,#10H,SEARCHKEYNUM //若(A)!=10H，则跳转到SEARCHKEYNUM继续判断键号
	MOV 32H,#1		 //否则没有找到相同的键码，32H单元内值置1说明是错误按键，如按双键
	SJMP TORET2		 //跳转到TORET2返回
ENDSEARCH:
	MOV 32H,#0	 //结束键号查询，32H单元内值置0表示按键无错误
TORET2:
	RET		  //子程序返回

DELAY10MS:		//延时10ms子程序		(48*2+1+1+2)*100+1~=10ms
	DL0:MOV R2,#100
	DL1:MOV R1,#48
	DL2:DJNZ R1,DL2		 //内循环
		NOP 
		DJNZ R2,DL1		 //外循环
		RET

TABLE1:DB 11H,21H,41H,81H,12H,22H,42H,82H,14H,24H,44H,84H,18H,28H,48H,88H  //按键键码表
TABLE2:DB 3FH,06H,5BH,4FH,66H,6DH,7DH,07H,7FH,6FH,77H,7CH,39H,5EH,79H,71H  //共阴数码管显示表0-F
END