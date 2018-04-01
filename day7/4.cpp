#include <iostream>
#include <string>
#include <cstring>
using namespace std;
template <class T>  
class myclass {
	T* p;
	int size;
public:
	myclass(T* src, int l):size(l){
		p = new T[l];
		memcpy(p, src, sizeof(T)*l);
	}
	~myclass( ) {
		delete [] p;
	}
	void Show()
	{
		for( int i = 0;i < size;i ++ ) {
			cout << p[i] << ",";
		}
		cout << endl;
	}
};
int a[100];
int main() {
	char line[100];
	while( cin >> line ) {
		myclass<char> obj(line,strlen(line));
		obj.Show();
		int n;
		cin >> n;
		for(int i = 0;i < n; ++i)
			cin >> a[i];
		myclass<int> obj2(a,n);
		obj2.Show();
	}
	return 0;
}
