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
#include <fstream>
#include <sstream>
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
map<string, int> ma[10000];
map<pair<int,int>,string> names;
int num_of_type[10000];
string attr_name[100];
string attr_now;
//存放所有数据
vector<vector<int> > trans;
vector<int> rst(10000);
//临时变量
vector<int> temp;
struct edge {
    int v;  //新的节点的标号
    int attr;
    int type;
    bool flag;  //是否终结
    bool result;    //结果
};
vector<edge> tree[10000];
//0是失败的记录，1是成功的记录，每条记录第一个数是attr标号，第二个数是attr的type
map<pair<int,int>,int> record[2];

int node_top;

//第cas组样例是否满足vec
inline bool check(int cas, vector<pair<int,int> > vec);

//是否到叶子节点了，返回元素的第一个参数是是否已经到叶子节点，第二个元素是应该设置的值。
pair<bool,bool> is_finished(vector<pair<int,int> >vec);

//num属性是否已经在选择的属性列表中了
inline bool Is_attr_not_in_vec(int num,vector<pair<int,int> > vec);

//根据已经选择的属性，返回接下来应该选择的属性
int Attribute_selection_method(vector<pair<int,int> > vec);

void Generate_dicision_tree(vector<pair<int,int> >vec,int now_node,int pre_node);

//打印决策树,now_node为当前的node的标号，len表示前面要空的空格
void display_tree(int now_node, int len, edge e,int node_pre);

//根据决策树测试trans_temp是否吻合
bool do_test(vector<int>& trans_temp);

int main() {
    //cin关同步
    ios::sync_with_stdio(false);
    
    fstream fin("input1.txt");
    fstream fin2("input2.txt");

	//freopen("input1.txt","r",stdin); 
	//freopen("out1.txt","w",stdout); 
    //属性的数量
    fin>>num_of_attr;
    //数据量
    fin>>num_of_case;

    for(int i=0;i<num_of_attr;++i) fin>>attr_name[i];
    //清空原始数据
    rep(i, num_of_attr + 1) {
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
        trans.push_back(temp);
    }
    
    //cout<<"in finished"<<endl;
    //print, just for debug
    /*rep(i, num_of_case) {
        rep(j, num_of_attr) {
            cout<<trans[i][j]<<"  ";
        }
        printf("%d\n",rst[i]);
    }*/


    //vec:循环当前选了多少属性,每种属性的标号和类别
    vector<pair<int,int> > vec;
    node_top = 1;
    //生成决策树
    Generate_dicision_tree(vec, 0, 0);
    
    edge et;
    //打印决策树
    display_tree(0, 0, et, 0);
    

    //==================================do test===============================
    //
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
    
    //cout<<true_num<<"  "<<false_num<<endl;
    cout<<"test finished"<<endl;
    cout<<"true_num  ==    "<<true_num<<endl;
    cout<<"false_num ==    "<<false_num<<endl;
	return 0;
}


//第cas组样例是否满足vec
inline bool check(int cas, vector<pair<int,int> > vec) {
    for(int i=0 ;i<vec.size(); ++i) {
        pair<int,int> &tp = vec[i];
        if(trans[cas][tp.first] != tp.second)  return false;
    }
    return true;
}

//是否到叶子节点了，返回元素的第一个参数是是否已经到叶子节点，第二个元素是应该设置的值。
pair<bool,bool> is_finished(vector<pair<int,int> >vec) {
    int fg = 2;
    //count[0]记录结果为false的数量，count[1]记录结果为true的数量
    int count[2] = {0, 0};
    for(int i = 0; i < num_of_case; ++i) {
        if(check(i,vec)) {
            if(fg == 2) {
                fg = rst[i];
            }
            //如果false和true的事件都存在，且属性还没有选完
            if(fg != rst[i] && vec.size() < num_of_attr) return MP(false,false);
            count[rst[i]] ++;
        }
    }
    if(count[0] > count[1]) return MP(true, false);
    else return MP(true, true);
}

//num属性是否已经在选择的属性列表中了
inline bool Is_attr_not_in_vec(int num,vector<pair<int,int> > vec) {
    //遍历，如果vec中有属性等于num，返回false，反之返回true
    for(int i = 0; i < vec.size(); ++i) {
        if(vec[i].first == num) return false;
    }
    return true;
}

//根据已经选择的属性，返回接下来应该选择的属性
int Attribute_selection_method(vector<pair<int,int> > vec) {
    //count记录的是总的满足vec的事件的数目。count[0]代表的是false的数目，count[1]代表的是true的数目
    double count[2] = {0, 0};
    int countall;
    //record里面存着每个属性所得到结果的数目。record[0]是结果为false的数目，record[1]是结果为true的数目
    record[0].clear(); record[1].clear();
    for(int i = 0;i < num_of_case; ++i) {
        if(check(i,vec)) {
            count[rst[i]] ++;
            for(int j = 0;j < num_of_attr; ++j) {
                record[rst[i]][MP(j,trans[i][j])] ++;
            }
        }
    }

    //countall为总的数目,eps防止浮点数误差，自动取整
    countall = int(count[0] + count[1] + eps);

    //现在count[0]是事件为false的百分比，count[1]是事件为true的百分比
    count[0] /= countall; count[1] /= countall;

    //all 为整体的信息熵
    double all = -count[0]*log2(count[0]) - count[1]*log2(count[1]);

    //求出各自属性的信息熵，以特定格式保存在result中。rusult中元素的第一个值是属性的标号，第二个值是信息熵
    vector<pair<int,double> >result;
    double anstemp;
    result.clear();
    //nowf, nowt为每个属性对应false的数量，对应true的数量
    //nowff，nowtt为每个属性对应false的百分比，对应true的百分比
    int nowf, nowt;
    double nowff, nowtt;
    for(int i = 0;i < num_of_attr; ++i) {
        //如果该属性没有在vec中
        if(Is_attr_not_in_vec(i, vec)) {
            //计算信息熵
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
            //放在result中。i为属性标号，anstemp为信息熵。
            result.push_back(MP(i, anstemp));
        }
    }
    
    //取最大信息熵,result中元素的第一个参数是属性，第二个参数是改属性的信息熵
    //ans是最大信息熵的属性。
    int ans;
    double nowmaxn = -inf;
    for(int i = 0; i < result.size(); ++i) {
        if(result[i].second > nowmaxn) {
            nowmaxn = result[i].second;
            ans = result[i].first;
        }
    }
    return ans;
}

void Generate_dicision_tree(vector<pair<int,int> >vec,int now_node,int pre_node) {
    //cout<<"nownode == "<<now_node<<endl;
    pair<bool, bool> tfg = is_finished(vec);
    //如果满足叶子节点的需要
    if(tfg.first) {
        for(int i=0; i<tree[pre_node].size(); ++i) {
            if(tree[pre_node][i].v == now_node) {
                tree[pre_node][i].flag = true;
                tree[pre_node][i].result = tfg.second;
                return ;
            }
        }
        return ;
    }
    //反之选择最佳的属性
    int attr_select = Attribute_selection_method(vec);
    //对应属性的各个值加边，递归
    for(int i = 1; i < num_of_type[attr_select]; ++i) {
        vector<pair<int,int> > vec_tp(vec);
        vec_tp.push_back(MP(attr_select, i));
        edge tedge;
        tedge.attr = attr_select;
        tedge.type = i;
        tedge.v = node_top;
        tedge.flag = false;
        tree[now_node].push_back(tedge);
        Generate_dicision_tree(vec_tp, node_top ++, now_node);
    }
    
}

//打印决策树,now_node为当前的node的标号，len表示前面要空的空格
void display_tree(int now_node, int len, edge e,int node_pre) {
    //cout<<"now_node == " << now_node<<endl;
    //cout<<string(40,'-')<<endl;
    int half = tree[now_node].size() / 2;
    for(int i = 0; i < half; ++i) {
        display_tree(tree[now_node][i].v, len + 15, tree[now_node][i],now_node);
    }
    if(now_node == 0) cout<<"root"<<endl;
    else {
        cout<<string(len,' ')<<now_node<<" fa:"<<node_pre<<" "<<attr_name[e.attr]<<" "<<names[MP(e.attr,e.type)];
        if(e.flag == true) {
            cout<<"  "<<(e.result == 0 ? "False":"True")<<endl;
        } else {
            cout<<endl;
        }
    }
    for(int i = half; i < tree[now_node].size(); ++i) {
        display_tree(tree[now_node][i].v, len + 15, tree[now_node][i], now_node);
    }
    //cout<<string(40,'-')<<endl;
}

bool do_test(vector<int>& trans_temp) {
    //trans_temp最后一个结果为result
    int now_node = 0;
    while(true) {
        edge _et = tree[now_node][0];
        int _attr = _et.attr;
        int type = trans_temp[_attr];
        for(int i=0;i<tree[now_node].size();++i) {
            edge _ee = tree[now_node][i];
            if(_ee.type == type) {
                if(_ee.flag == true) return _ee.result == trans_temp[trans_temp.size()-1];
                else {
                    now_node = _ee.v;
                    break;
                }
            }
        }
    }
}
