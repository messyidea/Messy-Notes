#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

sbit wei = P2^7;
sbit duan = P2^6;

//keys
sbit key_incr = P3^2;
sbit key_decr = P3^3;

unsigned char cnt, count_timer;
unsigned char leddata[]={ 
 
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
                0x5C,  //"o"
                0x40,  //"-"
                0x00,  //??
                0x00  //???
 
                         };


void display(unsigned int count);
void delay(void);

void main()
{
	count_timer = 0;
	
	while(1)
	{
		display(count_timer);
		
		if(0 == key_incr) 
		{
			delay();
			if(0 == key_incr)
			{
				count_timer ++;
				if(count_timer == 100) 
				{
					count_timer = 0;
				}
				while(!key_incr)
				{
					display(count_timer);
				}
			}
		}
		
		if(0 == key_decr && count_timer > 0)
		{
			delay();
			if(0 == key_decr)
			{
				count_timer --;
			}
			while(!key_decr)
			{
				display(count_timer);
			}
		}
	}
}

void display(unsigned int count)
{
	unsigned char shi,ge;
	
	shi = count / 10 % 10;
	ge = count % 10;
	
	P0 = 0XFF;
	
	wei = 1;
		P0 = 0XFE;
	wei = 0;
	
	duan = 1;
		P0 = leddata[shi];
	duan = 0;
	
	delay();
	
	wei = 1;
		P0 = 0XFD;
	wei = 0;
	
	duan = 1;
		P0 = leddata[ge];
	duan = 0;
	
	delay();
}

void delay(void)   //?? -0.000000000002us
{
    unsigned char a,b;
    for(b=249;b>0;b--)
        for(a=17;a>0;a--);
}
