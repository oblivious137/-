#include<queue>
#include<cmath>
#include<stack>
#include<cstdio>
#include<cstring>
#include<sstream>
#include<iostream>
#include<algorithm>
using namespace std;

priority_queue<double, vector<double>, greater<double>> res;
stack<double> dig;

bool check(const string &x){
    for (auto c:x){
        if (c>='0'&&c<='9') return true;
    }
    return false;
}

bool getnext(string& x){
    while(getline(cin,x)){
        if (check(x)) return true;
    }
    return false;
}

int main(){
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;++i){
        double x;
        scanf("%lf",&x);
        res.push(x);
    }
    string s;
    for(;;){
        while(!dig.empty()) dig.pop();
        if (!getnext(s)) break;
        // cout<<s<<endl;
        s=s.substr(s.find_first_not_of(" \n\r"),s.find_last_not_of(" \n\r")-s.find_first_not_of(" \n\r")+1);
        for (int st=0;st<s.length();){
            // printf("st=%d\n",st);
            int ed=st;
            while(ed<s.length()&&s[ed]!=' ') ++ed;
            // printf("ed=%d\n",ed);
            // cout<<s.substr(st,ed-st)<<endl;
            if (check(s.substr(st,ed-st))){
                stringstream tmp(s.substr(st,ed-st));
                double x;
                tmp>>x;
                dig.push(x);
            }
            else{
                if (s[st]=='=') break;
                double b=dig.top();
                dig.pop();
                double a=dig.top();
                dig.pop();
                if (s[st]=='+') dig.push(a+b);
                else if (s[st]=='-') dig.push(a-b);
                else if (s[st]=='*') dig.push(a*b);
                else if (s[st]=='/') dig.push(a/b);
                else if (s[st]=='^') dig.push(pow(a,b));
            }
            st=ed;
            while(st<s.length()&&s[st]==' ') ++st;
        }
        printf("%e\n",dig.top());
        res.pop();
        res.push(dig.top());
    }
    int ct=0;
    while(!res.empty()){
        if (ct%10==0) puts("");
        else printf(" ");
        printf("%e",res.top());
        res.pop();
        ++ct;
    }
    return 0;
}