#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;
constexpr int maxn = 110;

int n,m;
int h[maxn][maxn];
int sav[maxn][maxn];

int f(int a,int b){
    if (sav[a][b]!=-1) return sav[a][b];
    sav[a][b]=0;
    if (a!=0&&h[a-1][b]<h[a][b]) sav[a][b]=max(sav[a][b],f(a-1,b)+1);
    if (b!=0&&h[a][b-1]<h[a][b]) sav[a][b]=max(sav[a][b],f(a,b-1)+1);
    if (a!=n-1&&h[a+1][b]<h[a][b]) sav[a][b]=max(sav[a][b],f(a+1,b)+1);
    if (b!=m-1&&h[a][b+1]<h[a][b]) sav[a][b]=max(sav[a][b],f(a,b+1)+1);
    return sav[a][b];
}

int main(){
    memset(sav,-1,sizeof(sav));
    int ans = 0;
    cin>>n>>m;
    for(int i=0;i<n;++i)
    for(int j=0;j<m;++j) cin>>h[i][j];
    for(int i=0;i<n;++i)
    for(int j=0;j<n;++j) ans=max(ans,f(i,j));
    cout<<ans+1;
    return 0;
}