/***********************************************\
 |Author: YMC
 |Created Time: 2015/5/20 22:23:24
 |File Name: ceshi2.cpp
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
string s;
int main() {
	//freopen("input.txt","r",stdin); 
    ifstream fin1("input1.txt");
    fin1>>s;
    cout<<s<<endl;

    ifstream fin2("input2.txt");
    fin2>>s;
    cout<<s<<endl;

    ofstream fout("output.txt");
    fout<<s<<endl;
	return 0;
}

