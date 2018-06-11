#include<cmath>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;
const int maxn=1100;

int n;
double d;
struct rec{
    double l,r;
    rec(double a=0,double b=0):l(a),r(b){};
    bool operator < (const rec& a)const{
        return r<a.r;
    }
}sav[maxn];

int main(){
    int cas=0;
    for(;;){
        ++cas;
        cin>>n>>d;
        if (n==0) break;
        bool fl=false;
        for (int i=0;i<n;++i){
            double x,y,r;
            cin>>x>>y;
            if (y>d) fl=true;
            else sav[i]=rec(x-sqrt(d*d-y*y),x+sqrt(d*d-y*y));
        }
        if (fl){
            cout<<"Case "<<cas<<": "<<-1<<endl;
            continue;
        }
        sort(sav,sav+n);
        double lst=-1e30;
        int ct=0;
        for (int i=0;i<n;++i){
            if (sav[i].l>lst+1e-10){
                ++ct;
                lst=sav[i].r;
            }
        }
        cout<<"Case "<<cas<<": "<<ct<<endl;
    }
    return 0;
}