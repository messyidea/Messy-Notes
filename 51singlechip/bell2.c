#include <reg52.h>

sbit BUZZ = P2^3;
//keys
sbit key_incr = P3^2;
sbit key_decr = P3^3;

void initTimer0();

void delay(void)   //?? -0.000000000002us
{
    unsigned char a,b;
    for(b=249;b>0;b--)
        for(a=17;a>0;a--);
}

void main()
{
	initTimer0();
	while(1)
	{
			if(0 == key_incr) 
		{
			delay();
			if(0 == key_incr)
			{
				TR0 = 0;
				while(!key_incr);
			}
		}
		
		if(0 == key_decr)
		{
			delay();
			if(0 == key_decr)
			{
				TR0 = 1; 		
				while(!key_incr);
			}
		}
	}
}

void initTimer0()
{
	//interrupt
  EA = 1;
  ET0 = 1;
	//TMOD
	TMOD = 0X01;
	//Starter value
	TH0 = 0XFE;
	TL0 = 0X33;
	//Start
	TR0 = 1;
}

//interrupt procedure
void Timer0Interrupt(void) interrupt 1
{
	TH0 = 0XFE;
	TL0 = 0X33;
	BUZZ = ~BUZZ;
}