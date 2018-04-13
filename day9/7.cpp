#include<set>
#include<cmath>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

multiset<pair<int,int> > s;

int ca(int a){
    int ret=0;
    for (int i=2;i<=sqrt(a)+1;++i){
        if (a%i==0){
            ++ret;
            while(a%i==0){
                a/=i;
            }
        }
    }
    if (a>1&&ret) ++ret;
    return ret;
}

int main(){
    int n;
    scanf("%d",&n);
    while(n--){
        for (int i=0;i<10;++i){
            int a;
            scanf("%d",&a);
            s.insert(make_pair(ca(a),a));
        }
        printf("%d %d\n",s.rbegin()->second,s.begin()->second);
        s.erase(s.begin());
        set<pair<int,int> >::iterator it;
        it=s.end();
        --it;
        s.erase(it);
    }
}