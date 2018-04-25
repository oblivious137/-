#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

int n = 3;
vector<int> modify[9] = {{0, 1, 3, 4},
                         {0, 1, 2},
                         {1, 2, 4, 5},
                         {0, 3, 6},
                         {1, 3, 4, 5, 7},
                         {2, 5, 8},
                         {3, 4, 6, 7},
                         {6, 7, 8},
                         {4, 5, 7, 8}};
int sta[9];
bool fl = true;
vector<int> ans;

inline void appl(int x)
{
    for (auto a : modify[x])
        sta[a] = (sta[a] + 1) % 4;
}

void dfs(int now, vector<int> &tmp)
{
    if (now == 9)
    {
        for (int i = 0; i < 9; ++i)
            if (sta[i])
                return;
        if (fl || tmp.size() < ans.size())
            ans = tmp, fl = false;
        else if (tmp.size() == ans.size() && ans > tmp)
            ans = tmp;
        return;
    }
    dfs(now + 1, tmp);
    for (int i = 0; i < 3; ++i)
    {
        tmp.push_back(now + 1);
        appl(now);
        dfs(now + 1, tmp);
    }
    for (int i = 0; i < 3; ++i)
        tmp.pop_back();
    appl(now);
}

int main()
{
    for (int i = 0; i < 9; ++i)
        scanf("%d", &sta[i]);
    vector<int> gg;
    dfs(0, gg);
    for (auto x : ans)
        printf("%d ", x);
    return 0;
}