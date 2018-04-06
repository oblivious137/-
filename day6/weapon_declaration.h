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
	Weapon() = default;
	Weapon(const std::string &_name = "", double _ratio = 0) : name(_name), ratio(_ratio){};
	void setattack(int a){};  /*************/
	void setname(string a){}; /*************/
	string getname() { return name; }
	//int getattack() { return _attack; }
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
};

class Sword : Weapon
{
  public:
	virtual int getnumber() { return 0; }
	int attack(Samurai *a, Samurai *b)
	{
		int tmp = getratio()*(a->getAtk());
		b->hurted(tmp);
		if (tmp>0) return 1;
		else return 0;
	}
};

class Bomb : Weapon
{

  public:
	virtual int getnumber() { return 1; }
	int attack(Samurai *a, Samurai *b){
		int tmp = getratio()*(a->getAtk());
		a->hurted(tmp*0.5);
		b->hurted(tmp);
		int ret=4;
		if (tmp>0) ret|=1;
		if (tmp>1) ret|=2;
		return ret;
	}
};

class Arrow : Weapon
{
	int times;

  public:
	virtual int getnumber() { return 2; }
	int priority() { return times; }
	int attack(Samurai *a, Samurai *b){
		if (times<=0) return 4;
		int tmp = getratio()*(a->getAtk());
		b->hurted(tmp);
		if (tmp>0) return 5;
		else return 4;
	}
	bool bethrown() { return times <= 0; }
};

class WeaponBag
{
	vector<Weapon *> weapons;
	int VolumeLimit;
	vector<Weapon *>::iterator _now;

  public:
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
	~WeaponBag()
	{
		for (auto &x : weapons)
			delete x;
	}
};