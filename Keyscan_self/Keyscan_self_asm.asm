;矩阵键盘扫描自我完成试验：根据点击不同按键完成彩色旋转灯要求

ORG 0000H
AJMP MAIN	;跳转到主程序
ORG 0030H

MAIN:	
	MOV SP,#60H	   ;栈指针初始化
	MOV 30H,#1	   ;30H存放旋转方向DIR
	MOV 31H,#0	   ;31H存放旋转速度Speed以及对应周期
	MOV 32H,#0FEH	;32H单元存放旋转彩色灯亮灭码
	MOV A,32H
	MOV P1,A	 ;将32H单元内值送P1口
	MOV 33H,#0	;存放有无按键按下的标志,若为0表示无按键按下，1表示有按键按下
	MOV 34H,#0	;存放按下的按键的键码
LOOP:
	MOV 33H,#0
	LCALL KEYSCAN	  ;调用键盘扫描子程序
	MOV A,33H		 
	JZ LOOP1	  ;判断有无按键按下，若(A)=0说明无按键按下跳转到LOOP1处否则调用运行参数调整子程序
	LCALL Changeref	  ;调用运行参数调整子程序
LOOP1:
	MOV P1,32H	  ;将旋转彩色灯亮灭控制显示码送入P1口
	MOV A,31H	  ;将旋转速度送入累加器A
	MOV DPTR,#T_TABLE
	MOVC A,@A+DPTR	;查表获得旋转周期
	MOV R1,A	 ;将旋转周期保存置寄存器R1中
	MOV A,31H	 ;将旋转速度再次送入累加器A
	MOV DPTR,#Sout_TABLE
	MOVC A,@A+DPTR	 ;查表获得档位输出码
	MOV P0,A	  ;将档位输出码送入P0口让对应档位发光二极管亮
	MOV A,30H	  ;读DIR值判断旋转方向
	JNZ LOOP3	  ;若(A)=1正转否则反转
LOOP2:			 ;循环右移显示码进行旋转彩色灯反转
	MOV A,32H
	RR A
	MOV 32H,A
	LJMP LOOP4
LOOP3:		   ;循环左移显示码进行旋转彩色灯正转
	MOV A,32H
	RL A
	MOV 32H,A
LOOP4:			;进行旋转彩色灯的显示
	MOV P1,A	;将显示码送入P1口
	MOV A,R1	;将旋转周期送入累加器A
	MOV B,#08H
	DIV AB		;将周期均分成8份，每一份表示一个LED灯显示的时间
	MOV R1,A
DELAY:		   ;进行每个灯的显示
	LCALL DELAY5MS
	DJNZ R1,DELAY
	LJMP LOOP

DELAY100MS:					   //(248*2+1+1+2)*200+1~=100MS
	DL_0:MOV R6,#200
	DL_1:MOV R5,#248
	DL_2:DJNZ R5,DL_2	  ;内循环
		NOP
		DJNZ R6,DL_1	  ;中循环
	RET
	
DELAY5MS:	   	;延时5ms子程序		(23*2+1+1+2)*100+1~=5ms
	DL0:MOV R7,#100
	DL1:MOV R6,#23
	DL2:DJNZ R6,DL2	  ;内循环
		NOP
		DJNZ R7,DL1	  ;外循环
	RET

KEYSCAN:		   ;键盘扫描子程序
	MOV P3,#3CH		;//行置0列置1，读按下的按键的列值
	MOV A,P3
	ANL A,#30H		;//保留有效位
	CJNE A,#30H,ASKEY	;//若(A)!=30H说明有按键按下则跳转到ASKEY处继续判断一次
	LJMP TORET	   ;//否则跳转到TORET处返回
ASKEY:			   ;//继续判断是否有按键按下
	LCALL DELAY100MS ;//延时15ms去抖动
	MOV P3,#3CH
	MOV A,P3
	ANL A,#30H
	CJNE A,#30H,ISKEY	;//重复上述判断，若(A)!=30H说明有按键按下则跳转到ISKEY处进行键码确定
	MOV 33H,#0	   ;//否则33H置0说明无按键按下
	MOV 34H,#0	   ;//34H单元存放键码置0
	LJMP TORET	   ;//跳转到TORET处返回
ISKEY:
	ORL A,#0C0H	   ;//将列的其他位置1
	MOV 34H,A	   ;//将列值保存到34H单元存放键码
SEARCHLINE0:	   ;//扫描键盘第0行
	MOV P3,#3EH
	MOV A,P3
	ANL A,#30H
	CLR C
	SUBB A,#30H	   
	JNZ SAVELINE0	;//若(A)!=30H说明按下的按键在第0行则跳转到SAVELINE0保存行值
SEARCHLINE1:		;//否则继续扫描第1行
	MOV P3,#3DH
	MOV A,P3
	ANL A,#30H
	CLR C
	SUBB A,#30H
	JNZ SAVELINE1	;//若(A)!=30H说明按下的按键在第1行则跳转到SAVELINE1保存行值
	MOV 33H,#0		;//否则为无效按键，33H单元置0
	MOV 34H,#0		//按键键码存放单元置0
	LJMP TORET		//跳转到TORET处返回
SAVELINE0:			//当按键在第0行时保存第0行键值
	MOV A,#0EH
	SJMP KEYNUM
SAVELINE1:		  //当按键在第1行时保存第1行键值
	MOV A,#0DH
KEYNUM:
	ORL A,34H	 //合并行列键值
	CPL A
	MOV 34H,A	//将按下的按键键值存放到34H单元	
	MOV 33H,#1	//33H单元置1说明有按键按下
TORET:RET		//子程序返回

Changeref:		//旋转灯参数调整子程序
	MOV A,34H	   //将按键键值送入累加器A
	CJNE A,#11H,KEYCODE1	 //若键值不为11H则继续下一个键值判断
	MOV 30H,#1	   //说明按键为正转则DIR=1
	LJMP TORET1
KEYCODE1:
	CJNE A,#21H,KEYCODE2   //若键值不为21H则继续下一个键值判断
	MOV 30H,#0		 //说明按键为反转则DIR=0
	LJMP TORET1
KEYCODE2:
	CJNE A,#12H,KEYCODE3  //若键值不为12H则继续下一个键值判断
	INC 31H		 //说明按键为加速则Speed自增1
	MOV A,31H
	CJNE A,#5,TORET1	//因为最高档为5挡则Speed最大值为4
	MOV 31H,#4
	LJMP TORET1
KEYCODE3:
	CJNE A,#22H,WRONGKEY  //若键值不为22H则为错误按键键值跳转到WRONGKEY
	MOV A,31H		  //说明按键为减速
	CJNE A,#0,NEXT1	   //因为最低档为1挡，所以Speed最小值为0
	INC 31H
NEXT1:
	DEC 31H
	LJMP TORET1
WRONGKEY:			 //说明为错误按键，则33H和34H单元均置0
	MOV 33H,#0
	MOV 34H,#0
TORET1:
	RET

T_TABLE:DB 0C8H,0A0H,78H,50H,28H ;//旋转周期控制表，分别对应1挡-5挡
KEY_TABLE:DB 22H,12H,21H,11H	 ;//按键键码表，分别对应正转、反转、加速、减速按键
Sout_TABLE:DB 0FH,17H,1BH,1DH,1EH 	;//档位表，分别是1挡-5挡
END
