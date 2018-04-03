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

class Samurai
{
  Headquarter *belong;
	int Number, HealthPoint, Attack;

  public:
	Samurai(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	virtual string getname() const = 0;
	int getHP() const { return HealthPoint; }
	int getnumber() const { return Number; }
	int getAtk() const { return Attack; }
	virtual string getinfo() const = 0;
	virtual Samurai *generate(Headquarter *) const = 0;
	virtual ~Samurai() = default;
	Headquarter* get_belong();
	int get_direct();
	string escape(){return "";}
};

class Dragon : public Samurai
{
	Weapon *weapon;
	double morale;

  public:
	Dragon(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, double _morale = 0, Weapon *_weapon = NULL);
	virtual string getname() const { return "dragon"; }
	virtual ~Dragon();
	virtual string getinfo() const;
	Samurai *generate(Headquarter *info) const;
};

class Ninja : public Samurai
{
	Weapon *weapon[2];

  public:
	Ninja(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, Weapon *_weapon1 = NULL, Weapon *_weapon2 = NULL);
	virtual string getname() const { return "ninja"; }
	virtual ~Ninja();
	virtual string getinfo() const;
	Samurai *generate(Headquarter *info) const;
};

class Iceman : public Samurai
{
	Weapon *weapon;

  public:
	Iceman(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, Weapon *_weapon = NULL);
	virtual string getname() const { return "iceman"; }
	virtual ~Iceman();
	virtual string getinfo() const;
	Samurai *generate(Headquarter *info) const;
};

class Lion : public Samurai
{
	int loyalty;

  public:
	Lion(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, int _loyalty = 0);
	virtual string getname() const { return "lion"; }
	virtual ~Lion() = default;
	virtual string getinfo() const;
	Samurai *generate(Headquarter *info) const;
};

class Wolf : public Samurai
{

  public:
	Wolf(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	virtual string getname() const { return "wolf"; }
	virtual ~Wolf() = default;
	virtual string getinfo() const;
	Samurai *generate(Headquarter *info) const;
};

