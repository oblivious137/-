#include <bitset>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

int n;

inline bitset<100> read()
{
    bitset<100> ret;
    static char tmp[100];
    cin >> tmp;
    n = strlen(tmp);
    for (int i = 0; i < n; ++i)
        if (tmp[i] == '1')
            ret[i] = 1;
    return ret;
}

inline int solve(bitset<100> A)
{
    int ret = 0;
    for (int i = 1; i < n; ++i)
    {
        if (A[i - 1])
        {
            A[i] = A[i] ^ 1;
            A[i + 1] = A[i + 1] ^ 1;
            ++ret;
        }
    }
    if (A[n - 1])
        return 1000000;
    else
        return ret;
}

int main()
{
    bitset<100> state;
    state = read();
    state ^= read();
    int ans = solve(state);
    state[0] = state[0] ^ 1;
    state[1] = state[1] ^ 1;
    ans = min(ans, solve(state) + 1);
    if (ans > 10000)
        cout << "impossible";
    else
        cout << ans;
    return 0;
}