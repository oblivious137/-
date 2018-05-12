#include <iostream>
using namespace std;

class CType
{
    int num;

  public:
    void setvalue(int x) { num = x; }
    int operator++(int)
    {
        int tmp = num;
        num *= num;
        return tmp;
    }
    friend ostream &operator<<(ostream &out, const CType &x)
    {
        out << x.num;
        return out;
    }
};

int main(int argc, char *argv[])
{
    CType obj;
    int n;
    cin >> n;
    while (n)
    {
        obj.setvalue(n);
        cout << obj++ << " " << obj << endl;
        cin >> n;
    }
    return 0;
}