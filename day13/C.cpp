#include<vector>
#include<bitset>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;
constexpr int maxn = 210,maxx=10100;

bitset<maxx> dp[maxn][2];
int n,x;
int a[maxn];

int main(){
    cin>>n>>x;
    for(int i=1;i<=n;++i) cin>>a[i];
    dp[0][0][0]=1;
    dp[n+1][1][0]=1;
    for (int i=1;i<=n;++i) dp[i][0]=dp[i-1][0]|(dp[i-1][0]<<a[i]);
    for (int i=n;i>0;--i) dp[i][1]=dp[i+1][1]|(dp[i+1][1]<<a[i]);
    vector<int>ans;
    for (int i=1;i<=n;++i){
        bool fl=true;
        for (int j=0;j<=x;++j)if(dp[i-1][0][j]&&dp[i+1][1][x-j]){
            fl=false;
            break;
        }
        if (fl) ans.push_back(a[i]);
    }
    cout<<ans.size()<<endl;
    for (auto x:ans) cout<<x<<' ';
    return 0;
}