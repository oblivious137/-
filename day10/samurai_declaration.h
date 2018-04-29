#pragma once
#include "WarcraftClassDeclaration.h"
#include "weapon_declaration.h"

class Samurai
{
	Headquarter *belong;
	WeaponBag bag;
	int Number, HealthPoint, Attack, Position;
	double reattack_ratio;

  public:
	Samurai(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, double _reatack = 0);
	void setbelong(Headquarter *x) { belong = x; }
	virtual string getname() const = 0;
	int getHP() const { return HealthPoint; }
	int getnumber() const { return Number; }
	int getAtk() const { return Attack; }
	void setAtk(int x) { Attack = x; }
	virtual string borninfo() const { return ""; }
	static Samurai *generate() { return NULL; }
	virtual ~Samurai() = default;
	Headquarter *get_belong() const;
	int get_direct() const;
	virtual string escape() { return ""; }
	bool addweapon(Weapon *);
	virtual string rob(Samurai *x) { return ""; }
	WeaponBag &getbag() { return bag; }
	string getfullname() const;
	int getpos() const { return Position; }
	void setpos(int x) { Position = x; }
	void hurted(int x) { HealthPoint -= min(x, HealthPoint); }
	virtual void moveeffect() {}
	bool isdead() { return HealthPoint <= 0; }
	int get_outputlevel() const;
	int get_routputlevel() const;
	virtual string yelled() { return ""; }
	string report() { return getfullname() + " has " + bag.report(); }
	virtual Samurai *copy() = 0;
	virtual int reatack() { return getAtk() * reattack_ratio + 1e-8; };
	Weapon *canshot() { return bag.canshot(); }
	Weapon *canbomb() { return bag.canbomb(); }
	virtual string fightwin(Samurai *);
	virtual string fightlose(Samurai *) { return ""; }
	virtual string fightdraw(Samurai *, int) { return ""; }
	virtual string Fight(Samurai *x)
	{
		if (!isdead())
			x->hurted(Attack + bag.Atk());
		return "";
	}
	virtual string reFight(Samurai *x)
	{
		if (!isdead())
			x->hurted(Attack / 2 + bag.Atk());
		return "";
	}
	virtual void prefight() {}
};

class Dragon : public Samurai
{
	double morale;
	static const double fight_morale;

  public:
	Dragon(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, double _morale = 0);
	string getname() const { return "dragon"; }
	~Dragon() = default;
	static Samurai *generate(Headquarter *info, int _HP, int _Atk);
	Samurai *copy()
	{
		Dragon *ret = new Dragon(get_belong(), getnumber(), getHP(), getAtk(), morale);
		ret->getbag() = getbag();
		return ret;
	}
	string borninfo() const;
	string yelled();
	string fightwin(Samurai *a);
	string fightdraw(Samurai *, int);
};
const double Dragon::fight_morale = 0.2;

class Ninja : public Samurai
{
  public:
	Ninja(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	string getname() const { return "ninja"; }
	~Ninja() = default;
	static Samurai *generate(Headquarter *info, int _HP, int _Atk);
	Samurai *copy()
	{
		Ninja *ret = new Ninja(get_belong(), getnumber(), getHP(), getAtk());
		ret->getbag() = getbag();
		return ret;
	}
	string reFight(Samurai *) { return ""; }
};

class Iceman : public Samurai
{
  public:
	Iceman(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	string getname() const { return "iceman"; }
	~Iceman() = default;
	static Samurai *generate(Headquarter *info, int _HP, int _Atk);
	void moveeffect();
	Samurai *copy()
	{
		Iceman *ret = new Iceman(get_belong(), getnumber(), getHP(), getAtk());
		ret->getbag() = getbag();
		return ret;
	}
};

class Lion : public Samurai
{
	int loyalty;
	int LDec;
	int lastHP;

  public:
	Lion(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, int _loyalty = 0, int _LDec = 0);
	string getname() const { return "lion"; }
	~Lion() = default;
	string borninfo() const;
	static Samurai *generate(Headquarter *info, int _HP, int _Atk, int _LDec);
	void changeloyalty() { loyalty -= LDec; }
	string escape();
	Samurai *copy()
	{
		Lion *ret = new Lion(get_belong(), getnumber(), getHP(), getAtk(), loyalty, LDec);
		ret->getbag() = getbag();
		return ret;
	}
	void prefight() { lastHP = getHP(); }
	string fightdraw(Samurai *, int);
	string fightlose(Samurai *);
};

class Wolf : public Samurai
{
  public:
	Wolf(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	string getname() const { return "wolf"; }
	~Wolf() = default;
	static Samurai *generate(Headquarter *info, int _HP, int _Atk);
	string rob(Samurai *);
	Samurai *copy()
	{
		Wolf *ret = new Wolf(get_belong(), getnumber(), getHP(), getAtk());
		ret->getbag() = getbag();
		return ret;
	}
	string fightwin(Samurai *);
};
