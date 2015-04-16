#include <reg52.h>
#include <intrins.h>

/******  author:ymc  *******/
void Delay10ms(unsigned char ntime);

sbit wei = P2^7;
sbit duan = P2^6;

void main() 
{
	wei = 1;
	P0 = 0XE0;
	wei = 0;
	
	duan = 1;
	P0 = 0X06;
	duan = 0;
	
	Delay10ms(50);
	
	wei = 1;
	P0 = 0X0E;
	wei = 0;
	
	duan = 1;
	P0 = 0X06;
	duan = 0;
	
	Delay10ms(50);
	
	while(1);
}

void Delay10ms(unsigned char ntime) 
{
	unsigned char a,b;
	for(; ntime > 0; ntime--) {
		for(a=38; a>0; a--) {
			for(b=130;b>0;b--);
		}
	}
}
