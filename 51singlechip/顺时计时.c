#include <reg51.h>

void Delay10ms(unsigned char ntime);
sbit wei = P2^7;
sbit duan = P2^6;
unsigned char num[15] = {
    0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D,
    0X07, 0X7F, 0X6F
};
unsigned char temp[20];
unsigned char tot,tot2,tp;
void show(unsigned int number,unsigned int pos) {
    wei = 1;
		P0 = pos;
		wei = 0;
	
		duan = 1;
		P0 = num[number];
		duan = 0;
}
void shownum(int number) {
    tot = 0;
    while(number > 0) {
        temp[tot ++] = number % 10;
        number /= 10;
    }
    tot2 = tot;
    tp = 1;
    while(tot2 --) {
        show(temp[tot2],~tp);
				Delay10ms(1);
			  tp <<= 1;
    }
}
unsigned int cnt = 0;
unsigned int t = 0;
void main() {
	
		 //interrupt
    EA = 1;
    ET0 = 1;
		//TMOD
		TMOD = 0X01;
		//Starter value
		TH0 = 0XDC;
		TL0 = 0X00;
		
		//Start
		TR0 = 1;
	
		//Test TF0 Do something(??)
		//P1 = 0X00;
	  
    while(1) {
			shownum(t);
		}
		

}

//interrupt procedure
void Timer0Interrupt(void) interrupt 1
{
	TH0 = 0XDC;
	TL0 = 0X00;
	//TF0 = 0;	//????
  cnt ++;
  if(100 == cnt) {
    t++;
		cnt = 0;
		//P1 = ~P1;
  }
}

void Delay10ms(unsigned char ntime) 
{
	unsigned char a,b;
	for(; ntime > 0; ntime--) {
		for(a=100; a>0; a--) {
			for(b=10;b>0;b--);
		}
	}
}
