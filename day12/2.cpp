#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

int output(stringstream &out, string dirname, int dep)
{
    for (int i = 0; i < dep; ++i)
        out << "|     ";
    out << dirname << '\n';
    vector<string> file;
    for (;;)
    {
        string tmp;
        getline(cin, tmp);
        if (tmp == "]" || tmp == "*")
            break;
        if (tmp == "#")
            return -1;
        if (tmp[0] == 'f')
            file.push_back(tmp);
        else
            output(out, tmp, dep + 1);
    }
    sort(file.begin(), file.end());
    for (auto &x : file)
    {
        for (int i = 0; i < dep; ++i)
            out << "|     ";
        out << x << endl;
    }
    return 0;
}

int main()
{
    int cas = 0;
    for (;;)
    {
        ++cas;
        stringstream tmp;
        if (output(tmp, "ROOT", 0) == -1)
            break;
        cout << "DATA SET " << cas << ":\n";
        cout<<tmp.str()<<endl;
    }
    return 0;
}