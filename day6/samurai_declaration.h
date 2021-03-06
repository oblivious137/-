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
#include "weapon_declaration.h"
using namespace std;

class Samurai
{
	Headquarter *belong;
	WeaponBag bag;
	int Number, HealthPoint, Attack, Position;

  public:
	Samurai(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	void setbelong(Headquarter *x) { belong = x; }
	virtual string getname() const = 0;
	int getHP() const { return HealthPoint; }
	int getnumber() const { return Number; }
	int getAtk() const { return Attack; }
	virtual string borninfo() const { return ""; }
	virtual Samurai *generate(Headquarter *) const = 0;
	virtual ~Samurai() = default;
	Headquarter *get_belong() const;
	int get_direct() const;
	virtual string escape() { return ""; }
	bool addweapon(Weapon *);
	virtual string rob(Samurai *x) { return ""; }
	WeaponBag &getbag() { return bag; }
	string getfullname() const;
	int getpos() { return Position; }
	void setpos(int x) { Position = x; }
	void hurted(int x) { HealthPoint -= x; }
	virtual void moveeffect() {}
	bool isdead() { return HealthPoint <= 0; }
	int get_outputlevel() const;
	virtual string yelled() { return ""; }
	string report() { return getfullname() + " has " + bag.report() + " and " + to_string(getHP()) + " elements"; }
	friend bool BattleFirst(Samurai *a, Samurai *b);
	virtual Samurai* copy() = 0;
};

class Dragon : public Samurai
{
	double morale;

  public:
	Dragon(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, double _morale = 0);
	string getname() const { return "dragon"; }
	~Dragon() = default;
	Samurai *generate(Headquarter *info) const;
	Samurai* copy(){
		Dragon* ret = new Dragon();
		memcpy(ret, this, sizeof(Dragon));
		return ret;
	}
	string yelled();
};

class Ninja : public Samurai
{

  public:
	Ninja(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	string getname() const { return "ninja"; }
	~Ninja() = default;
	Samurai *generate(Headquarter *info) const;
	Samurai* copy(){
		Ninja* ret = new Ninja();
		memcpy(ret, this, sizeof(Ninja));
		return ret;
	}
};

class Iceman : public Samurai
{

  public:
	Iceman(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	string getname() const { return "iceman"; }
	~Iceman() = default;
	Samurai *generate(Headquarter *info) const;
	void moveeffect() { hurted(getHP() * (0.1 + 1e-8)); }
	Samurai* copy(){
		Iceman* ret = new Iceman();
		memcpy(ret, this, sizeof(Iceman));
		return ret;
	}
};

class Lion : public Samurai
{
	int loyalty;
	int LDec;

  public:
	Lion(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, int _loyalty = 0, int _LDec = 0);
	string getname() const { return "lion"; }
	~Lion() = default;
	string borninfo() const;
	Samurai *generate(Headquarter *info) const;
	void moveeffect() { loyalty -= LDec; }
	string escape();
	Samurai* copy(){
		Lion* ret = new Lion();
		memcpy(ret, this, sizeof(Lion));
		return ret;
	}
};

class Wolf : public Samurai
{

  public:
	Wolf(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	string getname() const { return "wolf"; }
	~Wolf() = default;
	Samurai *generate(Headquarter *info) const;
	string rob(Samurai *);
	Samurai* copy(){
		Wolf* ret = new Wolf();
		memcpy(ret, this, sizeof(Wolf));
		return ret;
	}
};
