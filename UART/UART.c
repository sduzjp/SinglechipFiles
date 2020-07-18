#include "reg51.h"
#define uchar unsigned char
#define uint unsigned int

uchar xdata D244 _at_ 0xfe00;
uchar rdata,i,j,temp;
uint x;

void delay()
{
	for(i=200;i>0;i--);
}

void longdelay()
{
	for(x=500;x>0;x--)
	for(i=200;i>0;i--);
}

main()
{
	TMOD=0x20;
	SM0=0;
	SM1=1;
	REN=1;
	PCON=0;
	TH1=0XFD;
	TL1=0XFD;
	TR1=1;
	rdata=D244;
	delay();
	P1=rdata;
	while(1)
	{
		while(!RI);
		temp=SBUF;
		RI=0;
		rdata=D244;
		delay();
		switch(temp)
		{
			case 1:
				for(j=10;j>0;j--)
				{
					P1=0x88;
					longdelay();
					P1=rdata;
					longdelay();
				}
				break;
			case 2:
				P1=rdata;
				SBUF=rdata;
				while(!TI);
				TI=0;
				break;
		}
	}
}