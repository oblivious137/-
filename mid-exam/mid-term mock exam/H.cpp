#include <algorithm>
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <map>
#include <set>

using namespace std;
class MyQueue
{
    int lim;
    multiset<int> q;
    public:
    MyQueue(int l):lim(l){};
    friend ostream& operator << (ostream& out, MyQueue& x){
        multiset<int>::reverse_iterator it = x.q.rbegin();
        for (int i=0;i<x.lim;++i,++it)
            out<<*it<<' ';
        return out;
    }
    friend istream& operator >> (istream& in, MyQueue& x){
        int g;
        in>>g;
        if (~g&1) x.q.insert(g);
        return in;
    }
};
int main()
{
	int t;
	cin >> t;
	while(t--) {
		int n, k;
		cin >> n >> k;
		MyQueue q(k);
		for (int i = 0; i < n; ++i)
			cin >> q;
		cout<<q;
		cout << endl;
	}
	return 0; 
}