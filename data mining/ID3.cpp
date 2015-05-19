/***********************************************\
 |Author: YMC
 |Created Time: 2015/5/17 12:50:37
 |File Name: ID3.cpp
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
int num_of_attr;
int num_of_case;
map<string, int> ma[100];
int num_of_type[100];
string attr_now;
//存放所有数据
vector<vector<int> > trans;
vector<int> rst(100);
//临时变量
vector<int> temp;
struct edge {
    int v;  //新的节点的标号
    int attr;
    int type;
    bool flag;  //是否终结
    bool result;    //结果
};
vector<edge> tree[1000];
//0是失败的记录，1是成功的记录，每条记录第一个数是attr标号，第二个数是attr的type
map<pair<int,int>,int> record[2];

int node_top;
inline bool check(int cas, vector<pair<int,int> > vec) {
    for(int i=0 ;i<vec.size(); ++i) {
        pair<int,int> &tp = vec[i];
        if(trans[cas][tp.first] != tp.second)  return false;
    }
    return true;
}
pair<bool,bool> is_finished(vector<pair<int,int> >vec) {
    int fg = 2;
    int count[2] = {0, 0};
    for(int i = 0; i < num_of_case; ++i) {
        if(check(i,vec)) {
            if(fg == 2) {
                fg = rst[i];
            }
            if(fg != rst[i] && vec.size() < num_of_attr) return MP(false,false);
            count[rst[i]] ++;
        }
    }
    if(count[0] > count[1]) return MP(true, false);
    else return MP(true, true);
}

inline bool Is_attr_not_in_vec(int num,vector<pair<int,int> > vec) {
    for(int i = 0; i < vec.size(); ++i) {
        if(vec[i].first == num) return false;
    }
    return true;
}

int Attribute_selection_method(vector<pair<int,int> > vec) {
    double count[2] = {0, 0};
    int countall;
    record[0].clear(); record[1].clear();
    for(int i = 0;i < num_of_case; ++i) {
        if(check(i,vec)) {
            count[rst[i]] ++;
            for(int j = 0;j < num_of_attr; ++j) {
                record[rst[i]][MP(j,trans[i][j])] ++;
                //count[rst[i]] ++;
            }
        }
    }
    //cout<<"   count    ==  " << count[0] << "  " << count[1] <<endl;
    //countall为总的数目,eps防止浮点数误差，自动取整
    countall = int(count[0] + count[1] + eps);
    count[0] /= countall; count[1] /= countall;
    //cout<<count[0]<<"  "<<count[1]<<endl;
    //all 为整体的信息熵
    double all = -count[0]*log2(count[0]) - count[1]*log2(count[1]);
    //cout<<"all == "<<all<<endl;
    //求出各自属性的信息熵，以特定格式保存在result中。
    vector<pair<int,double> >result;
    double anstemp;
    result.clear();
    int nowf, nowt;
    double nowff, nowtt;
    for(int i = 0;i < num_of_attr; ++i) {
        if(Is_attr_not_in_vec(i, vec)) {
            anstemp = all;
            for(int j = 0; j < num_of_type[i]; ++j) {
                nowf = record[0][MP(i,j)];
                nowt = record[1][MP(i,j)];
                if(nowf + nowt != 0) {
                    nowff = nowf*1.0 / (nowf + nowt);
                    nowtt = nowt*1.0 / (nowf + nowt);
                } else {
                    nowff = 0;
                    nowtt = 0;
                }
                double zen = 0;
                if(fabs(nowff - 0) < eps) {
                    zen -= nowff * log2(nowff);
                }
                if(fabs(nowtt - 0) < eps) {
                    zen -= nowtt * log2(nowtt);
                }
                if(fabs(zen - 0) < eps) {
                    anstemp -= (nowf + nowt) * 1.0 / countall * (zen); 
                }
            }
            result.push_back(MP(i, anstemp));
        }
    }
    
    //取最大信息熵
    int ans;
    double nowmaxn = -inf;
    for(int i = 0; i < result.size(); ++i) {
        //cout<<"========="<<result[i].first<<"  "<<result[i].second<<endl;
        if(result[i].second > nowmaxn) {
            nowmaxn = result[i].second;
            ans = result[i].first;
        }
    }
    //cout<<ans<<endl;
    return ans;
}

void Generate_dicision_tree(vector<pair<int,int> >vec,int now_node,int pre_node) {
    cout<<"nownode == "<<now_node<<endl;
    pair<bool, bool> tfg = is_finished(vec);
    if(tfg.first) {
        for(int i=0; i<tree[pre_node].size(); ++i) {
            if(tree[pre_node][i].v == now_node) {
                tree[pre_node][i].flag = true;
                tree[pre_node][i].result = tfg.second;
                return ;
            }
        }
        //tree[now_node].flag = true;
        //tree[now_node].result = tfg.second;
        return ;
    }
    int attr_select = Attribute_selection_method(vec);
    for(int i = 0; i < num_of_type[attr_select]; ++i) {
        vector<pair<int,int> > vec_tp(vec);
        vec_tp.push_back(MP(attr_select, i));
        edge tedge;
        tedge.attr = attr_select;
        tedge.type = i;
        tedge.v = node_top;
        Generate_dicision_tree(vec_tp, node_top ++, now_node++);
    }
    
}
int main() {
	freopen("input.txt","r",stdin); 
    //属性的数量
    cin>>num_of_attr;
    //数据量
    cin>>num_of_case;
    //清空原始数据
    rep(i, num_of_attr + 1) {
        ma[i].clear();
        num_of_type[i] = 1;
    }

    //读入
    rep(i, num_of_case) {
        temp.clear();
        rep(j, num_of_attr) {
            cin>>attr_now;
            if(ma[j][attr_now] == 0) {
                ma[j][attr_now] = num_of_type[j] ++;
            }
            temp.push_back(ma[j][attr_now]);
        }
        cin>>rst[i];
        trans.push_back(temp);
    }

    //print, just for debug
    rep(i, num_of_case) {
        rep(j, num_of_attr) {
            cout<<trans[i][j]<<"  ";
        }
        printf("%d\n",rst[i]);
    }


    //vec:循环当前选了多少属性,每种属性的标号和类别
    vector<pair<int,int> > vec;
    node_top = 1;
    //生成决策树
    Generate_dicision_tree(vec, 0, 0);
    


    

    
	return 0;
}

