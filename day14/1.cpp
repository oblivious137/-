#include<cstdio>
#include<iostream>
using namespace std;

int n,m;
int dp[15000];

int main(){
    cin>>n>>m;
    for (int i=0;i<n;++i){
        int a,b;
        cin>>a>>b;
        for (int j=m;j>=a;--j) dp[j]=max(dp[j],dp[j-a]+b);
    }
    cout<<dp[m];
    return 0;
}