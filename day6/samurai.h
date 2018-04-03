#pragma once
#include "headquarter_declaration.h"
#include "samurai_declaration.h"
#include "weapon_declaration.h"

Samurai::Samurai(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0) : belong(_B), Number(_number), HealthPoint(_HP), Attack(_Atk){};

int Samurai::get_direct()
{
    return belong->get_direct();
}

Headquarter *Samurai::get_belong()
{
    return belong;
}

Dragon::Dragon(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, double _morale = 0, Weapon *_weapon = NULL) : Samurai(_B, _number, _HP, _Atk), morale(_morale), weapon(_weapon){};
string Dragon::getinfo() const
{
	stringstream tmp;
	tmp << "It has a " << weapon->getname() << ",and it's morale is " << fixed << setprecision(2) << morale;
	return tmp.str();
}
Samurai *Dragon::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	return new Dragon(info, cnt, getHP(), getAtk(), (double)info->getHP() / getHP() - 1, info->getweapon(cnt % 3));
}
Dragon::~Dragon()
{
	if (weapon)
		delete weapon;
}

Ninja::Ninja(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, Weapon *_weapon1 = NULL, Weapon *_weapon2 = NULL) : Samurai(_B, _number, _HP, _Atk)
{
	weapon[0] = _weapon1;
	weapon[1] = _weapon2;
}
string Ninja::getinfo() const
{
	string ret = "It has";
	if (weapon[0])
		ret += " a " + weapon[0]->getname();
	if (weapon[1])
		ret += " and a " + weapon[1]->getname();
	return ret;
}
Samurai *Ninja::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	return new Ninja(info, cnt, getHP(), getAtk(), info->getweapon(cnt % 3), info->getweapon((cnt + 1) % 3));
}
Ninja::~Ninja()
{
	if (weapon[0])
		delete weapon[0];
	if (weapon[1])
		delete weapon[1];
}

Iceman::Iceman(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, Weapon *_weapon = NULL) : Samurai(_B, _number, _HP, _Atk), weapon(_weapon){};
string Iceman::getinfo() const
{
	return "It has a " + ((weapon != NULL) ? weapon->getname() : "");
}
Samurai *Iceman::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	return new Iceman(info, cnt, getHP(), getAtk(), info->getweapon(cnt % 3));
}
Iceman::~Iceman()
{
	if (weapon)
		delete weapon;
}

Lion::Lion(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, int _loyalty = 0) : Samurai(_B, _number, _HP, _Atk), loyalty(_loyalty){};
string Lion::getinfo() const
{
	return "It's loyalty is " + to_string(loyalty);
}
Samurai *Lion::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	return new Lion(info, cnt, getHP(), getAtk(), info->getHP() - getHP());
}

Wolf::Wolf(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0) : Samurai(_B, _number, _HP, _Atk){};
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