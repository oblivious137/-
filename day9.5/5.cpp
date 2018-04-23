#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
using namespace std;
class MyClass {	
public:
  int n;
  MyClass(int m):n(m) { }
	void f() { cout << n << " func." << endl; }
};

#include <typeindex>

class functype{
    public:
    virtual void del(void*) = 0;
    virtual void* cpy(void*) = 0;
    virtual functype* getanother() = 0;
};

template<typename T>
class function:public functype{
    public:
    void del(void* x){
        if (x!=NULL) delete ((T*)x);
    };
    void* cpy(void* x){
        if (x==NULL) return NULL;
        return new T(*((T*)x));
    }
    functype* getanother(){
        return new function<T>();
    }
};

class MyAny{
    static const type_index _init;
    void* pos;
    functype *func;
    type_index info;
    public:
    MyAny():pos(NULL),func(NULL),info(_init){};
    template<typename T>
    MyAny(const T &x):info(typeid(T)){
        pos = new T(x);
        func = new function<T>();
    }
    MyAny(const MyAny &x):func(x.func->getanother()),info(x.info){
        pos=func->cpy(x.pos);
    }
    bool check(const type_index &t)const{return t==info;}
    void* getp()const{ return pos; }
    void reset(){
        if (pos!=NULL&&func!=NULL) func->del(pos);
        pos=NULL;
        delete func;
        func = NULL;
        info = _init;
    }
    MyAny& operator = (const MyAny& x){
        reset();
        func=x.func->getanother();
        pos=func->cpy(x.pos);
        info=x.info;
        return *this;
    }
    template<typename T>
    MyAny& operator = (const T& x){
        reset();
        pos = new T(x);
        func = new function<T>();
        info = typeid(T);
        return *this;
    }
    ~MyAny(){
        reset();
    }
};
const type_index MyAny::_init(typeid(int));

template<typename T>
T&& MyAny_cast(const MyAny& x){
    return move(*((T*)x.getp()));
}

template<typename T>
T* MyAny_cast(MyAny *x){
    if (x->check(type_index(typeid(T)))) return (T*)(x->getp());
    return NULL;
}

int main() 
{
	while(true) {
		int n;
		string s;
		cin >>  n >> s;
		if ( n == 0)
			break;
		MyAny a = n;
	 	cout << MyAny_cast<int>(a) << endl;
		a = s;
		cout << MyAny_cast<string>(a) << endl;
		a = MyClass(n+1);
		MyAny b = n + 2;
		MyAny * p = new MyAny(MyClass(n+3));
		MyAny c = MyClass(n+4);
	 	c = * p;
	  	b = * p;
		delete p;
		MyAny d = b;
		MyAny_cast<MyClass>(&a)->f();
		MyAny_cast<MyClass>(&b)->f();	
		MyAny_cast<MyClass>(&c)->f();	
		MyAny_cast<MyClass>(&d)->f();		
		c = s + "OK";
		cout << MyAny_cast<string>(c) << endl;	
		int * pi = MyAny_cast<int> ( & c);
		if( pi == NULL)
			cout << "not a int" << endl;
		else
			cout << "the int is " << * pi << endl;
		string * ps = MyAny_cast<string> ( & c);
		if( ps == NULL)
			cout << "not a string" << endl;
		else
			cout << "the string is " << * ps << endl;
	}
}