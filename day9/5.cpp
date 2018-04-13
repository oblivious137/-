#include<set>
#include<cmath>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

set<pair<int ,int>> s;

int main(){
    s.insert(make_pair(1000000000,1));
    int n;
    cin>>n;
    for (int i=0;i<n;++i){
        int id,v;
        cin>>id>>v;
        pair<int,int> nw(v,id);
        set<pair<int,int>>::iterator it=s.lower_bound(nw);
        int dis=2000000000,d;
        if (it!=s.end()){
            if (abs(it->first-v)<dis) dis=abs(it->first-v),d=it->second;
            else if (abs(it->first-v)==dis && it->first<v) dis=abs(it->first-v),d=it->second;
        }
        if (it!=s.begin()){
            --it;
            if (abs(it->first-v)<dis) dis=abs(it->first-v),d=it->second;
            else if (abs(it->first-v)==dis && it->first<v) dis=abs(it->first-v),d=it->second;
        }
        printf("%d %d\n",id,d);
        s.insert(nw);
    }
    return 0;
}