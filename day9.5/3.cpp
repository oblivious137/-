#include <iostream>
using namespace std;
// 在此处补充你的代码
#include<functional>

template <typename T1, typename T2, typename res>
auto combine(T1 a,T2 b) -> std::function<res(res)>{
    return [=](res x)->res{return a(a(x)+b(x));};
}
/*
double(*comb(double(*a)(double), double (*b)(double)))(double){
	return [](double x)->double{return a(a(x)+b(x));};
}
*/
int main()
{
    auto Square = [] (double a) { return a * a; };
    auto Inc = [] (double a) { return a + 1; };
    cout << combine<decltype(Square),decltype(Inc),int>(Square,Inc)(3) << endl;
    cout << combine<decltype(Inc),decltype(Square),double>(Inc,Square)(2.5) << endl;

    return 0;
}
