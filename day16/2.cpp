#include<queue>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;
const int maxn=110;

char mp[maxn][maxn];
int lable[maxn][maxn];
struct state{
    int k,dis,x,y,sta;
    bool sn;
    state(int xx,int yy,int kk,int s,int d,bool f):k(kk),dis(d),x(xx),y(yy),sta(s),sn(f){};
};
bool mark[maxn][maxn][11][1<<5];

queue<state> q;
constexpr int mov[4][2]={{0,1},{0,-1},{1,0},{-1,0}};

int main(){
    int n,m;
    for(;;){
        cin>>n>>m;
        if (n==0) break;
        while(!q.empty()) q.pop();
        memset(mark,0,sizeof(mark));
        int ct=0;
        for (int i=0;i<n;++i){
            cin>>mp[i];
            for (int j=0;j<n;++j) if (mp[i][j]=='K'){
                q.push(state(i,j,0,0,0,false));
                mark[i][j][0][0]=true;
            }
            else if (mp[i][j]=='S') lable[i][j]=ct++;
        }
        bool fl=false;
        while(!q.empty()&&!fl){
            state now = q.front();
            q.pop();
            if (now.sn){
                now.sn=false;
                ++now.dis;
                q.push(now);
                continue;
            }
            for (int i=0;i<4;++i){
                int x=now.x+mov[i][0],y=now.y+mov[i][1];
                if (x>=0&&x<n&&y>=0&&y<n&&mp[x][y]!='#'){
                    state tmp(x,y,now.k,now.sta,now.dis+1,false);
                    if (mp[x][y]=='0'+now.k+1) ++tmp.k;
                    if (mp[x][y]=='S'){
                        if (~tmp.sta&(1<<lable[x][y])){
                            tmp.sta|=1<<lable[x][y];
                            tmp.sn=true;
                        }
                    }
                    if (!mark[x][y][tmp.k][tmp.sta]){
                        mark[x][y][tmp.k][tmp.sta]=true;
                        if (mp[x][y]=='T'&&tmp.k==m){
                            fl=true;
                            cout<<tmp.dis<<endl;
                            break;
                        }
                        q.push(tmp);
                    }
                }
            }
        }
        if (!fl) puts("impossible");
    }
    return 0;
}