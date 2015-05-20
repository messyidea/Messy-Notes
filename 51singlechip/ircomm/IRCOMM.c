#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
#define DataPort P0
sbit IR=P3^2;
sbit dula=P2^6;
sbit wela=P2^7;
uchar IRtime;
bit IR_ok;
bit IR_receive_ok;
uchar IRcord[4];
uchar IRdata[33];
uchar table[]={ 
                0x3F,  //"0"
                0x06,  //"1"
                0x5B,  //"2"
                0x4F,  //"3"
                0x66,  //"4"
                0x6D,  //"5"
                0x7D,  //"6"
                0x07,  //"7"
                0x7F,  //"8"
                0x6F,  //"9"
                0x77,  //"A"
                0x7C,  //"B"
                0x39,  //"C"
                0x5E,  //"D"
                0x79,  //"E"
                0x71,  //"F"
                0x76,  //"H"
                0x38,  //"L"
                0x37,  //"n"
                0x3E,  //"u"
                0x73,  //"P"
                         };
void IR_init()
{
	TMOD=0x22;
	TH0=0x00;
	TL0=0x00;
	EA=1;
	ET0=1;
	TR0=1;

	IT0=1;
	EX0=1;

	TH1=0xfd;
	TL0=0xfd;
	TR1=1;
	SM1=1;
	REN=1;
	ES=1;
}
void time0() interrupt 1
{
	IRtime++;  //277us
}
void int0() interrupt 0
{
	static uchar i;
	static startflag;
	if(startflag)
	{
		if((IRtime>32)&&(IRtime<52))
			i=0;
			IRdata[i]=IRtime;
			IRtime=0;
			i++;
			if(i==33)
			{
				IR_ok=1;
				i=0;
			}
				
	}
	else
	{
		startflag=1;
		IRtime=0;
	}
}
void IRcordpro()
{
	uchar i,j,k,dat,value;
	k=1;
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{
			dat=IRdata[k];
			if(dat>5)
			value|=0x80;
			if(j<7)
				value>>=1;
			k++;
		}
		IRcord[i]=value;
		value=0;
	}
	IR_receive_ok=1;
}
void IRword()
{
	switch(IRcord[2])
	{
		case 0x45:DataPort=table[0];
		break;
		case 0x46:DataPort=table[1];
		break;
		case 0x47:DataPort=table[2];
		break;
		case 0x44:DataPort=table[3];
		break;
		case 0x40:DataPort=table[4];
		break;
		case 0x43:DataPort=table[5];
		break;
		case 0x07:DataPort=table[6];
		break;
		case 0x15:DataPort=table[7];
		break;
		case 0x09:DataPort=table[8];
		break;
		case 0x16:DataPort=table[9];
		break;
		case 0x19:DataPort=table[10];
		break;
		case 0x0d:DataPort=table[11];
		break;
		case 0x0c:DataPort=table[12];
		break;
		case 0x18:DataPort=table[13];
		break;
		case 0x5e:DataPort=table[14];
		break;
		case 0x08:DataPort=table[15];
		break;
		case 0x1c:DataPort=table[16];
		break;
		case 0x5a:DataPort=table[17];
		break;
		case 0x42:DataPort=table[18];
		break;
		case 0x52:DataPort=table[19];
		break;
		case 0x4a:DataPort=table[20];
		break;
		default:break;

	}
	IR_receive_ok=0;
}
void delay(uint z)
{
	uchar x,y;
	for(x=z;x>0;x--)
		for(y=120;y>0;y--);
}
void main()
{
	uchar i;
	IR_init();
	wela=1;
	DataPort=0xfe;
	wela=0;
	dula=1;
	DataPort=0x00;
	while(1)
	{
		if(IR_ok)
		{
			IRcordpro();
			IR_ok=0;	
		}
		if(IR_receive_ok)
		{
			IRword();
			for(i=0;i<3;i++)
			{
				ES=0;
				SBUF=IRcord[i];
				while(!TI);
				TI=0;
				ES=1;
				delay(1000);
			}

		}
	}		
}