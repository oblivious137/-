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
#include "WarcraftClassDeclaration.h"
using namespace std;

class Weapon
{
  private:
	string name;
	//int _attack;
	double ratio;

  public:
	static const int _weaponkinds = 3;
	static const string _weaponnames[3];
	Weapon() = default;
	Weapon(const std::string &_name = "", double _ratio = 0) : name(_name), ratio(_ratio+1e-8){};
	void setattack(int a){};  /*************/
	void setname(string a){}; /*************/
	string getname() { return name; }
	// int getattack() { return _attack; }
	double getratio() { return ratio; }
	virtual int getnumber() = 0;
	virtual int priority() { return 0; }
	virtual bool bethrown() { return this == NULL; }
	virtual int attack(Samurai *a, Samurai *b) { return 0; } //bits: B_hurted, A_hurted, Weapon_hurted
	static bool CMP(Weapon *a, Weapon *b)
	{
		if (a->getnumber() != b->getnumber())
			return a->getnumber() < b->getnumber();
		return a->priority() < b->priority();
	}
	static bool RobCMP(Weapon *a, Weapon *b)
	{
		if (a->getnumber() != b->getnumber())
			return a->getnumber() < b->getnumber();
		return a->priority() > b->priority();
	}
};
const string Weapon::_weaponnames[3] = {"sword", "bomb", "arrow"};

class Sword : Weapon
{
  public:
	Sword(string name = "", double ratio = 0.0) : Weapon(name, ratio){};
	virtual int getnumber() { return 0; }
	int attack(Samurai *, Samurai *);
};

class Bomb : Weapon
{
	int times;
  public:
	Bomb(string name = "", double ratio = 0.0, int limit = 0) : Weapon(name, ratio), times(limit){};
	virtual int getnumber() { return 1; }
	int attack(Samurai *, Samurai *);
	bool bethrown() { return times <= 0; }
};

class Arrow : Weapon
{
	int times;

  public:
	Arrow(string name = "", double ratio = 0.0, int limit = 0) : Weapon(name, ratio), times(limit){};
	virtual int getnumber() { return 2; }
	int priority() { return times; }
	int attack(Samurai *, Samurai *);
	bool bethrown() { return times <= 0; }
	Weapon* build(){return new Arrow();}
};

class WeaponBag
{
	vector<Weapon *> weapons;
	int VolumeLimit;
	vector<Weapon *>::iterator _now;

  public:
	  WeaponBag() : VolumeLimit(10) {};
	void set_limit(int x) { VolumeLimit = x; }
	void preliminary(bool (*x)(Weapon *, Weapon *) = Weapon::CMP)
	{
		for (int i = 0; i < weapons.size(); ++i)
		{
			if (weapons[i]->bethrown())
				weapons.erase(weapons.begin() + i), --i;
		}
		sort(weapons.begin(), weapons.end(), x);
		_now = weapons.begin();
	}
	void throwit(vector<Weapon *>::iterator it)
	{
		int tmp = _now - weapons.begin();
		if (it < _now)
			--tmp;
		if (*it)
			delete *it;
		weapons.erase(it);
		_now = weapons.begin() + tmp;
	}
	Weapon *next()
	{
		if (_now == weapons.end())
			return NULL;
		if ((*_now)->bethrown())
			return throwit(_now), next();
		return *(_now++);
	}
	Weapon *renext()
	{
		while (weapons.size())
		{
			if (_now == weapons.end())
				_now = weapons.begin();
			if ((*_now)->bethrown())
				throwit(_now);
			else
				return *(_now++);
		}
	}
	bool push(Weapon *x) { return (weapons.size() < VolumeLimit) ? weapons.push_back(x), true : false; }
	int size() { return weapons.size(); }
	void pop_top(int x = 1)
	{
		int tmp = _now - weapons.begin();
		if (tmp < x)
			tmp = 0;
		else
			tmp -= x;
		weapons.erase(weapons.begin(), weapons.begin() + x);
		_now = weapons.begin() + tmp;
	}
	string report()
	{
		preliminary();
		int st = 0;
		string ret;

		for (int i = 0; i < Weapon::_weaponkinds; ++i)
		{
			int ed = st;
			while (ed < size() && weapons[ed]->getnumber() == i)
				++ed;
			if (i != 0) ret.append(" ");
			ret += to_string(ed - st) + ' ' + Weapon::_weaponnames[i];
			st = ed;
		}
		return ret;
	}
	~WeaponBag()
	{
		for (auto &x : weapons)
			delete x;
	}
};