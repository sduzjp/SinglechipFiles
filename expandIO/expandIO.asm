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
	SJMP LOOP		  //执行上述循环
END