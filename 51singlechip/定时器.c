#include <reg51.h>

unsigned char cnt = 0;
void main(void)
{
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
	
		//Test TF0 Do something(灯闪)
		P1 = 0X00;
	  
    while(1);
		/*while(1) {
				if(1 == TF0) {
						cnt ++;
					  
						TH0 = 0XDC;
						TL0 = 0X00;
						TF0 = 0;	//软件清零
				}
				
				if(50 == cnt) {
						cnt = 0;
					  P1 = ~P1;
				}
				
		}*/
}

//interrupt procedure
void Timer0Interrupt(void) interrupt 1
{
	TH0 = 0XDC;
	TL0 = 0X00;
	//TF0 = 0;	//硬件清零
  cnt ++;
  if(50 == cnt) {
    cnt = 0;
		P1 = ~P1;
  }
}
