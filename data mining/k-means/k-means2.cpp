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
struct raw {        //ԭʼ����
    vector<double> dou;     //double������
    vector<string> st;      //string������
    int rst;                //result
};
struct Data {
    raw data;       //����
    //string classify;
    int classify_org;       //ԭʼ�����
    int classify_pre;       //֮ǰ����𣬾�����
    int classify_fin;       //���յ���𣬾�����
};
vector<Data> Datas;         //���е�����
Data tdata;                 //��ʱ����
int K, tp, _size, coun;     //K�Ǿ۳ɼ��࣬tp����ʱ���ݣ�_size��Datas�Ĵ�С��coun������
map<int,int> count_map[10];

raw center_raw[100];        //ÿ���������������
raw center[100];            //��������֮��

int center_count[100];      //ÿ�������������
int swap_data[100];
int mindis[100];
int mindispos[100];
int tot = 0;
//tp_*Ϊ��ʱ���ݣ��ṹ�帴�ƻ����������ָ��
raw *tp_raw;
string tp_st;
double tp_dou;
Data *tp_data;
//�������ݵ�����
int num_of_trans;
//��out1.txt�ж���
fstream fin("out1.txt");

//raw�ĸ��ƣ���b���Ƶ�a��
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

//��������a������b�ľ���
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

//�������룬���ո����ĸ�ʽ��
//old  F  0.7  0.1  187  16  18  6.8  3.3  1
//ǰ����string����7��double������һ��int��result,���ݶ���֮���װΪData,�����Datas��
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

//�����ã����a������
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
   
    //�����������
    Datas.clear();
    cout<<"������K\n";
    cin>>K;
    cout<<"���ڶ�������\n";
    fin>>num_of_trans;
    rep(i,num_of_trans) {
        in();
    }
    
    //�õ�_size
    _size = Datas.size();

    cout<<"size == " <<_size<<endl;
    cout<<"�����������,��ʼ����\n";
    //���ȡ��K��������ʱ�����ĵ㣬�ұ�֤�������ظ�
    for(int i=0;i<K;++i) {
        tp = rand()%(_size-i);
        copy_raw(center_raw[i], Datas[tp].data);
        //ȡ��֮��ŵ�����ĩβ���ұ�֤tp����ȡ��ĩβ
        swap(Datas[tp],Datas[_size-1-i]);
    }
    //��¼��������
    int nn = 1;
    while(1) {
        cout<<"���ڽ��е�"<<nn++<<"�ε���\n";
        //��¼���ϴξ�������ͬ����������
        coun = 0;
        int pos = -1;
        double now_dis, minx = inf;
        //center�ŵ��������ܺͣ�Ϊ�˱�֤center_count[i]��Ϊ0�����ȸ�ֵ���ĵ�
        rep(i,K) copy_raw(center[i],center_raw[i]);
        rep(i,K) center_count[i] = 1;
        rep(i,K) {mindis[i] = inf; mindispos[i] = 0;}
        //��������
        rep(i,_size) {
            minx = inf;
            rep(j,K) {
                now_dis = dis(Datas[i].data, center_raw[j]);
                if(now_dis < minx) {
                    minx = now_dis; pos = j;
                }
            }
            if(mindis[pos] > minx) { mindis[pos] = minx; mindispos[pos] = i; }
            //�����ۼӵ�center
            rep(j,dou_len) center[pos].dou[j] += Datas[i].data.dou[j];
            center_count[pos] ++;

            //��ǰ���ڵľ���
            Datas[i].classify_fin = pos;
            //��ǰ���ڵľ����Ƿ��ǰһ�ξ�����ͬ������ǵĻ���coun+1
            if(Datas[i].classify_pre == Datas[i].classify_fin) coun ++;
            //��ǰ�ľ��ำֵ��֮ǰ�ľ���
            Datas[i].classify_pre = Datas[i].classify_fin;
        }
        //���coun��ֵ
        cout<<"count == " << coun<<endl;
        //�����90%�ĵ�û�иı䣬���ʾ�������
        if(coun * 1.0 / _size > 0.90) break;
        //���þ�������ĵ㡣
        rep(i,K) rep(j,dou_len) {
            center_raw[i].dou[j] = center[i].dou[j] / center_count[i];
        }
        rep(i,K) rep(j,center_raw[i].st.size()) {
            center_raw[i].st[j] = Datas[mindispos[i]].data.st[j];
        }
    }
    cout<<"�������"<<endl;
    //���
    map<int,int> map_rst;
    //����������ݺ������ķ���
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
            //�Ѳ��������������ľ������ͼ�¼һ��
            map_rst[Datas[i].classify_fin] ++;
        }
    }
    cout<<"���������˵ĸ�������Ӧ����Ŀ"<<endl;
    map<int,int>::iterator it = map_rst.begin();
    while(it != map_rst.end()) {
        cout<<it->first<<"   "<<it->second<<endl;
        it ++;
    }
    
	return 0;
}

