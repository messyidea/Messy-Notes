/*----------------------------------------
计算小车速度
-----------------------------------------*/
#ifndef _SPEED_H_
#define _SPEED_H_

//T2定时器初始化
void T2Init();
//加速
void speed_up();
//减速
void speed_down();
//重置速度
void speed_default();
//根据now的数值，自动运行
void run_itself();

#endif
