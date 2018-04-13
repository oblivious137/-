#include<map>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

map<int,int> mp;

int main(){
    int n;
    cin>>n;
    while(n--){
        int x;
        string op;
        cin>>op>>x;
        if (op=="add"){
            printf("%d\n",++mp[x]);
        }
        else if (op=="del"){
            if (mp.find(x)==mp.end()){
                printf("0\n");
            }
            else{
                printf("%d\n",mp[x]);
                mp[x]=0;
            }
        }
        else{
            map<int,int>::iterator it;
            it=mp.find(x);
            if (it==mp.end()){
                printf("0 0\n");
            }
            else{
                printf("1 %d\n",it->second);
            }
        }
    }
    return 0;
}