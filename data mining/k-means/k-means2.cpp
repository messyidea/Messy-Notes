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
#include <iomanip>
#define L(rt) (rt<<1)
#define R(rt) (rt<<1|1)
#define mset(l,n) memset(l,n,sizeof(l))
#define rep(i,n) for(int i=0;i<n;++i)
#define maxx(a) memset(a, 0x3f, sizeof(a))
#define zero(a) memset(a, 0, sizeof(a))
#define srep(i,n) for(int i = 1;i <= n;i ++)
#define MP make_pair
#define dou_len 9
const int inf=0x3f3f3f3f ;
const double eps=1e-8 ;
const double pi=acos (-1.0);
typedef long long ll;

using namespace std;
struct raw {        //原始数据
    vector<double> dou;     //double型数据
    vector<string> st;      //string型数据
    int rst;                //result
};
struct Data {
    raw data;       //数据
    //string classify;
    int classify_org;       //原始的类别
    int classify_pre;       //之前的类别，聚类用
    int classify_fin;       //最终的类别，聚类用
};
vector<Data> Datas;         //所有的数据
Data tdata;                 //临时数据
int K, tp, _size, coun;     //K是聚成几类，tp是临时数据，_size是Datas的大小，coun计数用
map<int,int> count_map[10];

raw center_raw[100];        //每个聚类的中心数据
raw center[100];            //中心数据之和

int center_count[100];      //每个聚类的数据量
int swap_data[100];
int mindis[100];
int mindispos[100];
int tot = 0;
//tp_*为临时数据，结构体复制会出错，改用了指针
raw *tp_raw;
string tp_st;
double tp_dou;
Data *tp_data;
//输入数据的条数
int num_of_trans;
//从out1.txt中读入
fstream fin("out1.txt");

//raw的复制，把b复制到a中
void copy_raw(raw &a, raw &b) {
    a.st.clear();
    a.dou.clear();
    for(int i=0;i<b.st.size();++i) {
        a.st.push_back(b.st[i]);
    }
    for(int i=0;i<b.dou.size();++i) {
        a.dou.push_back(b.dou[i]);
    }
    a.rst = b.rst;
}

//计算数据a和数据b的距离
double dis(raw &a, raw &b) {
    double ans = 0;
    double mx = -1;
    for(int i = 0;i < a.dou.size(); ++i) {
        double t = (a.dou[i] - b.dou[i])*(a.dou[i] - b.dou[i]);
        ans += t;
        if(t > mx) mx = t;
    }
    for(int i = 0;i < a.st.size(); ++i) {
        if(a.st[i] != b.st[i]) ans += mx;
    }
    return ans;
}

//数据输入，按照给定的格式。
//old  F  0.7  0.1  187  16  18  6.8  3.3  1
//前两个string，后7个double，还有一个int的result,数据读入之后包装为Data,存放在Datas中
void in() {
    tp_raw = new raw();
    tp_raw->dou.clear();
    tp_raw->st.clear();
    fin>>tp_st;
    tp_raw->st.push_back(tp_st);
    fin>>tp_st;
    tp_raw->st.push_back(tp_st);
    rep(i,7) {
        fin>>tp_dou;
        tp_raw->dou.push_back(tp_dou);
    }
    fin>>tp_raw->rst;
    //cout<<tp_raw->rst<<endl;
    tp_data = new Data();
    copy_raw(tp_data->data,*tp_raw);
    tp_data->classify_pre = -1;
    
    Datas.push_back(*tp_data);
}

//调试用，输出a的数据
void debug(raw &a) {
    for(int i=0;i<a.st.size();++i) {
        cout<<a.st[i]<<"  ";
    }
    cout<<endl;
    for(int i=0;i<a.dou.size();++i) {
        cout<<a.dou[i]<<"  ";
    }
    cout<<endl;
}



int main() {
	//freopen("input.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    ios::sync_with_stdio(false);
    srand(time(0));
   
    //清空所有数据
    Datas.clear();
    cout<<"请输入K\n";
    cin>>K;
    cout<<"正在读入数据\n";
    fin>>num_of_trans;
    rep(i,num_of_trans) {
        in();
    }
    
    //得到_size
    _size = Datas.size();

    cout<<"size == " <<_size<<endl;
    cout<<"读入数据完毕,开始聚类\n";
    //随机取出K个当成暂时的中心点，且保证不会有重复
    for(int i=0;i<K;++i) {
        tp = rand()%(_size-i);
        copy_raw(center_raw[i], Datas[tp].data);
        //取出之后放到数据末尾，且保证tp不能取到末尾
        swap(Datas[tp],Datas[_size-1-i]);
    }
    //记录迭代次数
    int nn = 1;
    while(1) {
        cout<<"正在进行第"<<nn++<<"次迭代\n";
        //记录与上次聚类结果相同的数据条数
        coun = 0;
        int pos = -1;
        double now_dis, minx = inf;
        //center放的是数据总和，为了保证center_count[i]不为0，则先赋值中心点
        rep(i,K) copy_raw(center[i],center_raw[i]);
        rep(i,K) center_count[i] = 1;
        rep(i,K) {mindis[i] = inf; mindispos[i] = 0;}
        //遍历数据
        rep(i,_size) {
            minx = inf;
            rep(j,K) {
                now_dis = dis(Datas[i].data, center_raw[j]);
                if(now_dis < minx) {
                    minx = now_dis; pos = j;
                }
            }
            if(mindis[pos] > minx) { mindis[pos] = minx; mindispos[pos] = i; }
            //数据累加到center
            rep(j,dou_len) center[pos].dou[j] += Datas[i].data.dou[j];
            center_count[pos] ++;

            //当前所在的聚类
            Datas[i].classify_fin = pos;
            //当前所在的聚类是否和前一次聚类相同，如果是的话，coun+1
            if(Datas[i].classify_pre == Datas[i].classify_fin) coun ++;
            //当前的聚类赋值给之前的聚类
            Datas[i].classify_pre = Datas[i].classify_fin;
        }
        //输出coun的值
        cout<<"count == " << coun<<endl;
        //如果有90%的点没有改变，则表示聚类完成
        if(coun * 1.0 / _size > 0.90) break;
        //重置聚类的中心点。
        rep(i,K) rep(j,dou_len) {
            center_raw[i].dou[j] = center[i].dou[j] / center_count[i];
        }
        rep(i,K) rep(j,center_raw[i].st.size()) {
            center_raw[i].st[j] = Datas[mindispos[i]].data.st[j];
        }
    }
    cout<<"聚类结束"<<endl;
    //输出
    map<int,int> map_rst;
    //依次输出数据和所属的分类
    rep(i,Datas.size()) {
        for(int j=0;j<Datas[i].data.st.size();++j) {
            cout<<setw(10)<<Datas[i].data.st[j]<<"  ";
        }
        for(int j=0;j<Datas[i].data.dou.size();++j) {
            cout<<setw(10)<<Datas[i].data.dou[j]<<"  ";
        }
        cout<<setw(10)<<Datas[i].classify_fin<<"  ";
        cout<<setw(10)<<Datas[i].data.rst<<endl;
        if(Datas[i].data.rst == 2) {
            //把不生病的人所属的聚类类型记录一下
            map_rst[Datas[i].classify_fin] ++;
        }
    }
    cout<<"不生病的人的各种类别对应的数目"<<endl;
    map<int,int>::iterator it = map_rst.begin();
    while(it != map_rst.end()) {
        cout<<it->first<<"   "<<it->second<<endl;
        it ++;
    }
    
	return 0;
}

