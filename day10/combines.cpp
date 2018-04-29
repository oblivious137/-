#include<cstdio>
#include<string>
#include<cstring>
#include<fstream>
#include<iostream>
#include<algorithm>
using namespace std;

char tmp[10000];

inline void add(ostream& out, string name){
	ifstream in(name);
	int len=-1;
	while(in.get(tmp,10000,'\0')){
		out<<tmp;
	}
	out<<endl;
}

int main(int argc, char*argv[]){
	if (argc<2) return 0;
	if (argc==2){
		ofstream out(argv[1]);
		add(out,"WarcraftClassDeclaration.h");
		add(out,"weapon_declaration.h");
		add(out,"samurai_declaration.h");
		add(out,"headquarter_declaration.h");
		add(out,"headquarter.h");
		add(out,"samurai.h");
		add(out,"weapon.h");
		add(out,"Main.cpp");
	}
	else{
		ofstream out(argv[1]);
		for (int i=2;i<argc;++i){
			string tmp = argv[i];
			// istream 
		}
	}
}