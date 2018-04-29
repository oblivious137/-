#pragma once
#include "headquarter_declaration.h"
#include "samurai_declaration.h"
#include "weapon_declaration.h"

int Sword::attack(Samurai *a, Samurai *b)
{
    int ret = 0;
    int tmp = a->getAtk() + getAtk();
    b->hurted(tmp);
    if (tmp > 0)
        ret |= 1;
    setAtk(getAtk() * worn_ratio + 1e-8);
    ret |= 4;
    return ret;
}
Weapon *Sword::generate(Samurai *w, double _atk, double _worn)
{
    return new Sword("sword", _atk*w->getAtk()+1e-8, _worn);
}

int Bomb::attack(Samurai *a, Samurai *b)
{
    if (times <= 0)
        return 4;
    int ret = 0;
    if (a->getHP())
        ret |= 2;
    if (b->getHP())
        ret |= 1;
    a->hurted(a->getHP());
    b->hurted(b->getHP());
    --times;
    ret |= 4;
    return ret;
}
Weapon *Bomb::generate(Samurai *w, int _limit)
{
    return new Bomb("bomb", 1, _limit);
}

int Arrow::attack(Samurai *a, Samurai *b)
{
    if (times <= 0)
        return 4;
    int ret = 0;
    if (b->getHP() > 0 && getAtk() > 0)
        ret |= 1;
    b->hurted(getAtk());
    --times;
    ret |= 4;
    return ret;
}
Weapon *Arrow::generate(Samurai *w, int _atk, int _limit)
{
    return new Arrow("arrow", _atk, _limit);
}