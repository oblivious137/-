#pragma once
#include "headquarter_declaration.h"
#include "samurai_declaration.h"
#include "weapon_declaration.h"

int Sword::attack(Samurai *a, Samurai *b)
{
    int tmp = getratio()*(a->getAtk());
    b->hurted(tmp);
    if (tmp>0) return 1;
    else return 0;
}

int Bomb::attack(Samurai *a, Samurai *b){
    if (times<=0) return 4;
    --times;
    int tmp = getratio()*(a->getAtk());
    int ret=4;
	if (a->getname() != "ninja") {
		a->hurted(tmp*0.5);
		if (tmp>1) ret |= 2;
	}
    b->hurted(tmp);
    if (tmp>0) ret|=1;
    return ret;
}

int Arrow::attack(Samurai *a, Samurai *b){
    if (times<=0) return 4;
    --times;
    int tmp = getratio()*(a->getAtk());
    b->hurted(tmp);
    if (tmp>0) return 5;
    else return 4;
}