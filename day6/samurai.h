#pragma once
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include "headquarter_define.h"
#include "weapon_define.h"
using namespace std;

class Samurai
{
	Headquarter *belong;
	int Number, HealthPoint, Attack;

  public:
	Samurai(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0) : belong(_B), Number(_number), HealthPoint(_HP), Attack(_Atk){};
	virtual string getname() const = 0;
	int getHP() const { return HealthPoint; }
	int getnumber() const { return Number; }
	int getAtk() const { return Attack; }
	virtual string getinfo() const = 0;
	virtual Samurai *generate(Headquarter *) const = 0;
	virtual ~Samurai() = default;
	Headquarter* get_belong(){return belong;}
	int get_direct(){return belong->get_direct();}
	string escape(){return "";}
};

class Dragon : public Samurai
{
	Weapon *weapon;
	double morale;

  public:
	Dragon(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, double _morale = 0, Weapon *_weapon = NULL) : Samurai(_B, _number, _HP, _Atk), morale(_morale), weapon(_weapon){};
	string getweaponname() const { return weapon->getname(); }
	virtual string getname() const { return "dragon"; }
	virtual ~Dragon()
	{
		if (weapon)
			delete weapon;
	}
	virtual string getinfo() const;
	Samurai *generate(Headquarter *info) const;
};

class Ninja : public Samurai
{
	Weapon *weapon[2];

  public:
	Ninja(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, Weapon *_weapon1 = NULL, Weapon *_weapon2 = NULL) : Samurai(_B, _number, _HP, _Atk)
	{
		weapon[0] = _weapon1;
		weapon[1] = _weapon2;
	}
	virtual string getname() const { return "ninja"; }
	virtual ~Ninja()
	{
		if (weapon[0])
			delete weapon[0];
		if (weapon[1])
			delete weapon[1];
	}
	virtual string getinfo() const;
	Samurai *generate(Headquarter *info) const;
};

class Iceman : public Samurai
{
	Weapon *weapon;

  public:
	Iceman(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, Weapon *_weapon = NULL) : Samurai(_B, _number, _HP, _Atk), weapon(_weapon){};
	virtual string getname() const { return "iceman"; }
	virtual ~Iceman()
	{
		if (weapon)
			delete weapon;
	}
	virtual string getinfo() const;
	Samurai *generate(Headquarter *info) const;
};

class Lion : public Samurai
{
	int loyalty;

  public:
	Lion(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, int _loyalty = 0) : Samurai(_B, _number, _HP, _Atk), loyalty(_loyalty){};
	virtual string getname() const { return "lion"; }
	virtual ~Lion() = default;
	virtual string getinfo() const;
	Samurai *generate(Headquarter *info) const;
};

class Wolf : public Samurai
{

  public:
	Wolf(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0) : Samurai(_B, _number, _HP, _Atk){};
	virtual string getname() const { return "wolf"; }
	virtual ~Wolf() = default;
	virtual string getinfo() const;
	Samurai *generate(Headquarter *info) const;
};

string Dragon::getinfo() const
{
	stringstream tmp;
	tmp << "It has a " << getweaponname() << ",and it's morale is " << fixed << setprecision(2) << morale;
	return tmp.str();
}
Samurai *Dragon::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	return new Dragon(info, cnt, getHP(), getAtk(), (double)info->getHP() / getHP() - 1, info->getweapon(cnt % 3));
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