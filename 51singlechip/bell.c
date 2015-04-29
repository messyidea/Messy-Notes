#include <reg52.h>

sbit BUZZ = P2^3;

void initTimer0();

void main()
{
	initTimer0();
	while(1);
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