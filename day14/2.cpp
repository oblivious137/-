#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

string name[20];
int tim[20], lim[20];

struct REC
{
    int tim,sum;
    vector<int> order;
    bool operator<(const REC &x) const
    {
        if (tim != x.tim)
            return tim < x.tim;
        else
            return order < x.order;
    }
    REC operator + (int id)const{
        REC ret;
        ret.tim=tim+max(sum+(::tim[id])-lim[id],0);
        ret.sum=sum+(::tim[id]);
        ret.order = order;
        ret.order.push_back(id);
        return ret;
    }
} dp[1 << 15];

int main()
{
    int T = 0;
    cin >> T;
    while (T--)
    {
        int n;
        cin >> n;
        for (int i = 0; i < n; ++i)
            cin >> name[i] >> lim[i] >> tim[i];
        for (int i = 1; i < (1 << n); ++i)
        {
            dp[i].tim = 2000000000;
            for (int j=0;j<n;++j) if (i&(1<<j)){
                REC tmp = dp[i^(1<<j)] + j;
                if (tmp<dp[i]) dp[i]=tmp;
            }
        }
        cout<<dp[(1<<n)-1].tim<<endl;
        for (auto i: dp[(1<<n)-1].order)
            cout<<name[i]<<endl;
    }
    return 0;
}