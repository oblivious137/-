#include <iostream>
using namespace std;


#include<functional>
template <typename T1, typename T2>
auto f(T1 a) -> std::function<T2(T2)>{
    return [=](T2 x){return x+a;};
}

int main()
{
    auto x = f<int,int>(7);

   cout << f<int,int>(7)(9) << endl;   //16
   cout << f<string,string> (" hello!")("world")  <<endl; // world hello!
   cout << f<char,string> ('!')("world") << endl;
   
   
   return 0;    //world!
}