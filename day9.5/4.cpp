#include <iostream>
#include <cstring>
using namespace std;
template <int bitNum>
struct MyBitset 
{
	char a[bitNum/8+1];
	MyBitset() { memset(a,0,sizeof(a));};
	void Set(int i,int v) {
		char & c = a[i/8];
		int bp = i % 8;
		if( v ) 
			c |= (1 << bp);
		else 
			c &= ~(1 << bp);
	}
    class BitOperator{
        char *addr;
        int pos;
        public:
        BitOperator(char* _addr, int _pos):addr(_addr),pos(_pos){};
        BitOperator& operator = (int v){
            if (v) (*addr)|=1<<pos;
            else (*addr)&=(~(1<<pos));
        }
        BitOperator& operator = (const BitOperator &v){
            if (v) (*addr)|=1<<pos;
            else (*addr)&=(~(1<<pos));
            return *this;
        }
        operator bool ()const{
            return (*addr)&(1<<pos);
        }
        friend ostream& operator << (ostream& out, const BitOperator &x){out<<(x?1:0);}
    };
    BitOperator operator [] (int x){
        return BitOperator(a+x/8,x%8);
    }
void Print() {
		for(int i = 0;i < bitNum; ++i) 
			cout << (*this) [i];
		cout << endl;
	}
};

int main()
{
	int n;
	int i,j,k,v;
	while( cin >>  n) {
		MyBitset<20> bs;
		for(int i = 0;i < n; ++i) {
			int t;
			cin >> t;
			bs.Set(t,1);
		}
		bs.Print();
		cin >> i >> j >> k >> v;
		bs[k] = v;
		bs[i] = bs[j] = bs[k];
		bs.Print();
		cin >> i >> j >> k >> v;
		bs[k] = v;
		(bs[i] = bs[j]) = bs[k];
		bs.Print();
	}
	return 0;
}