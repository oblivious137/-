#pragma once
#include "WarcraftClassDeclaration.h"

class Weapon
{
  private:
	string name;
	int Atk;

  public:
	static const int _weaponkinds = 3;
	static const string _weaponnames[3];
	Weapon() = default;
	Weapon(const Weapon &a) : name(a.name), Atk(a.Atk){};
	Weapon(const std::string &_name = "", int _atk = 0) : name(_name), Atk(_atk){};
	void setAtk(int a) { Atk = a; };
	void setname(string a) { name = a; };
	string getname() const { return name; }
	int getAtk() const { return Atk; }
	virtual int getnumber() const = 0;
	virtual int priority() { return 0; }
	virtual bool bethrown() const { return this == NULL; }
	virtual int attack(Samurai *a, Samurai *b) { return 0; } //bits: B_hurted, A_hurted, Weapon_worn
	virtual bool canshot() { return false; };
	virtual bool canbomb() { return false; };
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
	virtual Weapon *copy() = 0;
	virtual void worn(){};
	virtual string report() const { return ""; };
};
const string Weapon::_weaponnames[3] = {"sword", "bomb", "arrow"};

class Sword : Weapon
{
  public:
	double worn_ratio;
	Sword(string name = "", int _atk = 0, double _ratio = 0) : Weapon(name, _atk), worn_ratio(_ratio){};
	virtual int getnumber() const { return 0; }
	bool bethrown() { return getAtk() <= 0; }
	int attack(Samurai *, Samurai *);
	static Weapon *generate(Samurai *, double, double);
	void worn() { setAtk(getAtk() * worn_ratio + 1e-8); }
	Weapon *copy() { return new Sword(getname(), getAtk(), worn_ratio); }
	string report() const { return '(' + to_string(getAtk()) + ')'; }
};

class Bomb : Weapon
{
	int times;

  public:
	Bomb(string name = "", int _atk = 0, int limit = 0) : Weapon(name, _atk), times(limit){};
	virtual int getnumber() const { return 1; }
	int attack(Samurai *, Samurai *);
	bool bethrown() const { return times <= 0; }
	static Weapon *generate(Samurai *, int);
	bool canbomb() const { return true; }
	Weapon *copy() { return new Bomb(getname(), getAtk(), times); }
	void worn() { --times; }
};

class Arrow : Weapon
{
	int times;

  public:
	Arrow(string name = "", int _atk = 0, int limit = 0) : Weapon(name, _atk), times(limit){};
	virtual int getnumber() const { return 2; }
	int priority() { return times; }
	int attack(Samurai *, Samurai *);
	bool bethrown() const { return times <= 0; }
	static Weapon *generate(Samurai *, int, int);
	bool canshot() { return true; }
	Weapon *copy() { return new Arrow(getname(), getAtk(), times); }
	void worn() { --times; }
};

class WeaponBag
{
	vector<Weapon *> weapons;
	int VolumeLimit = 10;
	vector<Weapon *>::iterator _now;

  public:
	WeaponBag() = default;
	WeaponBag(const WeaponBag &t) : weapons(t.weapons)
	{
		for (auto &x : weapons)
			x = x->copy();
	}
	WeaponBag &operator=(const WeaponBag &t)
	{
		weapons = t.weapons;
		for (auto &x : weapons)
			x = x->copy();
	}
	void set_limit(int x) { VolumeLimit = x; }
	void preliminary(bool (*x)(Weapon *, Weapon *) = Weapon::CMP)
	{
		for (int i = 0; i < weapons.size(); ++i)
		{
			if (weapons[i]->bethrown())
				weapons.erase(weapons.begin() + i), --i;
		}
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
		return NULL;
	}
	Weapon *canshot()
	{
		preliminary();
		Weapon *p;
		while (p = next())
		{
			if (p->canshot())
				return p;
		}
		return NULL;
	}
	Weapon *canbomb()
	{
		preliminary();
		Weapon *p;
		while (p = next())
		{
			if (p->canbomb())
				return p;
		}
		return NULL;
	}
	Weapon *canslash()
	{
		preliminary();
		Weapon *p;
		while (p = next())
		{
			if (!p->canbomb() && !p->canshot())
				return p;
		}
		return NULL;
	}
	int Atk()
	{
		preliminary();
		Weapon *p;
		while (p = next())
		{
			if (!p->canshot() && !p->canbomb())
			{
				int ret = p->getAtk();
				p->worn();
				return ret;
			}
		}
		return 0;
	}
	bool push(Weapon *x)
	{
		preliminary();
		for (auto a : weapons)
			if (a->getnumber() == x->getnumber())
				return false;
		weapons.push_back(x);
		return true;
	}
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
	void pop(int x)
	{
		int tmp = _now - weapons.begin();
		weapons.erase(weapons.begin() + x);
		if (tmp >= x)
			--tmp;
		_now = weapons.begin() + tmp;
	}
	string report()
	{
		preliminary();
		int st = 0;
		string ret;
		for (auto x = weapons.rbegin(); x != weapons.rend(); ++x)
		{
			if (ret.length() > 0)
				ret.append(",");
			ret += (*x)->getname() + (*x)->report();
		}
		if (ret.length() == 0)
			ret = "no weapon";
		return ret;
	}
	~WeaponBag()
	{
		for (auto &x : weapons)
			delete x;
	}
};