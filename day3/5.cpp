#include <iostream>
#include <cstring>
using namespace std;

class Array2 {
    int n,m;
    int **p;
    public:
    Array2 (int a=0,int b=0):n(a),m(b){
        p = new int*[n];
        for (int i=0;i<n;++i) p[i]=new int[m];
    }
    int* operator [] (int a){
        return p[a];
    }
    int& operator () (int a,int b){
        return p[a][b];
    }
};

int main() {
    Array2 a(3,4);
    int i,j;
    for(  i = 0;i < 3; ++i )
        for(  j = 0; j < 4; j ++ )
            a[i][j] = i * 4 + j;
    for(  i = 0;i < 3; ++i ) {
        for(  j = 0; j < 4; j ++ ) {
            cout << a(i,j) << ",";
        }
        cout << endl;
    }
    cout << "next" << endl;
    Array2 b;     b = a;
    for(  i = 0;i < 3; ++i ) {
        for(  j = 0; j < 4; j ++ ) {
            cout << b[i][j] << ",";
        }
        cout << endl;
    }
    return 0;
}