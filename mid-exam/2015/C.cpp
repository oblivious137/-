#include <iostream>
using namespace std;

long long sum(int *a, int n, int (*f)(int))
{
    long long ret = 0;
    for (int i = 0; i < n; ++i)
        ret += f(a[i]);
    return ret;
}

int sqr(int n)
{
    return n * n;
}
int main()
{
    int t, n, a[0x100];
    cin >> t;
    for (int c = 0; c < t; ++c)
    {
        cin >> n;
        for (int i = 0; i < n; ++i)
            cin >> a[i];
        cout << sum(a, n, sqr) << endl;
    }
    return 0;
}
