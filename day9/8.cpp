#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <set>
using namespace std;

template<class T>
struct MyIterator{
	T t;
	MyIterator(const T &x):t(x){};
	static string change(string x){return x;}
	template<typename TT>
	static string change(TT x){return to_string(x);}
	void operator ++ (){
		++t;
		return;
	}
	string operator * (){
		return "("+change(t->first)+","+change(t->second)+")";
	}
	bool operator != (MyIterator a) const{
		return t!=a.t;
	}
	operator T (){
		return t;
	}
};

template<class T1,class T2, class CMP = greater<T1> >
class MyMultimap{
	multimap<T1,T2, CMP> data;
	typename multimap<T1,T2, CMP>::iterator xxxxxx;
    public:
	typedef typename multimap<T1,T2,CMP>::iterator iterator;
	typedef MyIterator<iterator> Iterator;
	void insert(pair<T1,T2> x){
		data.insert(x);
	}
    void Set(T1 a,T2 b){
        auto it = data.lower_bound(a);
        while(it!=data.end()&&it->first==a) (it++)->second=b;
    }
	Iterator begin(){
		return Iterator(data.begin());
	}
	Iterator end(){
		return Iterator(data.end());
	}
	Iterator find(T1 x){return Iterator(data.find(x));}
	void clear(){data.clear();}
};

struct Student 
{
	string name;
	int score;
};
template <class T>
void Print(T first,T last) {
	for(;first!= last; ++ first)
		cout << * first << ",";
	cout << endl;
}
int main()
{
	Student s[] = { {"Tom",80},{"Jack",70},
					{"Jone",90},{"Tom",70},{"Alice",100} };
	
	MyMultimap<string,int> mp;
	for(int i = 0; i<5; ++ i)
		mp.insert(make_pair(s[i].name,s[i].score));
	Print(mp.begin(),mp.end()); //按姓名从大到小输出

	mp.Set("Tom",78); //把所有名为"Tom"的学生的成绩都设置为78
	Print(mp.begin(),mp.end());
	
	
	
	MyMultimap<int,string,less<int> > mp2;
	for(int i = 0; i<5; ++ i) 
		mp2.insert(make_pair(s[i].score,s[i].name));
	
	Print(mp2.begin(),mp2.end()); //按成绩从小到大输出
	mp2.Set(70,"Error");          //把所有成绩为70的学生，名字都改为"Error"
	Print(mp2.begin(),mp2.end());
	cout << "******" << endl;
	
	mp.clear();
	
	string name;
	string cmd;
	int score;		
	while(cin >> cmd ) {
		if( cmd == "A") {
			cin >> name >> score;
			if(mp.find(name) != mp.end() ) {
				cout << "erroe" << endl;
			}
			mp.insert(make_pair(name,score));
		}
		else if(cmd == "Q") {
			cin >> name;
			MyMultimap<string,int>::iterator p = mp.find(name);
			if( p!= mp.end()) {
				cout << p->second << endl;
			}
			else {
				cout << "Not Found" << endl; 
			}		
		}
	}
	return 0;
}