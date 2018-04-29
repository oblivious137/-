#pragma once
#include "headquarter_declaration.h"
#include "samurai_declaration.h"
#include "weapon_declaration.h"

Samurai::Samurai(Headquarter *_B, int _number, int _HP, int _Atk) : belong(_B), Number(_number), HealthPoint(_HP), Attack(_Atk) {};
string Samurai::getfullname() const { return belong->getname() + ' ' + getname() + ' ' + to_string(Number); }
int Samurai::get_outputlevel() const { return belong->get_outputlevel() * 100 + getpos(); }
int Samurai::get_routputlevel() const { return getpos() * 100 + belong->get_outputlevel(); }
int Samurai::get_direct() const
{
	return belong->get_direct();
}
Headquarter *Samurai::get_belong() const
{
	return belong;
}
bool Samurai::addweapon(Weapon *x)
{
	return bag.push(x);
}
string Samurai::fightwin(Samurai *x)
{
	belong->askforHP(make_tuple(Position, this, 8));
	return "";
}

Dragon::Dragon(Headquarter *_B, int _number, int _HP, int _Atk, double _morale) : Samurai(_B, _number, _HP, _Atk), morale(_morale){};
Samurai *Dragon::generate(Headquarter *info, int _HP, int _Atk)
{
	if (info->getHP() < _HP)
		return NULL;
	int cnt = info->generateNextSamurai(_HP);
	Samurai *ret = new Dragon(info, cnt, _HP, _Atk, (double)info->getHP() / _HP);
	ret->addweapon(info->getweapon(cnt % 3, ret));
	ret->setpos(info->get_pos());
	return ret;
}
string Dragon::borninfo() const
{
	static char tmp[30];
	sprintf(tmp, "Its morale is %.2lf", morale + 1e-8);
	return tmp;
}
string Dragon::yelled()
{
	if (morale > 0.8)
		return getfullname() + " yelled in city " + to_string(getpos());
	else
		return "";
}
string Dragon::fightwin(Samurai * a)
{
	Samurai::fightwin(a);
	morale += fight_morale;
	return "";
}
string Dragon::fightdraw(Samurai * a , int x)
{
	//Samurai::fightdraw(a, x);
	if (x == 1)
		morale -= fight_morale;
	return "";
}

Ninja::Ninja(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
Samurai *Ninja::generate(Headquarter *info, int _HP, int _Atk)
{
	if (info->getHP() < _HP)
		return NULL;
	int cnt = info->generateNextSamurai(_HP);
	Samurai *ret = new Ninja(info, cnt, _HP, _Atk);
	ret->addweapon(info->getweapon(cnt % 3, ret));
	ret->addweapon(info->getweapon((cnt + 1) % 3, ret));
	ret->setpos(info->get_pos());
	return ret;
}

Iceman::Iceman(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
Samurai *Iceman::generate(Headquarter *info, int _HP, int _Atk)
{
	if (info->getHP() < _HP)
		return NULL;
	int cnt = info->generateNextSamurai(_HP);
	Samurai *ret = new Iceman(info, cnt, _HP, _Atk);
	ret->addweapon(info->getweapon(cnt % 3, ret));
	ret->setpos(info->get_pos());
	return ret;
}
void Iceman::moveeffect()
{
	++step_count;
	if (step_count % 2 == 0)
	{
		hurted(min(9, getHP() - 1));
		setAtk(getAtk() + 20);
	}
}

Lion::Lion(Headquarter *_B, int _number, int _HP, int _Atk, int _loyalty, int _LDec) : Samurai(_B, _number, _HP, _Atk), loyalty(_loyalty), LDec(_LDec){};
string Lion::borninfo() const
{
	return "Its loyalty is " + to_string(loyalty);
}
Samurai *Lion::generate(Headquarter *info, int _HP, int _Atk, int _LDec)
{
	if (info->getHP() < _HP)
		return NULL;
	int cnt = info->generateNextSamurai(_HP);
	Samurai *ret = new Lion(info, cnt, _HP, _Atk, info->getHP(), _LDec);
	ret->setpos(info->get_pos());
	return ret;
}
string Lion::escape()
{
	if (loyalty > 0)
		return "";
	else
		return getfullname() + " ran away";
}
string Lion::fightlose(Samurai *x)
{
	x->hurted(-lastHP);
	return "";
}
string Lion::fightdraw(Samurai *, int x)
{
	if (x == 1)
		changeloyalty();
	return "";
}

Wolf::Wolf(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
Samurai *Wolf::generate(Headquarter *info, int _HP, int _Atk)
{
	if (info->getHP() < _HP)
		return NULL;
	int cnt = info->generateNextSamurai(_HP);
	Samurai* ret = new Wolf(info, cnt, _HP, _Atk);
	ret->setpos(info->get_pos());
	return ret;
}
string Wolf::rob(Samurai *x)
{
	WeaponBag &tmp = x->getbag();
	tmp.preliminary(Weapon::RobCMP);
	for (int t = 0; t < tmp.size(); ++t)
	{
		Weapon *now = tmp.next();
		if (addweapon(now))
		{
			tmp.pop(t);
			--t;
		}
	}
	return "";
}
string Wolf::fightwin(Samurai *x)
{
	Samurai::fightwin(x);
	rob(x);
	return "";
}