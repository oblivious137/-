#include<map>
#include<cmath>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

map<int ,int> s;

int main(){
    s[1000000000]=1;
    int n;
    cin>>n;
    for (int i=0;i<n;++i){
        int id,v;
        scanf("%d%d\n",&id,&v);
        map<int,int>::iterator it=s.lower_bound(v);
        int dis=2147483647,d=2147483647;
        if (it!=s.end()){
            if (abs(it->first-v)<dis) dis=abs(it->first-v),d=it->second;
            else if (abs(it->first-v)==dis && it->second<d) dis=abs(it->first-v),d=it->second;
        }
        if (it!=s.begin()){
            map<int,int>::iterator x=it;
            --it;
            if (abs(it->first-v)<dis) dis=abs(it->first-v),d=it->second;
            else if (abs(it->first-v)==dis && it->second<d) dis=abs(it->first-v),d=it->second;
            it=x;
        }
        printf("%d %d\n",id,d);
        if (it!=s.end()&&it->first==v){
            it->second=min(it->second,id);
        }
        else s.insert(it,make_pair(v,id));
    }
    return 0;
}