#include <map>
#include <stack>
#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

inline int &nextc(string &x, int &pos, int ad = 1)
{
    while (x[pos] == ' ')
        pos += ad;
    return pos;
}

inline int min_exp(string &x, map<int, int> &match, int pos, int ed)
{
    // cout << "find: " << x.substr(pos, ed - pos + 1) << endl;
    int fl1 = -1, fl2 = -1;
    for (int i = pos; i <= ed; ++i)
    {
        if (x[i] == '(')
            i = match[i];
        if (x[i] == '!')
            fl1 = i;
        else if (x[i] == '&' && fl2 < 0)
            fl2 = i;
        else if (x[i] == '|')
            return i - 1;
    }
    if (fl2 >= 0)
        return fl2 - 1;
    else
        return ed;
}
// (F&F|V|!V&!F&!(F|F&V))
char calc(string &exp, map<int, int> &match, int a, int b)
{
    // cout << "calc: " << exp.substr(a, b - a + 1) << endl;
    // getchar();
    nextc(exp, a, 1);
    nextc(exp, b, -1);
    while (exp[a] == '(' && match[a] == b)
    {
        nextc(exp, ++a, 1);
        nextc(exp, --b, -1);
    }
    if (a == b)
        return exp[a];
    char x, y;
    int op;
    if (exp[a] == '!')
    {
        op = min_exp(exp, match, a + 1, b);
        x = calc(exp, match, a + 1, op) == 'V' ? 'F' : 'V';
        if (op == b)
            return x;
    }
    else
    {
        op = min_exp(exp, match, a, b);
        x = calc(exp, match, a, op);
    }
    ++op;
    nextc(exp, op);
    int r = op + 1;
    nextc(exp, r);
    y = calc(exp, match, r, b);
    if (exp[op] == '&')
        return (x == 'V' && y == 'V') ? 'V' : 'F';
    if (exp[op] == '|')
        return (x == 'V' || y == 'V') ? 'V' : 'F';
}

int main()
{
    int cas = 0;
    string tmp;
    while (getline(cin, tmp))
    {
        ++cas;
        map<int, int> match;
        stack<int> sta;
        for (int i = 0; i <= tmp.length(); ++i)
        {
            if (tmp[i] == '(')
                sta.push(i);
            else if (tmp[i] == ')')
            {
                match[sta.top()] = i;
                sta.pop();
            }
        }
        cout << "Expression " << cas << ": " << calc(tmp, match, 0, tmp.length() - 1) << endl;
    }
    return 0;
}