#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;
const int maxn=100100;

int pos[maxn];
int n, m;

bool judge(int d){
    int lst=0,ct=0;
    for (int i=0;i<n;++i){
        if (pos[i]-lst<d) ++ct;
        else lst=pos[i];
    }
    if (pos[n]-lst<d) ++ct;
    return ct<=m;
}

int main(){
    int l,r,mid;
    cin>>l>>n>>m;
    pos[n]=l;
    for (int i=0;i<n;++i) cin>>pos[i];
    l=0,r=pos[n];
    while(l<r){
        mid=(l+r+1)/2;
        if (judge(mid)) l=mid;
        else r=mid-1;
    }
    cout<<l;
}