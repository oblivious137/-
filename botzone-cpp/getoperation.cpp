#include <map>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;

int getvalue(int x)
{
    if (x > 51)
        return x - 52 + 13;
    else
        return x / 4;
}

template <typename T>
vector<T> merge(const vector<T> &a, const vector<T> &b)
{
    vector<T> ret;
    for (auto &i : a)
        ret.push_back(i);
    for (auto &i : b)
        ret.push_back(i);
    return ret;
}

vector<vector<int>> getoperation(const vector<int> &_card, const vector<vector<int>> &history)
{
    vector<vector<int>> ans;
    map<int, int> lst;
    if (history.size() > 0 && history[history.size() - 1].size() > 0)
    {
        for (auto &i : history[history.size() - 1])
            lst[getvalue(i)] += 1;
    }
    else if (history.size() > 1 && history[history.size() - 2].size() > 0)
    {
        for (auto &i : history[history.size() - 2])
            lst[getvalue(i)] += 1;
    }
    if (lst.size() > 0)
        ans.push_back(vector<int>());
    // 预处理一下
    vector<vector<int>> card;
    for (int i = 0; i < 15; ++i)
        card.push_back(vector<int>());
    for (auto &i : _card)
        card[getvalue(i)].push_back(i);
    
    // 炸弹
    if (lst.size() == 1 && lst.begin()->second == 4)
    {
        for (int i = lst.begin()->first + 1; i < 13; ++i)
            if (card[i].size() >= 4)
            {
                ans.push_back(card[i]);
            }
    }
    else if (lst.find(13) == lst.end() || lst.find(14) == lst.end())
    {
        for (int i = 0; i < 13; ++i)
            if (card[i].size() >= 4)
            {
                ans.push_back(card[i]);
            }
    }

    // 火箭
    if (card[13].size() > 0 && card[14].size() > 0)
        ans.push_back(merge(card[13], card[14]));

    // 单
    {
        if (lst.size() == 1 && lst.begin()->second == 1)
        {
            for (int i = lst.begin()->first + 1; i < 15; ++i) if (card[i].size()>=1)
            {
                vector<int> tmp;
                tmp.push_back(card[i][0]);
                ans.push_back(tmp);
            }
        }
        else if (lst.size() == 0)
        {
            for (int i = 0; i < 15; ++i) if (card[i].size()>=1)
            {
                vector<int> tmp;
                tmp.push_back(card[i][0]);
                ans.push_back(tmp);
            }
        }
    }

    // 对
    {
        if (lst.size() == 1 && lst.begin()->second == 2)
        {
            for (int i = lst.begin()->first + 1; i < 15; ++i) if (card[i].size()>=2)
            {
                vector<int> tmp;
                tmp.push_back(card[i][0]);
                tmp.push_back(card[i][1]);
                ans.push_back(tmp);
            }
        }
        else if (lst.size() == 0)
        {
            for (int i = 0; i < 15; ++i) if (card[i].size()>=2)
            {
                vector<int> tmp;
                tmp.push_back(card[i][0]);
                tmp.push_back(card[i][1]);
                ans.push_back(tmp);
            }
        }
    }

    // 单顺
    {
        bool flag = true;
        int mn = 100, sl = 0;
        for (auto &i : lst)
        {
            if (i.second != 1)
            {
                flag = false;
                break;
            }
            mn = min(mn, i.first);
            ++sl;
        }
        if (lst.size() > 4 && flag)
        {
            int l = 0;
            for (int i = 11; i > mn; --i)
            {
                if (card[i].size())
                    ++l;
                else
                    l = 0;
                if (l >= sl)
                {
                    vector<int> tmp;
                    for (int j = 0; j < sl; ++j)
                        tmp.push_back(card[i + j][0]);
                    ans.push_back(tmp);
                }
            }
        }
        else if (lst.size() == 0)
        {
            int l = 0;
            for (int i = 11; i >= 0; --i)
            {
                if (card[i].size())
                    ++l;
                else
                    l = 0;
                if (l >= 5)
                {
                    vector<int> tmp;
                    for (int j = 0; j < 4; ++j)
                        tmp.push_back(card[i + j][0]);
                    for (int j = 4; j < l; ++j)
                    {
                        tmp.push_back(card[i + j][0]);
                        ans.push_back(tmp);
                    }
                }
            }
        }
    }

    // 双顺
    {
        bool flag = true;
        int mn = 100, sl = 0;
        for (auto &i : lst)
        {
            if (i.second != 2)
            {
                flag = false;
                break;
            }
            mn = min(mn, i.first);
            ++sl;
        }
        if (lst.size() > 4 && flag)
        {
            int l = 0;
            for (int i = 11; i > mn; --i)
            {
                if (card[i].size())
                    ++l;
                else
                    l = 0;
                if (l >= sl)
                {
                    vector<int> tmp;
                    for (int j = 0; j < sl; ++j)
                        tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]);
                    ans.push_back(tmp);
                }
            }
        }
        else if (lst.size() == 0)
        {
            int l = 0;
            for (int i = 11; i >= 0; --i)
            {
                if (card[i].size())
                    ++l;
                else
                    l = 0;
                if (l >= 5)
                {
                    vector<int> tmp;
                    for (int j = 0; j < 4; ++j)
                        tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]);
                    for (int j = 4; j < l; ++j)
                    {
                        tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]);
                        ans.push_back(tmp);
                    }
                }
            }
        }
    }

    // 三带 &&飞机
    {
        int sl = 0, sn = 0, mn = 100;
        bool flag = false;
        for (auto &i : lst)
        {
            if (i.second == 3)
            {
                flag = true;
                ++sl;
                mn = min(mn, i.first);
            }
            else
                sn = i.second;
        }
        if (flag)
        {
            int l = 0;
            for (int i = 12; i > mn; --i)
            {
                if (card[i].size() >= 3)
                    ++l;
                else
                    l = 0;
                if (l >= sl)
                {
                    vector<int> tmp, canuse;
                    for (int j = 0; j < sl; ++j)
                        tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]), tmp.push_back(card[i + j][2]);
                    for (int j = 0; j < i; ++j)
                        if (card[j].size() >= sn)
                            canuse.push_back(j);
                    for (int j = i + sl; j < 15; ++j)
                        if (card[j].size() >= sn)
                            canuse.push_back(j);
                    function<void(int)> func = [&](int pos) {
                        if (tmp.size() == (sn + 3) * sl)
                        {
                            ans.push_back(tmp);
                            return;
                        }
                        if ((canuse.size() - pos) * sn + tmp.size() < (sn + 3) * sl)
                            return;
                        func(pos + 1);
                        for (int i = 0; i < sn; ++i)
                            tmp.push_back(card[canuse[pos]][i]);
                        func(pos + 1);
                        for (int i = 0; i < sn; ++i)
                            tmp.pop_back();
                        return;
                    };
                    func(0);
                }
                if (i == 12)
                    l = 0;
            }
        }
        else if (lst.size() == 0)
        {
            int l = 0;
            for (int i = 12; i >= 0; --i)
            {
                if (card[i].size() >= 3)
                    ++l;
                else
                    l = 0;
                for (sl = 1; sl <= l; ++sl)
                    for (sn = 0; sn < 3; ++sn)
                    {
                        vector<int> tmp, canuse;
                        for (int j = 0; j < sl; ++j)
                            tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]), tmp.push_back(card[i + j][2]);
                        for (int j = 0; j < i; ++j)
                            if (card[j].size() >= sn)
                                canuse.push_back(j);
                        for (int j = i + sl; j < 15; ++j)
                            if (card[j].size() >= sn)
                                canuse.push_back(j);
                        function<void(int)> func = [&](int pos) {
                            if (tmp.size() == (sn + 3) * sl)
                            {
                                ans.push_back(tmp);
                                return;
                            }
                            if ((canuse.size() - pos) * sn + tmp.size() < (sn + 3) * sl)
                                return;
                            func(pos + 1);
                            for (int i = 0; i < sn; ++i)
                                tmp.push_back(card[canuse[pos]][i]);
                            func(pos + 1);
                            for (int i = 0; i < sn; ++i)
                                tmp.pop_back();
                            return;
                        };
                        func(0);
                    }
                if (i == 12)
                    l = 0;
            }
        }
    }

    // 四帯二 or 航天飞机
    {
        int sl = 0, sn = 0, mn = 100;
        bool flag = false;
        for (auto &i : lst)
        {
            if (i.second == 4)
            {
                flag = true;
                ++sl;
                mn = min(mn, i.first);
            }
            else
                sn = i.second;
        }
        if (flag)
        {
            int l = 0;
            for (int i = 12; i > mn; --i)
            {
                if (card[i].size() >= 4)
                    ++l;
                else
                    l = 0;
                if (l >= sl)
                {
                    vector<int> tmp, canuse;
                    for (int j = 0; j < sl; ++j)
                        tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]), tmp.push_back(card[i + j][2]), tmp.push_back(card[i + j][3]);
                    for (int j = 0; j < i; ++j)
                        if (card[j].size() >= sn)
                            canuse.push_back(j);
                    for (int j = i + sl; j < 15; ++j)
                        if (card[j].size() >= sn)
                            canuse.push_back(j);
                    function<void(int)> func = [&](int pos) {
                        if (tmp.size() == 4 * sl + sn * sl * 2)
                        {
                            ans.push_back(tmp);
                            return;
                        }
                        if ((canuse.size() - pos) * sn + tmp.size() < 4 * sl + sn * sl * 2)
                            return;
                        func(pos + 1);
                        for (int i = 0; i < sn; ++i)
                            tmp.push_back(card[canuse[pos]][i]);
                        func(pos + 1);
                        for (int i = 0; i < sn; ++i)
                            tmp.pop_back();
                        return;
                    };
                    func(0);
                }
                if (i == 12)
                    l = 0;
            }
        }
        else if (lst.size() == 0)
        {
            int l = 0;
            for (int i = 12; i >= 0; --i)
            {
                if (card[i].size() >= 4)
                    ++l;
                else
                    l = 0;
                for (sl=1;sl<=l;++sl)
                for (sn=((sl==1)?1:0);sn<=2;++sn)
                {
                    vector<int> tmp, canuse;
                    for (int j = 0; j < sl; ++j)
                        tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]), tmp.push_back(card[i + j][2]), tmp.push_back(card[i + j][3]);
                    for (int j = 0; j < i; ++j)
                        if (card[j].size() >= sn)
                            canuse.push_back(j);
                    for (int j = i + sl; j < 15; ++j)
                        if (card[j].size() >= sn)
                            canuse.push_back(j);
                    function<void(int)> func = [&](int pos) {
                        if (tmp.size() == 4 * sl + sn * sl * 2)
                        {
                            ans.push_back(tmp);
                            return;
                        }
                        if ((canuse.size() - pos) * sn + tmp.size() < 4 * sl + sn * sl * 2)
                            return;
                        func(pos + 1);
                        for (int i = 0; i < sn; ++i)
                            tmp.push_back(card[canuse[pos]][i]);
                        func(pos + 1);
                        for (int i = 0; i < sn; ++i)
                            tmp.pop_back();
                        return;
                    };
                    func(0);
                }
                if (i == 12)
                    l = 0;
            }
        }
    }
    return ans;
}

int main()
{
    vector<int> cd;
    for (int i=0;i<54;++i) cd.push_back(i);
    auto ans = getoperation(cd,vector<vector<int>>());
    fprintf(stderr,"%d",ans.size());
    for (auto &i:ans){
        for(auto x:i){
            printf("%d ",x);
        }
        puts("");
    }
    return 0;
}