#include <iterator>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <stack>
#include <iostream>
#include <set>
using namespace std;

int main()
{
    int t;
    int a[100];
    cin >> t;
    while (t--)
    {
        for (int i = 0; i < 12; ++i)
            cin >> a[i];
        int c[100];
        vector<int> b;
        sort(a, a + 12);
        int ct = unique(a, a + 12) - a;
        for (int i = 0; i < ct; ++i)
            cout << a[i] << ' ';
        std::copy(b.begin(), b.end(), c);
        cout << endl;
    }
    return 0;
}