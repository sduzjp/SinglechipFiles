//基本并行I/O口实验：通过调节DIP开关的DSW1中的各开关键观察D1-D8发光二极管的亮灭情况
//发光二极管共阳极，将P1口的数据写入P2口，P1口哪一位低电平对应P2口的哪一位二极管亮
//高电平则二极管灭。
ORG 0000H
AJMP MAIN	 //跳转到主程序
ORG 0030H

MAIN:			//主程序
	MOV SP,#60H	 //设置栈指针初值
	//	MOV P2,#0FFH ;将P2口全部置高电平，复位时P2口自动置高电平
LOOP:
	MOV P1,#0FFH  //初始时P1口置高电平，二极管全部灭
	MOV A,P1	  //读P1口数据
	MOV P2,A	  //向P2口写入数据
	SJMP LOOP
END