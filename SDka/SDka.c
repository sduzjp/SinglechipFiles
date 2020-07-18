#include "reg51.h"
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

#define F_OSC  11059200//����Ƶ��Hz
#define F_BAUD 9600
#define RELOAD 256-F_OSC/12/32/F_BAUD
#define CR 0x0D        //�س�

//����SD����Ҫ��4���ź���
sbit SD_CLK = P1^4;
sbit SD_DI  = P1^6;
sbit SD_DO  = P1^5;
sbit SD_CS  = P1^7;

uchar xdata DATA[512];	  //����512�ֽڻ�������ע����Ҫʹ�� xdata�ؼ���
/*
//��ʱ�Ӻ���
void delay_ms(uint x)
{
	int i,j;
	for(i=0;i<x;i++)
	for(j=0;j<260;j++);
}  */

//дһ�ֽڵ�SD��,ģ��SPI���߷�ʽ
void SdWrite(uchar n)
{
    uchar i;
    for(i=8;i;i--)
    {
        SD_CLK=0;
        SD_DI=(n&0x80);
        n<<=1;
        SD_CLK=1;
        }
        SD_DI=1;
    }

//��SD����һ�ֽ�,ģ��SPI���߷�ʽ
uchar SdRead()
{
    uchar n,i;
    for(i=8;i;i--)
    {
        SD_CLK=0;
        SD_CLK=1;
        n<<=1;
        if(SD_DO) n|=1;
    }
    return n;
}

//���SD������Ӧ
uchar SdResponse()
{
    uchar i=0,response;

    while(i<=8)
    {
        response = SdRead();
        if(response==0x00)break;
        if(response==0x01)break;
        i++;
    }
    return response;
}

//�����SD��
void SdCommand(uchar command,ulong argument, uchar CRC)
{

    SdWrite(command|0x40);
    SdWrite(((uchar *)&argument)[0]);
    SdWrite(((uchar *)&argument)[1]);
    SdWrite(((uchar *)&argument)[2]);
    SdWrite(((uchar *)&argument)[3]);
    SdWrite(CRC);
}

//��ʼ��SD��
uchar SdInit(void)
{
    int delay=0, trials=0;
    uchar i;
    uchar response=0x01;

    SD_CS=1;
    for(i=0;i<=9;i++)
    SdWrite(0xff);
    SD_CS=0;

    //Send Command 0 to put MMC in SPI mode
    SdCommand(0x00,0,0x95);

    response=SdResponse();
    if(response!=0x01)
    {
        return 0;
    }

    while(response==0x01)
    {
        SD_CS=1;
        SdWrite(0xff);
        SD_CS=0;
        SdCommand(0x01,0x00ffc000,0xff);
        response=SdResponse();
    }

    SD_CS=1;
    SdWrite(0xff);
    return 1;
}

//��SD��ָ����ַд����,һ�����512�ֽ�
uchar SdWriteBlock(uchar *Block,ulong address,int len)
{
    uint count;
    uchar dataResp;
    //Block size is 512 bytes exactly
    //First Lower SS

    SD_CS=0;
    //Then send write command
    SdCommand(0x18,address,0xff);

    if(SdResponse()==00)
    {
        SdWrite(0xff);
        SdWrite(0xff);
        SdWrite(0xff);
        //command was a success - now send data
        //start with DATA TOKEN = 0xFE
        SdWrite(0xfe);
        //now send data
        for(count=0;count<len;count++) SdWrite(*Block++);

        for(;count<512;count++) SdWrite(0);
        //data block sent - now send checksum
        SdWrite(0xff); //���ֽ�CRCУ��, Ϊ0XFFFF ��ʾ������CRC
        SdWrite(0xff);
        //Now read in the DATA RESPONSE token
        dataResp=SdRead();
        //Following the DATA RESPONSE token
        //are a number of BUSY bytes
        //a zero byte indicates the MMC is busy

        while(SdRead()==0);

        dataResp=dataResp&0x0f; //mask the high byte of the DATA RESPONSE token
        SD_CS=1;
        SdWrite(0xff);
        if(dataResp==0x0b)
        {
        //printf("DATA WAS NOT ACCEPTED BY CARD -- CRC ERROR\n");
        return 0;
        }
        if(dataResp==0x05)
        return 1;

        //printf("Invalid data Response token.\n");
        return 0;
    }
    //printf("Command 0x18 (Write) was not received by the MMC.\n");
    return 0;
}

//��SD��ָ����ַ��ȡ����,һ�����512�ֽ�
uchar SdReadBlock(uchar *Block,ulong address,int len)
{
    uint count;
    //Block size is 512 bytes exactly
    //First Lower SS

     //printf("MMC_read_block\n");

    SD_CS=0;
    //Then send write command
    SdCommand(0x11,address,0xff);

    if(SdResponse()==00)
    {
        //command was a success - now send data
        //start with DATA TOKEN = 0xFE
        while(SdRead()!=0xfe);

        for(count=0;count<len;count++) *Block++=SdRead();

        for(;count<512;count++) SdRead();

        //data block sent - now send checksum
        SdRead();
        SdRead();
        //Now read in the DATA RESPONSE token
        SD_CS=1;
        SdRead();
        return 1;
    }
//printf("Command 0x11 (Read) was not received by the MMC.\n");
    return 0;
}

/*******************************************
         ���ڳ�ʼ��
*******************************************/
void UART()
{
    SCON=0x40;//�����뷽ʽ1���������
    TMOD=0x20;//��ʱ��1�����뷽ʽ2�Զ���װģʽ
    TH1=RELOAD;
    TR1=1;
    TI=0;   
}

//ͨ�����ڷ���һ���ַ���
void Sen_String(uchar *string)
{
    while(*string!='\0')
    {
        if(*string=='\n')
        {
            SBUF=CR;
        }
        else
        {
            SBUF=*string;
        }
        while(TI==0);
        TI=0;
        string++;
    }
}

void main()
{
    UART();
    while(!SdInit()); //�ȴ�SD����ʼ�����
    SdWriteBlock("THIS IS A TEST!",0x000000,15);//д���ַ�����Ȼ��������м���
    SdReadBlock(DATA,0x000000,15);
    Sen_String(DATA);        //��������
    while(1)
   {
   }
}