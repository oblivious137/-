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
	int getpos() { return Position; }
	void setpos(int x) { Position = x; }
	void hurted(int x) { HealthPoint -= x; }
	virtual void moveeffect() {}
	bool isdead() { return HealthPoint <= 0; }
	int get_outputlevel() const;
	virtual string yelled() { return ""; }
	string report() { return getfullname() + " has " + bag.report() + " and " + to_string(getHP()) + " elements"; }
	friend bool BattleFirst(Samurai *a, Samurai *b);
	virtual Samurai *copy() = 0;
	virtual int reatack() { return getAtk() * reattack_ratio + 1e-8; };
};

class Dragon : public Samurai
{
	double morale;

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
	string yelled();
};

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
};

class Iceman : public Samurai
{

  public:
	Iceman(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	string getname() const { return "iceman"; }
	~Iceman() = default;
	static Samurai *generate(Headquarter *info, int _HP, int _Atk);
	void moveeffect() { hurted(getHP() * (0.1 + 1e-8)); }
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

  public:
	Lion(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, int _loyalty = 0, int _LDec = 0);
	string getname() const { return "lion"; }
	~Lion() = default;
	string borninfo() const;
	static Samurai *generate(Headquarter *info, int _HP, int _Atk, int _LDec);
	void moveeffect() { loyalty -= LDec; }
	string escape();
	Samurai *copy()
	{
		Lion *ret = new Lion(get_belong(), getnumber(), getHP(), getAtk(), loyalty, LDec);
		ret->getbag() = getbag();
		return ret;
	}
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
};
