#include <reg52.h>
#include "move.h"
#include "lcd.h"


sbit P1_7 = P1 ^ 7;                              // 定义位变量
extern unsigned char now;
int max_count;
unsigned int Timer2_Server_Count;

/*
T2计时器初始化
设晶振为11.0592MHz，每秒钟可以执行921600（11059200/12）个机器周期。而T2每次溢出时最多
经过了65536个机器周期。我们应该尽量让T2定时器的溢出中断的次数最少，这样对主程序的干扰
也就最小。
*/
void T2Init() 
{
	max_count = 8;
	Timer2_Server_Count = 0;
	//时间设定
	RCAP2H=0xFF;
	RCAP2L=0x38;
	//开T2定时器中断
	ET2=1;
	//开总中断
	EA=1;
	//启动T2
	TR2=1;  
	//初始化now为一个无关的数值
  now = 100;
}

//根据now的装填自动运行
void run_itself()
{
		if(now == 2) 
		{
			run();
		} 
		else if(now == 3)
		{
			backrun();
		} 
		else if(now == 4)
		{
			leftrun();
		} 
		else if(now == 5)
		{
			rightrun();
		}
}


void Timer2_Server(void) interrupt 5
{ 
	//手动软件清零
	TF2=0;
	Timer2_Server_Count++;
	if(Timer2_Server_Count>=max_count)                    // T2定时器的预装载值为0x0BDC，溢出16次就是1秒钟。
  {
		stop();
		Timer2_Server_Count=0;
	} else
	{
		run_itself();
	}
}



void speed_up()
{
	max_count += 1;
	if(max_count > 16) {
		max_count = 16;
		LcdAllClear();
		LcdShowStr(0, 0, "Warming:");
		LcdShowStr(0, 1, "Speed is max");
	}
	Timer2_Server_Count = 0;
	
}

void speed_down()
{
	max_count -= 1;
	if(max_count < 2) {
		max_count = 2;
		LcdAllClear();
		LcdShowStr(0, 0, "Warming:");
		LcdShowStr(0, 1, "Speed is min");
	}
	Timer2_Server_Count = 0;
}
	

void speed_default()
{
	max_count = 8;
	Timer2_Server_Count = 0;
}


