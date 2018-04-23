#include <iostream>

using namespace std;
//#define MySharedPtr shared_ptr

template <class T> 
struct MySharedPtr
{
    int *count;
    T* addr;
    public:
    MySharedPtr():count(NULL),addr(NULL){};
    MySharedPtr(T* x):addr(x),count(new int(1)){};
    MySharedPtr(const MySharedPtr<T> &x):addr(x.addr),count(x.count){
        if (count) ++(*count);
    }
    operator bool (){
        return addr!=NULL;
    }
    T* operator -> (){return addr;}
    T& operator * (){return *addr;}
    T* get(){return addr;}
    void reset(){
        if (count && addr){
            --(*count);
            if (*count == 0){
                delete addr;
                delete count;
            }
        }
        addr=NULL;
        count=NULL;
    }
    void reset(T* p){
        reset();
        addr=p;
        count = new int(1);
    }
    MySharedPtr<T>& operator = (const MySharedPtr<T> &x){
        //puts("zzzzzz");
        reset();
        count = x.count;
        if (count) ++(*count);
        addr = x.addr;
        return *this;
    }
    ~MySharedPtr(){
        reset();
    }
};

struct A   {    
 	 int n;
	 A(int v = 0):n(v){ } 
	 ~A() { cout << n << " destructor" << endl; }
};
int main()
{
	MySharedPtr<A> sp1(new A(2)); 
	MySharedPtr<A> sp2(sp1);
	cout << "1)" << sp1->n  << "," << sp2->n << endl; 
	MySharedPtr<A> sp3;
	A * p = sp1.get();
	cout << "2)" << p->n << endl;
	sp3 = sp1;
	cout << "3)" << (*sp3).n << endl;  
	sp1.reset();
	if( !sp1 )
		cout << "4)sp1 is null" << endl; //会输出 
	A * q = new A(3);
	sp1.reset(q); // 
	cout << "5)" << sp1->n << endl;
	MySharedPtr<A> sp4(sp1); 
	MySharedPtr<A> sp5;
	sp5.reset(new A(8));
	sp1.reset();          
	cout << "before end main" <<endl;
	sp4.reset();          
	cout << "end main" << endl;
	return 0; //程序结束，会delete 掉A(2)
}