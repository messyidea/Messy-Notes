/***********************************************\
 |Author: YMC
 |Created Time: 2015/5/17 13:14:09
 |File Name: ceshi.cpp
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
void dfs(vector<int> a) {
    //a.erase(a.begin());
    rep(i,a.size()) {
        cout<<a[i]<<endl;
    }
}
int main() {
	//freopen("input.txt","r",stdin); 
    /*vector<int> a;
    rep(i,5) a.push_back(i);
    dfs(a);
    rep(i,a.size()) {
        cout<<a[i]<<endl;
    }*/
    cout<<string(10,'0');
    cout<<log2(5.0/9)<<endl;
    set<int>a;
    rep(i,5) a.insert(i);
    set<int>::iterator it = a.begin();
    a.erase(3);
    while(it != a.end()) {
        cout<<(*it)<<endl;
        it ++;
    }

 	return 0;
}

