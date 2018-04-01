#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;
const int MAX = 110;
class CHugeInt
{
  private:
    static const int MAXLENGTH = 210;
    int w[MAXLENGTH];
    int length;
    void repair(int l = 0, int r = MAXLENGTH - 1)
    {
        for (int i = 0; i < r; ++i)
        {
            if (w[i] >= 10)
            {
                w[i + 1] += w[i] / 10;
                w[i] %= 10;
            }
        }
        for (length = r;length>=1;--length){
            if (w[length]!=0) break;
        }
        return;
    }

  public:
    CHugeInt(int x)
    {
        memset(w,0,sizeof(w));
        w[0]=x;
        repair();
    }
    CHugeInt(const char *s){
        memset(w,0,sizeof(w));
        int tmp=strlen(s);
        for (int i=0;i<tmp;++i) w[tmp-i-1]=s[i]-'0';
        repair();
    }
    friend ostream& operator << (ostream& out, const CHugeInt& x){
        for (int i=x.length;i>=0;--i) out<<x.w[i];
        return out<<flush;
    }
    friend CHugeInt operator + (const CHugeInt& x, const CHugeInt& y){
        CHugeInt ret=y;
        for (int i=0;i<=ret.length||i<=x.length;++i){
            ret.w[i]+=x.w[i];
        }
        ret.repair();
        return ret;
    }
    CHugeInt& operator += (const CHugeInt& x){
        *this = *this + x;
        return *this;
    }
    CHugeInt& operator ++(){
        *this+=1;
        return *this;
    }
    CHugeInt operator ++(int){
        CHugeInt tmp = *this;
        *this+=1;
        return tmp;
    }
};

int main()
{
    char s[210];
    int n;

    while (cin >> s >> n)
    {
        CHugeInt a(s);
        CHugeInt b(n);

        cout << a + b << endl;
        cout << n + a << endl;
        cout << a + n << endl;
        b += n;
        cout << ++b << endl;
        cout << b++ << endl;
        cout << b << endl;
        cout<<flush;
    }
    return 0;
}