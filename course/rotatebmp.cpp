#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include "img_bmp.hpp"
using namespace std;

int main(int argc, const char* args[])
{
    if (argc!=3){
        cerr<<"rotatebmp src.bmp dest.bmp"<<endl;
        return 0;
    }
    BMP::Image a;
    a.load(args[1]);
    a.rotate();
    a.write(args[2]);
    return 0;
}