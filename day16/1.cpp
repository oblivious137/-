#include<cstdio>
#include<cstring>
#include<iostream>
#include<tuple>
#include<algorithm>
using namespace std;

int dis[10][10];
pair<int,int> fr[10][10];
bool mp[10][10];
constexpr int n=5,m=5;
constexpr int mov[4][2]={{0,1},{0,-1},{1,0},{-1,0}};

void dfs(int x,int y){
    for (int i=0;i<4;++i){
        int dx=x+mov[i][0],dy=y+mov[i][1];
        if (dx>=0&&dx<5&&dy>=0&&dy<5&&mp[dx][dy]){
            if (dis[dx][dy]>dis[x][y]+1){
                dis[dx][dy]=dis[x][y]+1;
                fr[dx][dy]=make_pair(x,y);
                dfs(dx,dy);
            }
        }
    }
    return;
}

int main(){
    int a;
    for (int i=0;i<5;++i)
    for (int j=0;j<5;++j){
        scanf("%d",&a);
        mp[i][j]=!a;
        dis[i][j]=1000;
    }
    dis[4][4]=0;
    dfs(4,4);
    int x=0,y=0;
    for(;;){
        printf("(%d, %d)\n",x,y);
        if (x==4&&y==4) break;
        std::tie(x,y)=make_tuple(fr[x][y].first,fr[x][y].second);
    }
    return 0;
}