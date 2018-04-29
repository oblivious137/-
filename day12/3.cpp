#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

char mp[4100][2100];

inline void cpy(int a, int b, int x, int y)
{
    for (int i = 0; i < a; ++i)
        for (int j = 0; j < b; ++j)
            mp[x + i][y + j] = mp[i][j];
    return;
}

int main()
{
    for (;;)
    {
        memset(mp, 0, sizeof(mp));
        int n;
        cin >> n;
        if (n == 0)
            break;
        --n;
        sprintf(mp[1], " /\\ ");
        sprintf(mp[0], "/__\\");
        for (int i = 0; i < n; ++i)
        {
            int w = 1 << (i + 2), h = 1 << (i + 1);
            cpy(h, w, h, h);
            cpy(h, w, 0, w);
        }
        for (int i = (1 << (n + 1)) - 1; i >= 0; --i)
        {
            int j;
            for (j = 2099; mp[i][j] == ' ' || mp[i][j] == 0; --j)
                mp[i][j] = 0;
            for (j; j >= 0; --j)
                if (mp[i][j] == 0)
                    mp[i][j] = ' ';
            puts(mp[i]);
        }
        puts("");
    }
    return 0;
}