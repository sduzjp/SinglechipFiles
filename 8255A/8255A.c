#include "reg51.h"
#include "absacc.h"	
#define uchar unsigned char
#define uint unsigned int
#define PA    XBYTE[0x7cff]     /*PA口地址*/
#define PB    XBYTE[0x7dff]     /*PB口地址*/
#define PC    XBYTE[0x7eff]     /*PC口地址*/
#define CON   XBYTE[0x7fff]     /*控制字地址*/

unsigned char code led[]={0x00,0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
unsigned char code num[]={0x00,0xf3, 0x49, 0x61, 0x33,0x25, 0x05, 0xf1, 0x01};
 
unsigned char temp;

void monitar_key()
{
	temp = PB;
	switch(temp)
	{
		case 0xfe:{
			PC = num[1];
			PA = led[1];	
		}break;
		case 0xfd:{
			PC = num[2];
			PA = led[2];	
		}break;
		case 0xfb:{
			PC = num[3];
			PA = led[3];	
		}break;
		case 0xf7:{
			PC = num[4];
			PA = led[4];	
		}break;
		case 0xef:{
			PC = num[5];
			PA = led[5];	
		}break;
		case 0xdf:{
			PC = num[6];
			PA = led[6];	
		}break;
		case 0xbf:{
			PC = num[7];
			PA = led[7];	
		}break;
		case 0x7f:{
			PC = num[8];
			PA = led[8];	
		}break;
	}		
}

void main()					   
{
	CON = 0x82;
	PA=0XFF;
	PC=0XFF;
	while(1)
	{	
		monitar_key();	
	}
}