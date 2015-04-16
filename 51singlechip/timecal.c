#include <reg51.h>

void Delay10ms(unsigned char ntime);
sbit wei = P2^7;
sbit duan = P2^6;
unsigned char num[15] = {
    0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D,
    0X07, 0X7F, 0X6F
};
int temp[20];		//保存每位数字信息
int tot,tot2,tp;
void show(unsigned int number,unsigned int pos) {	//显示单个数字
    wei = 1;
		P0 = pos;
		wei = 0;
	
		duan = 1;
		P0 = num[number];
		duan = 0;
}
void shownum(long number) {		//显示一整个数字
    tot = 0;
    while(number > 0) {
        temp[tot ++] = number % 10;
        number /= 10;
    }
    tot2 = tot;
    tp = 1;
    while(tot2 --) {
        show(temp[tot2],~tp);
				//show(1,~tp);
				Delay10ms(1);
			  tp <<= 1;
    }
}
unsigned int cnt = 0;
unsigned int t = 0;
void main() {
	
		//interrupt
    EA = 1;
    ET1 = 1;
		//TMOD
		TMOD = 0X10;
		//Starter value
		TH1 = 0XDC;
		TL1 = 0X00;
		
		//Start
		TR1 = 1;
	
		//Test TF0 Do something
    while(1) {
			shownum(12345678);
		}	

}

//interrupt procedure
void Timer0Interrupt(void) interrupt 3
{
	TH1 = 0XDC;
	TL1 = 0X00;
	//TF0 = 0;	//
  cnt ++;
  if(1 == cnt) {
    t+=10;
		
		cnt = 0;
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
