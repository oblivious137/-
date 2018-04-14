#pragma once
#include "headquarter_declaration.h"
#include "samurai_declaration.h"
#include "weapon_declaration.h"

Samurai::Samurai(Headquarter *_B, int _number, int _HP, int _Atk) : belong(_B), Number(_number), HealthPoint(_HP), Attack(_Atk){};
string Samurai::getfullname()const { return belong->getname() + ' ' + getname() + ' ' + to_string(Number); }
int Samurai::get_outputlevel() const{ return Position * 100 + belong->get_outputlevel(); }
bool BattleFirst(Samurai *a, Samurai *b) { return (a->getpos() & 1) && (a->get_belong()->get_pos() == 0) || (~a->getpos() & 1) && (a->get_belong()->get_pos() != 0); }
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

Ninja::Ninja(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
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

Iceman::Iceman(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
Samurai *Iceman::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	Samurai* ret= new Iceman(info, cnt, getHP(), getAtk());
	ret->addweapon(info->getweapon(cnt % 3));
	return ret;
}

Lion::Lion(Headquarter *_B, int _number, int _HP, int _Atk, int _loyalty, int _LDec) : Samurai(_B, _number, _HP, _Atk), loyalty(_loyalty), LDec(_LDec){};
string Lion::borninfo() const
{
	return "Its loyalty is " + to_string(loyalty);
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

	if (loyalty>0)
		return "";
	else 
		return getfullname() + " ran away";
}

Wolf::Wolf(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
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
	tmp.preliminary(Weapon::RobCMP);
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
	return getfullname() + " took " + to_string(t) + ' ' +first->getname() +" from " + x->getfullname() + " in city " + to_string(getpos());
}