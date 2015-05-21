/***********************************************\
 |Author: YMC
 |Created Time: 2015/5/20 17:40:56
 |File Name: 数据处理.cpp
 |Description: 
\***********************************************/
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#define L(rt) (rt<<1)
#define R(rt) (rt<<1|1)
#define mset(l,n) memset(l,n,sizeof(l))
#define rep(i,n) for(int i=0;i<n;++i)
#define maxx(a) memset(a, 0x3f, sizeof(a))
#define zero(a) memset(a, 0, sizeof(a))
#define srep(i,n) for(int i = 1;i <= n;i ++)
#define MP make_pair
const int inf=0x3f3f3f3f ;
const double eps=1e-8 ;
const double pi=acos (-1.0);
typedef long long ll;

using namespace std;
int x1,x2,x5,x6,x7,x10;
double x3,x4,x8,x9;
string xx[11];
bool in() {
    return cin>>x1>>x2>>x3>>x4>>x5>>x6>>x7>>x8>>x9>>x10;
}
void out() {
    //cout<<x1<<" "<<x2<<" "<<x3<<" "<<x4<<" "<<x5<<" "<<x6<<" "<<x7<<" "<<x8<<" "<<x9<<" "<<x10<<endl;
    for(int i=1;i<=10;++i) {
        if(i == 5 || i == 7 || i == 9) continue;
        cout<<xx[i]<<"         ";
    }
    cout<<endl;
}
void process() {
     if(x1 < 20) xx[1] = "child";
     else if(x1 < 50) xx[1] = "young";
     else xx[1] = "old";

     if(x2 == 1) xx[2] = "M";
     else xx[2] = "F";

     if(x3 < 3.4) xx[3] = "low";
     else if(x3 <= 20.5) xx[3] = "normal";
     else xx[3] = "hign";

     if(x4 <= 7.32) xx[4] = "normal";
     else xx[4] = "high";
    
     //x5 不能参考
     /*if(x2 == 1) {
         if()
     }*/
    
     if(x6 < 40) xx[6] = "normal";
     else xx[6] = "high";

     //x7不能参考
     
     if(x8 < 6) xx[8] = "low";
     else if(x8 <= 8.2) xx[8] = "nomal";
     else xx[8] = "high";

     //x10无数据

     if(x10 == 1) xx[10] = "1";
     else xx[10] = "0";

}
int main() {
	freopen("data1.txt","r",stdin); 
	freopen("out1.txt","w",stdout);
    int count = 0;
    while(in()) {
        count ++;
        process();
        out();
    } 
    cout<<count<<endl;


	return 0;
}

