/***********************************************\
 |Author: YMC
 |Created Time: 2015/6/10 21:30:07
 |File Name: k-means.cpp
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
#include <map>
#include <fstream>
#include <sstream>
#include <ctime>
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
struct Data {
    double dot[4];
    string classify;
    int classify_org;
    int classify_pre;
    int classify_fin;
};
vector<Data> Datas;
Data tdata;
int K, tp, _size, coun;
map<int,int> count_map[10];
double center[100][4];
double center_point[100][4];
int center_count[100];
int swap_data[100];
int tot = 0;
double dis(double a[4],double b[4]) {
    double ans = 0;
    for(int i=0;i<4;++i) ans += (a[i]-b[i])*(a[i]-b[i]);
    return ans;
}
int main() {
	//freopen("input.txt","r",stdin);
    //freopen("out.txt","w",stdout); 
    ios::sync_with_stdio(false);
    srand(time(0));
    fstream fin("iris.data");
    Datas.clear();
    cout<<"请输入K\n";
    cin>>K;
    cout<<"正在读入数据\n";
    while(fin>>tdata.dot[0]>>tdata.dot[1]>>tdata.dot[2]>>tdata.dot[3]>>tdata.classify) {
        tdata.classify_pre = -1;
        Datas.push_back(tdata);
    }
    _size = Datas.size();
    tot = 0;
    Datas[0].classify_org = tot;
    for(int i=1;i<_size;++i) {
        if(Datas[i].classify != Datas[i-1].classify) Datas[i].classify_org = ++tot;
        else Datas[i].classify_org = tot;
    }
    cout<<"size == " <<_size<<endl;
    cout<<"读入数据完毕,开始聚类\n";
    for(int i=0;i<K;++i) {
        tp = rand() % (_size - i);
        swap_data[i] = tp;
        for(int j=0;j<4;++j) {
            center[i][j] = Datas[tp].dot[j];
        }
        swap(Datas[tp],Datas[_size-1-i]);       //保证现在选的之前都没有出现
    }
    rep(i,K) swap(Datas[swap_data[i]],Datas[_size-1-i]);
    int nn = 1;
    while(1) {
        cout<<"正在进行第"<<nn++<<"次迭代\n";
        coun = 0;
        int pos = -1;
        double now_dis, minx = inf;
        rep(i,K) rep(j,4) center_point[i][j] = center[i][j];
        rep(i,K) center_count[i] = 1;
        rep(i,_size) {
            minx = inf;
            rep(j,K) {
                now_dis = dis(Datas[i].dot, center[j]);
                if(now_dis < minx) {
                    minx = now_dis; pos = j;
                }
            }
            rep(j,4) center_point[pos][j] += Datas[i].dot[j];
            center_count[pos] ++;
            Datas[i].classify_fin = pos;
            if(Datas[i].classify_pre == Datas[i].classify_fin) coun ++;
            Datas[i].classify_pre = Datas[i].classify_fin;
        }
        if(coun * 1.0 / K > 0.90) break;
        rep(i,K) rep(j,4) {
            center[i][j] = center_point[i][j] / center_count[i]; 
        }
    }
    //输出
    rep(i,Datas.size()) {
        for(int j=0;j<4;++j) {
            cout<<Datas[i].dot[j]<<"  ";
        }
        cout<<Datas[i].classify_org<<"  ";
        cout<<Datas[i].classify_fin<<"\n";
    }
    rep(i,tot+1) {
        count_map[i].clear();
    }
    rep(i,_size) {
        count_map[Datas[i].classify_org][Datas[i].classify_fin] ++;
    }
    int num_count = 0;
    rep(i,tot+1) {
        map<int,int>::iterator it = count_map[i].begin();
        int maxx = 0;
        while(it != count_map[i].end()) {
            if(it->second > maxx) maxx = it->second;
            it ++;
        }
        num_count += maxx;
    }
    cout<<num_count<<"     "<<_size<<endl;
    cout<<"准确率为："<<num_count * 1.0 / _size<<endl;
	return 0;
}

