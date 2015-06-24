/*----------------------------------------
控制小车移动
-----------------------------------------*/
#ifndef _MOVE_H_
#define _MOVE_H_


void Left_go();				//左轮前进
void Left_back();			//左轮后退
void Left_stop();			//左轮暂停
void Right_go();			//右轮前进
void Right_back();		//右轮后退
void Right_stop();		//右轮暂停

void run(void);				//前进
void backrun(void);		//后退
void leftrun(void); 	//向左转
void rightrun(void);	//向右转
void stop(void);			//暂停

#endif