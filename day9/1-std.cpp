#include<vector>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

vector<long long> s[10010];

int main(){
    //ios::sync_with_stdio(false);
    int cas=0;
    cin>>cas;
    while(cas--){
        string opt;
        cin>>opt;
        if (opt == "new"){
            int id;
            cin>>id;
            s[id].clear();
        }
        else if (opt == "add"){
            int id;
            long long num;
            cin>>id>>num;
            s[id].push_back(num);
        }
        else if (opt == "merge"){
            int id1,id2;
            cin>>id1>>id2;
            for (auto x:s[id2]) s[id1].push_back(x);
            s[id2].clear();
        }
        else if (opt == "unique"){
            int id;
            cin>>id;
            sort(s[id].begin(),s[id].end());
            s[id].erase(unique(s[id].begin(),s[id].end()),s[id].end());
        }
        else if (opt == "out"){
            int id;
            cin>>id;
            sort(s[id].begin(),s[id].end());
            bool fl=false;
            for (auto x:s[id]){
                if (fl) cout<<' ';
                cout<<x;
                fl=true;
            }
            cout<<endl;
        }
    }
    return 0;
}