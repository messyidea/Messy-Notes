#include <reg52.h>
#include "ircomm.h"
#include "move.h"
#include "speed.h"
#include "lcd.h"
#include <intrins.h>
#include "i2c.h"

#define DataPort P0
//sbit IR=P3^2;
sbit dula=P2^6;
sbit wela=P2^7;
int SS;
uchar IRtime;
//小车运行的状态
extern unsigned char now;
extern unsigned char pre;
unsigned char now_tp;
bit IR_ok;
bit IR_receive_ok;
uchar IRcord[4];
uchar IRdata[33];
int time;
//保存接收到的数据处理之后的值
int pos;
//是否进入自动循迹模式
bit AutoRun;
sbit Left_led = P3^5;	 //左边的探头
sbit Right_led = P3^6; //右边的探头
int time1_count = 0;

//遥控板上每个按钮的数值
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
	//两个定时器都以方式2工作
	TMOD=0x22;
	TH0=0x00;
	TL0=0x00;
	//开总中断
	EA=1;
	//定时器0允许中断
	ET0=1;
	//定时器0启动
	TR0=1;
	
	//触发方式，等于1是下降沿触发，0是低电平触发
	IT0=1;
	//打开外部中断0中断
	EX0=1;
	
	TH1=0x0F8;
	TL0=0x0CD;
	//定时器1开启
	EA = 1;
  ET1 = 1;
  //TR1 = 1;
	
	
	SM1=1;
	//允许接收位
	REN=1;
	//允许串口中断
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
		//接收32个时间段
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
			//高电平时间为560ms,接收到1位1
			if(dat>5)
			value|=0x80;
			//左移1位
			if(j<7)
				value>>=1;
			k++;
		}
		//value保存在IRcord中
		IRcord[i]=value;
		value=0;
	}
	//全部接收成功之后，IR_receive_ok置1
	IR_receive_ok=1;
}
void IRword()
{
	//判断接收到的数据是什么，然后赋给DataPort相应值
	//这个本来是因为在数字灯上显示用的
	//但是做到后来准备用LCD1602代替数字灯。
	//所以这段代码现在只用来给DataPort赋值
	//赋值后在后面可以很方便根据table表处理pos值
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
	//把它置0，可以再次接收。
	IR_receive_ok=0;
}

void delay(uint z)
{
	uchar x,y;
	for(x=z;x>0;x--)
		for(y=120;y>0;y--);
}


void control()
{
	uchar i;
	IR_init();
	/*wela=1;
	DataPort=0xfe;
	wela=0;
	dula=1;*/
	DataPort=0x00;
	
	AutoRun = 0;
	while(1)
	{
		//当正处于AutoRun状态时，执行IF块，自动循迹
		if(AutoRun)
		{
			//两边都检测到黑线，在轨道上，直走
			if(Left_led==1&&Right_led==1)
			{
				run();	
			} else {
				//右边检测不到黑线，需左转
				if(Right_led==0&&Left_led==1)
				{
					leftrun();
				}
				//左边检测不到黑线，需右转
				else if(Left_led==0&&Right_led==1)
				{
					rightrun();
				}
				//否则继续前进
				else {
					run();
				}
			}
		}
		//相关数据得到了
		if(IR_ok)
		{
			//判断是否得到哪些数据，保存在DataPort
			IRcordpro();
			IR_ok=0;	
		}
		
		if(IR_receive_ok)
		{
			//判断到底按了哪个键，保存在pos里面。
			IRword();
			pos = -1;
			for(i=0;i<=16;++i)
			{
					if(DataPort == table[i])
					{
						pos = i;
						break;
					}
			}
			//根据pos进行相关操作
			switch(pos)
			{
				case 0:AutoRun=1;pre=now;now=1;
							I2C_TransmitData(0x08,pre);
							LcdAllClear();
							LcdShowStr(0, 0, "Action:");
							LcdShowStr(0, 1, "autorun");
							break;
				case 1:run();AutoRun=0;pre=now;now=2;
							I2C_TransmitData(0x08,pre);
							LcdAllClear();
							LcdShowStr(0, 0, "Action:");
							LcdShowStr(0, 1, "go forward");
							break;
				case 2:AutoRun=0;backrun();pre=now;now=3;
							I2C_TransmitData(0x08,pre);
							LcdAllClear();
							LcdShowStr(0, 0, "Action:");
							LcdShowStr(0, 1, "go back");
							break;
				case 3:AutoRun=0;leftrun();pre=now;now=4;
							I2C_TransmitData(0x08,pre);
							LcdAllClear();
							LcdShowStr(0, 0, "Action:");
							LcdShowStr(0, 1, "go left");
							break;
				case 4:AutoRun=0;rightrun();pre=now;now=5;
							I2C_TransmitData(0x08,pre);
							LcdAllClear();
							LcdShowStr(0, 0, "Action:");
							LcdShowStr(0, 1, "go right");
							break;
				case 5:speed_up();
							LcdAllClear();
							LcdShowStr(0, 0, "Action:");
							LcdShowStr(0, 1, "Speed up");
							break;
				case 6:speed_down();
							LcdAllClear();
							LcdShowStr(0, 0, "Action:");
							LcdShowStr(0, 1, "Speed down");
							break;
				case 7:speed_default();
							LcdAllClear();
							LcdShowStr(0, 0, "Action:");
							LcdShowStr(0, 1, "Speed default");
							break;
				case 8:speed_default();
							now_tp = I2C_ReceiveData(0x08);
							if(now_tp == 0) break;
							now = now_tp;
							run_itself();
							LcdAllClear();
							LcdShowStr(0, 0, "Action:");
							LcdShowStr(0, 1, "revoke");
							break;
				default: AutoRun=0;stop();now=6;
							LcdAllClear();
							LcdShowStr(0, 0, "Action:");
							LcdShowStr(0, 1, "Stop");
							break;						
			}

		}
	}
}