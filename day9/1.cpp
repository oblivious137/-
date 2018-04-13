#include<set>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

class Array{
	struct Data{
		int key,times,dfn;
		Data(int a=0,int b=1,int t=0):key(a),times(b),dfn(t){};
		bool operator < (const Data&x)const{return key<x.key;}
		string sprint(bool multi=false)const{
			if (times <= 0) return "";
			string ret = to_string(key);
			if (multi){
				for (int i=1;i<times;++i){
					ret+=' ';
					ret+=to_string(key);
				}
			}
			return ret;
		}
		Data& inc(int ti = 1){
			times+=ti;
			return *this;
		}
		Data& update(int t,int nt){
			if (dfn<t) times = 1;
			dfn = nt;
			return *this;
		}
	};
	set<Data> *val;
	int tim, utim;
	public:
	Array()=default;
	void init(){
		tim=utim=0;
		if (val == NULL) val = new set<Data>;
		else val->clear();
	}
	void insert(int x,int cnt=1){
		++tim;
		set<Data>::iterator it = val->find(Data(x));
		if (it == val->end()) val->insert(Data(x,cnt,tim));
		else{
			Data nw = *it;
			val->erase(it);
			val->insert(nw.update(utim,tim).inc(cnt));
		}
	}
	void unique(){utim=++tim;}
	void output(){
		++tim;
		int ct = 0;
		for (auto x:*val){
			x.update(utim,tim);
			 if (ct) cout<<' ';
			cout<<x.sprint(true);
			++ct;
		}
	}
	void merge(Array& src){
		if (val->size()<src.val->size()){
			swap(val,src.val);
			swap(tim,src.tim);
			swap(utim,src.utim);
		}
		for (auto x:*src.val){
			x.update(src.utim,src.tim);
			insert(x.key,x.times);
		}
		src.init();
	}
	// void print(){
	// 	printf("tim = %d, utim = %d\n",tim,utim);
	// 	for (auto x:*val){
	// 		printf("item: key: %d, times: %d, dfn: %d\n",x.key,x.times,x.dfn);
	// 	}
	// }
}rec[10010];

int main(){
	int T=0;
	cin>>T;
	while(T--){
		string opt;
		cin>>opt;
		if (opt == "new"){
			int id;
			cin>>id;
			rec[id].init();
		}
		else if (opt == "add"){
			int id,num;
			cin>>id>>num;
			rec[id].insert(num);
		}
		else if (opt == "merge"){
			int id1,id2;
			cin>>id1>>id2;
			rec[id1].merge(rec[id2]);
		}
		else if (opt == "unique"){
			int id;
			cin>>id;
			rec[id].unique();
		}
		else if (opt == "out"){
			int id;
			cin>>id;
			rec[id].output();
			cout<<endl;
		}
		// rec[1].print();
	}
	return 0;
}