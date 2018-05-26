#include <vector>
#include <bitset>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
constexpr int maxn = 60;

class Bigint
{
    int a[60], len;

  public:
    Bigint()
    {
        memset(a, 0, sizeof(a));
        len = 0;
    }
    Bigint(int x)
    {
        memset(a, 0, sizeof(a));
        len = 0;
        if (x == 0)
            return;
        len = -1;
        while (x)
        {
            a[++len] = x % 10;
            x /= 10;
        }
    }
    Bigint(const Bigint &x)
    {
        memcpy(a, x.a, sizeof(a));
        len = x.len;
    }
    Bigint(char a) : Bigint((int)(a - '0')){};
    Bigint(char *s, int l)
    {
        memset(a, 0, sizeof(a));
        len = l - 1;
        for (int i = 0; i <= len; ++i)
            a[i] = s[len - i] - '0';
    };
    void repair()
    {
        for (int i = 0; i < 60; ++i)
            if (a[i] >= 10)
            {
                a[i + 1] += a[i] / 10;
                a[i] %= 10;
            }
        for (len = 59; len > 0; --len)
            if (a[len] != 0)
                break;
    }
    Bigint operator+(const Bigint &x) const
    {
        Bigint ret(*this);
        for (int i = 0; i <= x.len; ++i)
            ret.a[i] += x.a[i];
        ret.repair();
        return ret;
    }
    Bigint &operator=(const Bigint &x)
    {
        memcpy(a, x.a, sizeof(a));
        len = x.len;
        return *this;
    }
    bool operator<(const Bigint &x) const
    {
        if (len != x.len)
            return len < x.len;
        for (int i = len; i >= 0; --i)
            if (a[i] != x.a[i])
                return a[i] < x.a[i];
        return false;
    }
    friend ostream &operator<<(ostream &out, const Bigint &x)
    {
        for (int i = x.len; i >= 0; --i)
            out << x.a[i];
        return out;
    }
};

Bigint dp[maxn][maxn];
char a[maxn];

int main()
{
    int n, m;
    while(cin>>m){
        cin >> a + 1;
        n = strlen(a + 1);
        for (int i = 1; i <= n; ++i)
            for (int j = 0; j <= m && j < i; ++j)
            {
                dp[i][j] = Bigint(a + 1, i);
                if (j > 0)
                    for (int k = i - 1; k >= j; --k)
                    {
                        Bigint tmp(dp[k][j - 1] + Bigint(a + k + 1, i - k));
                        if (tmp < dp[i][j])
                            dp[i][j] = tmp;
                    }
                // cout << i << ',' << j << ": " << dp[i][j] << endl;
            }
        cout << dp[n][m]<<endl;
    }
    return 0;
}