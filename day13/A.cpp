#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
constexpr int maxn = 300;

long long dp[maxn][maxn][2];
long long ans[maxn];

int main()
{
    dp[0][0][0] = 1;
    ans[0] = 1;
    for (int i = 1; i < maxn; ++i)
        for (int j = 1; j <= i; ++j)
        {
            for (int k = 0; k <= j; ++k)
                dp[i][j][1] += dp[i - j][k][0];
            if (j * 2 <= i)
            {
                for (int k = 0; k <= j; ++k)
                    dp[i][j][0] += dp[i - j * 2][k][0];
            }
            ans[i] += dp[i][j][0] + dp[i][j][1];
        }
    for(;;){
        int n;
        cin>>n;
        if(n==0) break;
        cout<<n<<' '<<ans[n]<<'\n';
    }
    return 0;
}