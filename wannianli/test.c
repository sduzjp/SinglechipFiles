void InitUART(void)
{
   	SCON = 0x50; 	// SCON: 模式1, 8-bit UART, 使能接收 
	TMOD |= 0x20;
	TH1=0xfd; 		//波特率9600 初值
	TL1=0xfd;
	TR1= 1;
	EA = 1;	    //开总中断
	ES= 1; 		//打开串口中断
}

void SendOneByte(uchar c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}
void UART_SER () interrupt 4
{
	uint n; 	//定义临时变量
	if(RI) 		//判断是接收中断产生
	{
		RI=0; 	//标志位清零
		n=SBUF; //读入缓冲区的值
		switch(n)
		{
			case 1:			//对应时间设置
						
					SendOneByte(1);
					break;	//亮灯
			case 2:			//对应闹钟设置
					LED1 = 1;	
					SendOneByte(2);
					break;	//灭灯
			case 3:			//对应AM/PM设置
					
					
		}
	}
}