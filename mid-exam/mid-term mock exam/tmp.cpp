#include<iostream>
using namespace std;

class A{
    public:
    virtual void f(){
        cout<<"A"<<endl;
    }
    void lala(){
        f();
    }
};

class B:public A{
    public:
    void f(){
        cout<<"B"<<endl;
    }
};

int main(){
    A* p = new B();
    p->lala();
    return 0;
}