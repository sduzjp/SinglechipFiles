//程序功能：将随机构建的表格中的32个随机数进行冒泡排序并存放
ORG 0000H  //伪指令，汇编程序开始处
AJMP MAIN  //跳转到主程序
ORG 0060H  //定义汇编指令开始处

MAIN:			  //先将表格中的数存放到30H-4FH单元处
	MOV R7,#20H	  //R7存放循环次数
	MOV R0,#30H	  //R0指向30H单元
	MOV A,#0	  //将累加器A置0
	MOV DPTR,#TABLE //DPTR指向TABLE表格首地址
 LOOP:
	MOVC A,@A+DPTR  //取得表格中的数
	MOV @R0,A  //将数存放到首地址为30H的单元处
	INC R0	   //R0自增1
	CLR A	   //将累加器A清0
	INC DPTR	   //DPTR自增1
	DJNZ R7,LOOP //判断R7是否为0从而判断循环是否结束

//进行冒泡排序将30H-4FH中的32个随机数从小到大排序
	MOV R0,#30H	//R0指向30H单元处
	MOV R7,#31	//R7存放排序次数
 LOOP1:
 	MOV R1,#30H  //R1指向30H单元处
	MOV R6,#31  //R6复制存放排序次数
	MOV R0,#30H	//R0恢复指向30H单元处

 LOOP2:		   //两两进行判断，大的存放到高地址处，小的存放在低地址处
 	MOV A,@R0  //寄存器间址获取R0单元内数
	MOV B,A	   //将该数保存到B中
	INC R0	   //R0自增1
	MOV A,@R0  //获取下一单元的数
	CJNE A,B,L1	 //判断两数大小，A、B中数不相等就跳转到L1处
 L1:JNC UNEX //A中数大于B中数就跳转到UNEX处。因为这里实现(A)>=(B)就不交换，否则交换
 	DEC R0 //R0指向低地址
	XCH A,@R0 //高地址单元和低地址单元中数交换，小的数保存到低地址单元中
	INC R0
	MOV @R0,A //大数保存在高地址单元中
 UNEX:
 	DJNZ R6,LOOP2 //R6自减1，当R6为0时完成1轮比较
	DJNZ R7,LOOP1 //R7自减1，当R7为0时完成31轮比较

	SJMP $ //等待中断信号的到来

//TABLE存放32个随机数
TABLE:
	db 1,3,9,2,17,4,11,6
	db 5,20,100,64,21,14,79,35
	db 92,7,91,23,65,16,13,18
	db 18,73,65,101,27,19,62,69
END	