void InitUART(void)
{
   	SCON = 0x50; 	// SCON: ģʽ1, 8-bit UART, ʹ�ܽ��� 
	TMOD |= 0x20;
	TH1=0xfd; 		//������9600 ��ֵ
	TL1=0xfd;
	TR1= 1;
	EA = 1;	    //�����ж�
	ES= 1; 		//�򿪴����ж�
}

void SendOneByte(uchar c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}
void UART_SER () interrupt 4
{
	uint n; 	//������ʱ����
	if(RI) 		//�ж��ǽ����жϲ���
	{
		RI=0; 	//��־λ����
		n=SBUF; //���뻺������ֵ
		switch(n)
		{
			case 1:			//��Ӧʱ������
						
					SendOneByte(1);
					break;	//����
			case 2:			//��Ӧ��������
					LED1 = 1;	
					SendOneByte(2);
					break;	//���
			case 3:			//��ӦAM/PM����
					
					
		}
	}
}