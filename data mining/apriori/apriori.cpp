/***********************************************\
 |Author: YMC
 |Created Time: 2015/4/11 15:54:50
 |File Name: main.cpp
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
#define N 1000
#define min_sup 2
int n, m, tp;
int num;

struct Transaction {    //事务
    vector<int> da;
}trans[N];
vector<vector<int> > data[N];
vector<vector<int> > tdata;     //存放临时向量
int tot;
map<int, int> mm;   //记录l1
vector<int> tv;     //temp vector
bool judge(vector <int> a, vector<int> b) {
    int size = a.size();
    for(int i=0;i<size-1;++i) {
        if(a[i] != b[i]) return false;
    }
    if(a[size-1] >= b[size-1]) return false;
    return true;
}
void link(vector<int> a, vector<int> b, vector<int> &c) {
    c.clear();
    int size = a.size();
    for(int i=0;i<size;++i) {
        c.push_back(a[i]);
    }
    c.push_back(b[size-1]);
}
bool test(vector<int> tp, int level) {
    bool vis[N];
    memset(vis,false,sizeof(vis));
    int size = level + 1;
    for(int i = 0;i<data[level].size(); ++i) {
        vector <int> &tt = data[level][i];

        int count = 0;
        int pos = 0;
        for(int i=0; i < size; ++i) {   //tp is large
            if(tt[pos] == tp[i]) {
                pos ++;
                count ++;
            } else if(tt[pos] > tp[i]) {
                continue;
            } else {
                pos ++;
                i --;
            }
            if(pos >= level) break;
        }
        if(count == level) {        //tt is the subset of tp
            bool flag = false;
            for(int i=0;i<size-1;++i) {
                if(tt[i] == tp[i]) continue;
                else {
                    flag = true;
                    vis[tp[i]] = true;
                    break;
                }
            }
            if(!flag) {
                vis[tp[size-1]] = true;
            }
        }
    }
    for(int i=0;i<size;++i) {
        if(!vis[tp[i]]) return false;
    }
    return true;
}
void aproiri_gen(int now) {
    tdata.clear();
    //link and judge
	int size = data[now - 1].size();
    for(int i=0;i<size;++i) {
        for(int j=i+1;j<size;++j) {
           if(judge(data[now-1][i],data[now-1][j])) {
                link(data[now-1][i], data[now-1][j], tv);
                if(test(tv, now - 1)) {
                    tdata.push_back(tv);
                }
            }
        }
    }
}
bool is_subset(vector<int> &a, vector<int> &b) {    //a是否是b的子集
    int sizea = a.size();
    int sizeb = b.size();
    int pos = 0;
    int count = 0;
    for(int i=0;i<sizeb;++i) {
        if(a[pos] == b[i]) {
            pos ++;
            count ++;
        } else if(a[pos] > b[i]) {
            continue;
        } else {
            pos ++;
            i --;
        }
        if(pos >= sizea) break;
    }
    if(count == sizea) return true;
    return false;
}
int main() {
	freopen("input.txt","r",stdin); 
    int T;
    scanf("%d",&T);
    rep(i,T) {
        scanf("%d", &num);   //num
        trans[i].da.clear();
        rep(j, num) {
            scanf("%d", &tp);
            mm[tp] ++;
            trans[i].da.push_back(tp);
        }
        sort(trans[i].da.begin(), trans[i].da.end());
    }
    tot = 0;
    map<int, int>::iterator it = mm.begin();
    //get l1
    while(it != mm.end()) {
        if(it->second >= min_sup) {
            tv.clear();
            tv.push_back(it->first);
            data[1].push_back(tv);
        } 
        it ++;
    }
    int now = 2;
    while(true) {
        aproiri_gen(now);
        for(int i=0;i<tdata.size();++i) {
            int count = 0;
            for(int j=0;j<T;++j) {
                if(is_subset(tdata[i], trans[j].da)) {
                    count ++;
                }
            }
            if(count >= min_sup) {
                data[now].push_back(tdata[i]);
            }
        }
        if(data[now].size() == 0) break;
        now ++;
    }
    for(int i=1;i<=3;++i) {
        for(int j=0;j<data[i].size();++j) {
            for(int k=0;k<data[i][j].size();++k) {
                cout<<data[i][j][k]<<"  ";
            }
            cout<<endl;
        }
    }
    puts("success");
	return 0;
}

