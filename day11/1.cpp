#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

int sta[100], use[100];
int n;

inline void read(int &x)
{
    x = 0;
    static char tmp[100];
    scanf("%s", tmp);
    for (int i = 0; i < n; ++i)
    {
        x <<= 1;
        if (tmp[i] == 'w')
            x |= 1;
    }
    return;
}

inline int calc(int x)
{
    int ret = 0;
    while (x)
    {
        if (x & 1)
            ++ret;
        x >>= 1;
    }
    return ret;
}

inline void print()
{
    for (int i = 0; i < n; ++i)
        printf("%d, ", use[i]);
    puts("");
}

int main()
{
    scanf("%d", &n);
    for (int i = 0; i < n; ++i)
    {
        read(sta[i]);
    }
    int ans = 10000000;
    int ran = (1 << n) - 1;
    for (int i = 0; i < (1 << n); ++i)
    {
        int tmp = calc(i);
        memcpy(use, sta, sizeof(sta));
        use[0] = sta[0] ^ i ^ ((i << 1) & ran) ^ ((i >> 1) & ran);
        use[1] ^= i;
        for (int j = 1; j < n; ++j)
        {
            tmp += calc(use[j - 1]);
            use[j] ^= use[j - 1];
            use[j] ^= (use[j - 1] << 1) & ran;
            use[j] ^= (use[j - 1] >> 1) & ran;
            use[j + 1] ^= use[j - 1];
        }
        if (use[n - 1] == 0)
            ans = min(ans, tmp);
    }
    if (ans >= 1000000)
        printf("inf");
    else
        printf("%d", ans);
    return 0;
}