#pragma once
#include "headquarter_declaration.h"
#include "samurai_declaration.h"
#include "weapon_declaration.h"

Samurai::Samurai(Headquarter *_B, int _number, int _HP, int _Atk) : belong(_B), Number(_number), HealthPoint(_HP), Attack(_Atk){};
int Samurai::get_direct()const
{
    return belong->get_direct();
}
Headquarter *Samurai::get_belong()const
{
    return belong;
}
bool Samurai::addweapon(Weapon* x){
	return bag.push(x);
}

Dragon::Dragon(Headquarter *_B, int _number, int _HP, int _Atk, double _morale) : Samurai(_B, _number, _HP, _Atk), morale(_morale){};
string Dragon::getinfo() const
{
	stringstream tmp;
	tmp << "It has a " ;//<< weapon->getname() << ",and it's morale is " << fixed << setprecision(2) << morale;
	return tmp.str();
}
Samurai *Dragon::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	Samurai* ret=new Dragon(info, cnt, getHP(), getAtk(), (double)info->getHP() / getHP() - 1);
	ret->addweapon(info->getweapon(cnt % 3));
	return ret;
}
string Dragon::yelled(){
	return getfullname()+" yelled in city " + to_string(getpos())+'\n';
}
Dragon::~Dragon()
{
}

Ninja::Ninja(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
string Ninja::getinfo() const
{
	string ret = "It has";
	// if (weapon[0])
	// 	ret += " a " + weapon[0]->getname();
	// if (weapon[1])
	// 	ret += " and a " + weapon[1]->getname();
	return ret;
}
Samurai *Ninja::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	Samurai* ret = new Ninja(info, cnt, getHP(), getAtk() );
	ret->addweapon(info->getweapon(cnt % 3));
	ret->addweapon(info->getweapon((cnt + 1) % 3));
	return ret;
}
Ninja::~Ninja(){
	
}

Iceman::Iceman(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
string Iceman::getinfo() const
{
	return "It has a ";// + ((weapon != NULL) ? weapon->getname() : "");
}
Samurai *Iceman::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	Samurai* ret= new Iceman(info, cnt, getHP(), getAtk());
	ret->addweapon(info->getweapon(cnt % 3));
	return ret;
}
Iceman::~Iceman()
{
}

Lion::Lion(Headquarter *_B, int _number, int _HP, int _Atk, int _loyalty, int _LDec) : Samurai(_B, _number, _HP, _Atk), loyalty(_loyalty), LDec(_LDec){};
string Lion::getinfo() const
{
	return "It's loyalty is " + to_string(loyalty);
}
Samurai *Lion::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;

	Samurai* ret= new Lion(info, cnt, getHP(), getAtk(), info->getHP() - getHP(), LDec);
	ret->addweapon(info->getweapon(cnt % 3));
	return ret;
}
string Lion::escape(){
	if (loyalty<=0) return "";
	else return get_belong()->getname()+' '+getname()+' '+to_string(getnumber()) + " ran away";
}

Wolf::Wolf(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
string Wolf::getinfo() const
{
	return "";
}
Samurai *Wolf::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	return new Wolf(info, cnt, getHP(), getAtk());
}
string Wolf::rob(Samurai* x){
	if (x->getname()=="wolf") return "";
	WeaponBag &tmp = x->getbag();
	tmp.preliminary([](Weapon*a,Weapon*b) -> bool{return !Weapon::CMP(a,b);});
	int t=0;
	Weapon* first = NULL;
	for (;t<tmp.size();++t){
		Weapon* now = tmp.next();
		if (first==NULL) first=now;
		if (now->getnumber()!=first->getnumber()) break;
		if (!addweapon(now)) break;
	}
	tmp.pop_top(t);
	if (t==0) return "";
	return getfullname() + " took " + to_string(t) + ' ' +first->getname() +" from " + x->getfullname();
}