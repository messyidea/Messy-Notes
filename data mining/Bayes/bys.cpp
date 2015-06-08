/***********************************************\
 |Author: YMC
 |Created Time: 2015/6/7 13:37:47
 |File Name: bys.cpp
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
#include <fstream>
#include <sstream>
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
int num_of_attr;
int num_of_case;
string attr_name[1000];

map<pair<int,int>,string> names;
map<string,int> ma[1000];
//存放所有数据
vector<vector<int> > trans;
vector<int> rst(1000);
int num_of_type[1000];
string attr_now;
vector<int> temp;
map<pair<int,int>,double> probability[2];
map<pair<int,int>,int> record[2];
int coun[10] = {0};

bool do_test(vector<int>& test) {
    int len = test.size();
    double an[2] = {1,1};
    for(int i=0;i<len-1;++i) {
        an[0]*=probability[0][MP(i,test[i])];
        an[1]*=probability[1][MP(i,test[i])];
    }
    an[0] *= coun[0]*1.0/(coun[0] + coun[1]);
    an[1] *= coun[1]*1.0/(coun[0] + coun[1]);
    //if(an[0] >= an[1]) cout<<0<<"   "<<test[len-1]<<endl;
    //else cout<<1<<"   "<<test[len-1]<<endl;
    if(an[0] >= an[1]) {
        return test[len-1] == 0;
    } else {
        return test[len-1] == 1;
    }
}

int main() {
	//freopen("input.txt","r",stdin); 
    ios::sync_with_stdio(false);
    fstream fin("input1.txt");
    fstream fin2("input2.txt");

    //属性的数量
    fin>>num_of_attr;
    //数据量
    fin>>num_of_case;

    for(int i=0;i<num_of_attr;++i) fin>>attr_name[i];
    //清空原始数据
    for(int i=0;i<num_of_attr;++i) {
        ma[i].clear();
        num_of_type[i] = 1;
    }
    names.clear();

    //读入
    rep(i, num_of_case) {
        temp.clear();
        rep(j, num_of_attr) {
            fin>>attr_now;
            if(ma[j][attr_now] == 0) {
                ma[j][attr_now] = num_of_type[j] ++;
                names[MP(j,num_of_type[j]-1)] = attr_now;
            }
            temp.push_back(ma[j][attr_now]);
        }
        fin>>rst[i];
        //cout<<rst[i]<<endl;
        coun[rst[i]] ++;
        trans.push_back(temp);
    }

    /*rep(i, num_of_case) {
        rep(j, num_of_attr) {
            cout<<trans[i][j]<<"  ";
        }
        cout<<rst[i]<<endl;
    }*/

    //caculate
    rep(i,num_of_case) {
        rep(j,num_of_attr) {
            record[rst[i]][MP(j,trans[i][j])] ++;
        }
    }
    rep(i,num_of_attr) {
        for(int j=1;j<num_of_type[i];++j) {
            if(coun[0] != 0)
                probability[0][MP(i,j)] = record[0][MP(i,j)] * 1.0 / coun[0];
            if(coun[1] != 0)
                probability[1][MP(i,j)] = record[1][MP(i,j)] * 1.0 / coun[1];
            //cout<<probability[0][MP(i,j)]<<"     ";
            //cout<<probability[0][MP(i,j)]<<"     \n";
        }
    }

    
    //do_test
    //属性的数量
    fin2>>num_of_attr;
    //数据量
    fin2>>num_of_case;

    for(int i=0;i<num_of_attr;++i) fin2>>attr_name[i];
    vector<int> trans_temp;
    int num_temp;
    int true_num = 0, false_num = 0;
    rep(i,num_of_case) {
        trans_temp.clear();
        rep(j, num_of_attr) {
            fin2>>attr_now;
            trans_temp.push_back(ma[j][attr_now]);
        }
        fin2>>num_temp;
        trans_temp.push_back(num_temp);
        if(do_test(trans_temp)) true_num ++;
        else false_num ++;
    }
    cout<<"test finished"<<endl;
    cout<<"true_num  ==    "<<true_num<<endl;
    cout<<"false_num ==    "<<false_num<<endl;

	return 0;
}

