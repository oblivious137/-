#pragma once
#include "headquarter_declaration.h"
#include "samurai_declaration.h"
#include "weapon_declaration.h"

void Sword::attack(Samurai* a, Samurai*b){
    b->hurted(a->getAtk()*0.2);
    
}