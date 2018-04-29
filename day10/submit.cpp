#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <set>
#include <map>
#include <tuple>
using namespace std;
/******* Weapon ********/
class Weapon;
class WeaponBag;

/******* Samurai *******/
class Samurai;

/****** Battle *******/
class Headquarter;
class TIME;
class _OUT_CMP;
class OrderedOutput;
class BattleField;

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
	virtual bool canshot() const { return false; };
	virtual bool canbomb() const { return false; };
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
	virtual ~Weapon() = default;
};
const string Weapon::_weaponnames[3] = {"sword", "bomb", "arrow"};

class Sword : Weapon
{
  public:
	double worn_ratio;
	Sword(string name = "", int _atk = 0, double _ratio = 0) : Weapon(name, _atk), worn_ratio(_ratio){};
	virtual int getnumber() const { return 0; }
	bool bethrown() const { return getAtk() <= 0; }
	int attack(Samurai *, Samurai *);
	static Weapon *generate(Samurai *, double, double);
	void worn() { setAtk(getAtk() * worn_ratio + 1e-8); }
	Weapon *copy() { return new Sword(getname(), getAtk(), worn_ratio); }
	string report() const { return '(' + to_string(getAtk()) + ')'; }
	~Sword() = default;
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
	~Bomb() = default;
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
	bool canshot() const { return true; }
	Weapon *copy() { return new Arrow(getname(), getAtk(), times); }
	void worn() { --times; }
	string report() const { return '(' + to_string(times) + ')'; }
	~Arrow() = default;
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
		weapons.clear();
		VolumeLimit = t.VolumeLimit;
		for (auto x : t.weapons)
			weapons.push_back(x->copy());
		return *this;
	}
	void set_limit(int x) { VolumeLimit = x; }
	void preliminary(bool (*x)(Weapon *, Weapon *) = Weapon::CMP)
	{
		for (int i = 0; i < weapons.size(); ++i)
		{
			if (weapons[i]->bethrown())
				weapons.erase(weapons.begin() + i), --i;
		}
		sort(weapons.begin(), weapons.end(), Weapon::CMP);
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
		while ((p = next()) != NULL)
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
		while ((p = next()) != NULL)
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
		while ((p = next()) != NULL)
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
		while ((p = next()) != NULL)
		{
			if ((!p->canshot()) && (!p->canbomb()))
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
		return getfullname() + " fought back against " + x->getfullname() + " in city " + to_string(getpos());
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
		ret->setpos(getpos());
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
		ret->setpos(getpos());
		return ret;
	}
	string reFight(Samurai *x) { return ""; }
};

class Iceman : public Samurai
{
	int step_count = 0;
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
		ret->setpos(getpos());
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
		ret->setpos(getpos());
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
		ret->setpos(getpos());
		return ret;
	}
	string fightwin(Samurai *);
};

class Headquarter
{
  private:
	typedef tuple<int, Samurai *, int> quetype;
	string name;
	vector<function<Samurai *(Headquarter *)>> Order;
	vector<function<Weapon *(Samurai *)>> Weapons;
	vector<int> ExistNumber;
	int HealthPoint, OnWitch = 0, Count = 0;
	int posi, dire, outputlevel;
	vector<quetype> que;

  public:
	Headquarter(const string &_name, int _HP,
				const vector<function<Samurai *(Headquarter *)>> &_Order,
				const vector<function<Weapon *(Samurai *)>> &_Weapons);
	Headquarter(const Headquarter &);
	tuple<Samurai *, string> Build_SA();
	unsigned int getOrderSize();
	unsigned int getWeaponsSize() { return Weapons.size(); }
	int getCount() const { return Count; }
	int generateNextSamurai(int);
	void changeCount(int x) { Count = x; }
	int getHP() const { return HealthPoint; }
	void changeHP(int x) { HealthPoint = x; }
	Weapon *getweapon(int, Samurai *) const;
	int get_pos() const { return posi; }
	void set_pos(int x) { posi = x; }
	int get_direct() const { return dire; }
	void set_direct(int x) { dire = x; }
	int get_outputlevel() const { return outputlevel; }
	void set_outputlevel(int x) { outputlevel = x; }
	string getname() const { return name; }
	void receiveHP(int x = 0) { HealthPoint += x; }
	void askforHP(quetype x) { que.push_back(x); }
	void clearque();
	~Headquarter() = default;
};

class TIME
{
	int minutes;

  public:
	TIME() = default;
	TIME(int x) : minutes(x){};
	void inc(int a) { minutes += a; }
	void Print()
	{
		printf("%03d:%02d", minutes / 60, minutes % 60);
	}
	string sPrint()
	{
		char tmp[10];
		sprintf(tmp, "%03d:%02d", minutes / 60, minutes % 60);
		return tmp;
	}
	bool operator<(const TIME t) { return minutes < t.minutes; }
	bool operator>(const TIME t) { return minutes > t.minutes; }
};

class OrderedOutput
{
	vector<pair<int, string>> data;

  public:
	void push(int t, string x) { data.push_back(make_pair(t, x)); }
	void flush()
	{
		sort(data.begin(), data.end(), [](const pair<int, string> &a, const pair<int, string> &b) { return a.first < b.first; });
		
		for (auto &x : data)
		{
			cout << x.second;
		}
		data.clear();
	}
	~OrderedOutput()
	{
		flush();
	}
};

class City
{
	typedef set<Samurai *> SamuraiSet;
	SamuraiSet samurais;
	int pos, HP = 0;
	Headquarter *possessed = NULL, *_definedfirst = NULL;
	Headquarter *last_win = NULL;

  public:
	City() = default;
	void insert(Samurai *x) { samurais.insert(x); }
	void erase(Samurai *const &x) { samurais.erase(x); }
	SamuraiSet::const_iterator begin() const { return samurais.cbegin(); }
	SamuraiSet::const_iterator end() const { return samurais.cend(); }
	SamuraiSet::const_reverse_iterator rbegin() const { return samurais.crbegin(); }
	SamuraiSet::const_reverse_iterator rend() const { return samurais.crend(); }
	void set_pos(int x) { pos = x; }
	int get_pos() const { return pos; }
	unsigned int size() const { return samurais.size(); }
	void generateHP() { HP += 10; }
	void win(Headquarter *);
	Headquarter *deffirst() const { return _definedfirst; }
	void setdeffirst(Headquarter *x) { _definedfirst = x; }
	Headquarter *conquer() const { return possessed; }
	string fightresult(Headquarter *a)
	{
		if (a != NULL && last_win == a && possessed != a){
			possessed = last_win;
			return a->getname() + " flag raised in city " + to_string(get_pos());
		}
		last_win = a;
		return "";
	}
	int lostHP();
};

class BattleField
{
	TIME T;
	int Size;
	Headquarter HeadA, HeadB;
	City *city;

  public:
	BattleField(int n, Headquarter a, Headquarter b);
	void Run(TIME);
	void BuildTurn();
	int MoveTurn();
	void BattleTurn();
	vector<string> Fight(Samurai *, Samurai *);
	bool FightFirst(Samurai *, Samurai *);
	~BattleField();
};

int Sword::attack(Samurai *a, Samurai *b)
{
    int ret = 0;
    int tmp = a->getAtk() + getAtk();
    b->hurted(tmp);
    if (tmp > 0)
        ret |= 1;
    setAtk(getAtk() * worn_ratio + 1e-8);
    ret |= 4;
    return ret;
}
Weapon *Sword::generate(Samurai *w, double _atk, double _worn)
{
    return new Sword("sword", _atk*w->getAtk()+1e-8, _worn);
}

int Bomb::attack(Samurai *a, Samurai *b)
{
    if (times <= 0)
        return 4;
    int ret = 0;
    if (a->getHP())
        ret |= 2;
    if (b->getHP())
        ret |= 1;
    a->hurted(a->getHP());
    b->hurted(b->getHP());
    --times;
    ret |= 4;
    return ret;
}
Weapon *Bomb::generate(Samurai *w, int _limit)
{
    return new Bomb("bomb", 1, _limit);
}

int Arrow::attack(Samurai *a, Samurai *b)
{
    if (times <= 0)
        return 4;
    int ret = 0;
    if (b->getHP() > 0 && getAtk() > 0)
        ret |= 1;
    b->hurted(getAtk());
    --times;
    ret |= 4;
    return ret;
}
Weapon *Arrow::generate(Samurai *w, int _atk, int _limit)
{
    return new Arrow("arrow", _atk, _limit);
}

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

/********* Headquarter ***********/

Headquarter::Headquarter(const string &_name, int _HP,
                         const vector<function<Samurai *(Headquarter *)>> &_Order,
                         const vector<function<Weapon *(Samurai *)>> &_Weapons) : name(_name), HealthPoint(_HP), Order(_Order),
                                                                                  Weapons(_Weapons), ExistNumber(_Order.size(), 0){};
Headquarter::Headquarter(const Headquarter &a) : name(a.name), HealthPoint(a.HealthPoint), Order(a.Order), Weapons(a.Weapons), ExistNumber(a.ExistNumber) {}

int Headquarter::generateNextSamurai(int _HP)
{
    HealthPoint -= _HP;
    return ++Count;
}

unsigned int Headquarter::getOrderSize() { return Order.size(); }
Weapon *Headquarter::getweapon(int x, Samurai *who) const
{
    return Weapons[x](who);
}

tuple<Samurai *, string> Headquarter::Build_SA()
{
    Samurai *tmp = Order[OnWitch](this);
    if (tmp)
    {
        ++ExistNumber[OnWitch];
        ++OnWitch;
        if (OnWitch >= Order.size())
            OnWitch -= Order.size();
        string ret = tmp->borninfo();
        if (ret != "")
            ret = tmp->getfullname() + " born\n" + ret;
        else
            ret = tmp->getfullname() + " born";
        return make_tuple(tmp, ret);
    }
    return make_tuple<Samurai *, string>(NULL, "");
}

void Headquarter::clearque()
{
    sort(que.begin(), que.end(), [this](const quetype &x, const quetype &y) {
        if (get<2>(x) != get<2>(y))
            return get<2>(x) > get<2>(y);
        return abs(get<0>(x) - get_pos()) > abs(get<0>(y) - get_pos());
    });
    for (auto x : que)
    {
        if (get<2>(x) > 0 && getHP() >= get<2>(x))
        {
            get<1>(x)->hurted(-get<2>(x));
            changeHP(getHP() - get<2>(x));
        }
        else if (get<2>(x) < 0)
        {
            changeHP(getHP() - get<2>(x));
        }
    }
    que.clear();
    return;
}

/******* City *********/

int City::lostHP()
{
    int ret = HP;
    HP = 0;
    return ret;
}

/******* BattleField *******/

BattleField::BattleField(int n, Headquarter a, Headquarter b) : Size(n + 1), HeadA(a), HeadB(b), T(0)
{
    HeadA.set_pos(0);
    HeadB.set_pos(n + 1);
    HeadA.set_direct(1);
    HeadB.set_direct(-1);
    HeadA.set_outputlevel(1);
    HeadB.set_outputlevel(2);
    city = new City[n + 2];
    for (int i = 0; i <= Size; ++i)
    {
        city[i].set_pos(i);
        if (i & 1)
            city[i].setdeffirst(&HeadA);
        else
            city[i].setdeffirst(&HeadB);
    }
}

void BattleField::BuildTurn()
{
    OrderedOutput output;
    string log;
    Samurai *nS;
    tie(nS, log) = HeadA.Build_SA();
    if (nS && log.length())
        output.push(HeadA.get_outputlevel(), T.sPrint() + ' ' + log + '\n');
    if (nS)
        city[HeadA.get_pos()].insert(nS);

    tie(nS, log) = HeadB.Build_SA();
    if (nS && log.length())
        output.push(HeadB.get_outputlevel(), T.sPrint() + ' ' + log + '\n');
    if (nS)
        city[HeadB.get_pos()].insert(nS);
}

int BattleField::MoveTurn()
{
    OrderedOutput output;
    for (int i = 0; i <= Size; ++i)
    {
        vector<Samurai *> tmp;
        for (auto x : city[i])
        {
            if (x->get_direct() < 0)
            {
                int aim = i + x->get_direct();
                if (aim >= 0 && aim <= Size)
                {
                    tmp.push_back(x);
                    city[aim].insert(x);
                    x->moveeffect();
                    x->setpos(aim);
                    stringstream tp;
                    tp << T.sPrint() << ' ' << x->getfullname();
                    if (aim != 0 && aim != Size)
                        tp << " marched to city " << aim;
                    else
                        tp << " reached " << ((aim == 0) ? (HeadA.getname()) : (HeadB.getname())) << " headquarter";
                    tp << " with " << x->getHP() << " elements and force " << x->getAtk() << '\n';
                    output.push(x->get_routputlevel(), tp.str());
                }
            }
        }
        for (auto &x : tmp)
            city[i].erase(x);
    }

    for (int i = Size; i >= 0; --i)
    {
        vector<Samurai *> tmp;
        for (auto x : city[i])
        {
            if (x->get_direct() > 0)
            {
                int aim = i + x->get_direct();
                if (aim >= 0 && aim <= Size)
                {
                    tmp.push_back(x);
                    city[aim].insert(x);
                    x->moveeffect();
                    x->setpos(aim);
                    stringstream tp;
                    tp << T.sPrint() << ' ' << x->getfullname();
                    if (aim != 0 && aim != Size)
                        tp << " marched to city " << aim;
                    else
                        tp << " reached " << ((aim == 0) ? (HeadA.getname()) : (HeadB.getname())) << " headquarter";
                    tp << " with " << x->getHP() << " elements and force " << x->getAtk() << '\n';
                    output.push(x->get_routputlevel(), tp.str());
                }
            }
        }
        for (auto &x : tmp)
            city[i].erase(x);
    }
    if (city[0].size() >= 2)
        output.push(50, T.sPrint() + ' ' + HeadA.getname() + " headquarter was taken\n");
    if (city[Size].size() >= 2)
        output.push(100 * Size + 50, T.sPrint() + ' ' + HeadB.getname() + " headquarter was taken\n");
    output.flush();
    if (city[0].size() >= 2 || city[Size].size() >= 2)
        return -1;
    else
        return 0;
}

bool BattleField::FightFirst(Samurai *a, Samurai *b)
{
    if (city[a->getpos()].conquer() == a->get_belong())
        return true;
    if (city[b->getpos()].conquer() == b->get_belong())
        return false;
    if (city[a->getpos()].deffirst() == a->get_belong())
        return true;
    return false;
}

vector<string> BattleField::Fight(Samurai *a, Samurai *b)
{
    vector<string> ret;
    if (a->isdead() || b->isdead())
        return ret;
    if (!FightFirst(a, b))
        swap(a, b);
    a->Fight(b);
    ret.push_back(a->getfullname() + " attacked " + b->getfullname() + " in city " + to_string(a->getpos()) + " with " + to_string(a->getHP()) + " elements and force " + to_string(a->getAtk()));
    if (!b->isdead())
    {
        string tmp = b->reFight(a);
		if (tmp.length() > 0) ret.push_back(tmp);
    }
    return ret;
}

void BattleField::BattleTurn()
{
    static const int base = 100, fight = 0, dead = 10, yelled = 15, earn = 20, flag = 25;
    OrderedOutput output;
    for (int i = 1; i < Size; ++i)
        if (city[i].size() == 2)
        {

            Samurai *a = *city[i].begin();
            Samurai *b = *city[i].rbegin();
            a->prefight();
            b->prefight();
            vector<string> state = Fight(a, b);
            //Output
            for (int k = 0; k < state.size(); ++k)
                output.push(i * base + fight + k, T.sPrint() + ' ' + state[k] + '\n');

            //win && draw && lose
            if (a->isdead() ^ b->isdead())
            {
                if (a->isdead())
                    swap(a, b);
                a->fightwin(b);
                b->fightlose(a);
            }
            else if (a->isdead())
            {
                a->fightdraw(b, 0);
                b->fightdraw(a, 0);
            }
            else
            {
                a->fightdraw(b, 1);
                b->fightdraw(a, 1);
            }

            //Dead message
            if (a->isdead())
            {
                if (state.size() > 0)
                    output.push(i * base + dead, T.sPrint() + ' ' + a->getfullname() + " was killed in city " +
                                                     to_string(i) + '\n');
            }
            if (b->isdead())
            {
                if (state.size() > 0)
                    output.push(i * base + dead, T.sPrint() + ' ' + b->getfullname() + " was killed in city " +
                                                     to_string(i) + '\n');
            }

            //yelled    because draw or win won't change Fight oreder!!!!!!!!!!!!!!!!!!!!!!
            if (!a->isdead() && FightFirst(a, b))
            {
                string log = a->yelled();
                if (log != "")
                    output.push(i * base + yelled, T.sPrint() + ' ' + log + '\n');
            }
            if (!b->isdead() && FightFirst(b, a))
            {
                string log = b->yelled();
                if (log != "")
                    output.push(i * base + yelled, T.sPrint() + ' ' + log + '\n');
            }

            //earn elements
            if (a->isdead() ^ b->isdead())
            {
                if (!b->isdead())
                    swap(a, b);
                int tmp = city[i].lostHP();
                output.push(i * base + earn, T.sPrint() + ' ' + a->getfullname() + " earned " +
                                                 to_string(tmp) + " elements for his headquarter\n");
                a->get_belong()->askforHP(make_tuple(i, a, -tmp));
            }

            //Confirm flag
            if (a->isdead() ^ b->isdead())
            {
                if (a->isdead())
                    swap(a, b);
                string log = city[i].fightresult(a->get_belong());
                if (log.length() > 0)
                    output.push(i * base + flag, T.sPrint() + ' ' + log + '\n');
            }
            else if (!a->isdead())
            {
                string log = city[i].fightresult(NULL);
                if (log.length() > 0)
                    output.push(i * base + flag, T.sPrint() + ' ' + log + '\n');
            }
        }
    // clear queue
    HeadA.clearque();
    HeadB.clearque();
    //delete dead samurai
	for (int i = 0; i <= Size; ++i) {
		vector<Samurai*> trash;
		for (auto a : city[i]) {
			if (a->isdead())
			{
				trash.push_back(a);
				delete a;
			}
		}
		for (auto a : trash) city[i].erase(a);
	}
    return;
}

void BattleField::Run(TIME limit)
{
    for (;;)
    {
        //fprintf(stderr,"xxxxxxxxxx\n");
        OrderedOutput output;
        /********* 0' : Build Samurais ********/
        BuildTurn();
        /********* 5' : Escape ********/
        T.inc(5);
        if (T > limit)
            break;
        {
            for (int i = 0; i <= Size; ++i)
            {
                vector<Samurai *> trash;
                string tmp;
                for (auto x : city[i])
                {
                    if ((tmp = x->escape()) != "")
                    {
                        trash.push_back(x);
                        output.push(x->get_outputlevel(), T.sPrint() + ' ' + tmp + "\n");
                        delete x;
                    }
                }
                for (auto &x : trash)
                    city[i].erase(x);
            }
            output.flush();
        }

        /******** 10: Move ********/
        T.inc(5);
        if (T > limit)
            break;
        if (MoveTurn() == -1)
            break;

        /********* 20' : generate HP in cities ********/
        T.inc(10);
        if (T > limit)
            break;
        for (int i = 1; i < Size; ++i)
            city[i].generateHP();

        /********* 30' : gain HP from cities ********/
        T.inc(10);
        if (T > limit)
            break;
        {
            for (int i = 1; i < Size; ++i)
                if (city[i].size() == 1)
                {
                    Samurai *a = *city[i].begin();
                    int tmp = city[i].lostHP();
                    a->get_belong()->receiveHP(tmp);
                    if (tmp)
                        output.push(i, T.sPrint() + ' ' + a->getfullname() + " earned " +
                                           to_string(tmp) + " elements for his headquarter\n");
                }
            output.flush();
        }

        /********* 35' : Shot *********/
        T.inc(5);
        if (T > limit)
            break;
        {
            for (int i = 0; i <= Size; ++i)
            {
                Weapon *p;
                for (auto x : city[i])
                    if ((p = x->canshot()) != NULL)
                    {
                        int aim = i + x->get_direct();
                        if (aim >= 0 && aim <= Size)
                        {
                            for (auto y : city[aim])
                                if (y->get_belong() != x->get_belong())
                                {
                                    string log = T.sPrint() + ' ' + x->getfullname() + " shot";
                                    p->attack(x, y);
                                    if (y->isdead())
                                        log += " and killed " + y->getfullname();
                                    output.push(i * 100 + x->get_belong()->get_outputlevel(), log + '\n');
                                    break;
                                }
                        }
                    }
            }
            output.flush();
        }

        /********* 38' : Bomb **********/
        T.inc(3);
        if (T > limit)
            break;
        {
            for (int i = 1; i < Size; ++i)
                if (city[i].size() == 2)
                {
                    Samurai *a = *city[i].begin(), *b = *city[i].rbegin();
                    if (a->isdead() || b->isdead())
                        continue;
                    Samurai *aa = a->copy(), *bb = b->copy();
                    Fight(aa, bb);
                    if (aa->isdead() && aa->canbomb() != NULL)
                    {
                        output.push(i, T.sPrint() + ' ' + aa->getfullname() + " used a bomb and killed " + bb->getfullname() + '\n');
                        city[i].erase(a);
                        delete a;
                        city[i].erase(b);
                        delete b;
                    }
                    else if (bb->isdead() && bb->canbomb())
                    {
                        output.push(i, T.sPrint() + ' ' + bb->getfullname() + " used a bomb and killed " + aa->getfullname() + '\n');
                        city[i].erase(a);
                        delete a;
                        city[i].erase(b);
                        delete b;
                    }
                    delete aa;
                    delete bb;
                }
            output.flush();
        }

        /********* 40' : Fight *********/
        T.inc(2);
        if (T > limit)
            break;
        BattleTurn();

        /********* 50' : Headquarter Report **********/
        T.inc(10);
        if (T > limit)
            break;
        output.push(HeadA.get_outputlevel(), T.sPrint() + ' ' + to_string(HeadA.getHP()) + " elements in " + HeadA.getname() + " headquarter\n");
        output.push(HeadB.get_outputlevel(), T.sPrint() + ' ' + to_string(HeadB.getHP()) + " elements in " + HeadB.getname() + " headquarter\n");
        output.flush();

        /********* 55' : Samurai Report **********/
        T.inc(5);
        if (T > limit)
            break;
        for (int i = 0; i <= Size; ++i)
            for (auto x : city[i])
                output.push(x->get_outputlevel(), T.sPrint() + ' ' + x->report() + '\n');
        output.flush();

        T.inc(5);
        if (T > limit)
            break;
    }
}

BattleField::~BattleField()
{
    for (int i = 0; i < Size; ++i)
    {
        for (auto x : city[i])
            delete x;
    }
    delete[] city;
}

int main()
{
	int T = 0;
	scanf("%d", &T);
	for (int cas = 1; cas <= T; ++cas)
	{
		printf("Case %d:\n", cas);
		int M, N, R, K, D;
		scanf("%d%d%d%d%d", &M, &N, &R, &K, &D);
		int hp[5], atk[5];
		for (int i = 0; i < 5; ++i)
			scanf("%d", &hp[i]);
		for (int i = 0; i < 5; ++i)
			scanf("%d", &atk[i]);
		vector<function<Samurai *(Headquarter *)>> AS;
		AS.push_back(bind(Iceman::generate, placeholders::_1, hp[2], atk[2]));
		AS.push_back(bind(Lion::generate, placeholders::_1, hp[3], atk[3], K));
		AS.push_back(bind(Wolf::generate, placeholders::_1, hp[4], atk[4]));
		AS.push_back(bind(Ninja::generate, placeholders::_1, hp[1], atk[1]));
		AS.push_back(bind(Dragon::generate, placeholders::_1, hp[0], atk[0]));

		vector<function<Samurai *(Headquarter *)>> BS;
		BS.push_back(bind(Lion::generate, placeholders::_1, hp[3], atk[3], K));
		BS.push_back(bind(Dragon::generate, placeholders::_1, hp[0], atk[0]));
		BS.push_back(bind(Ninja::generate, placeholders::_1, hp[1], atk[1]));
		BS.push_back(bind(Iceman::generate, placeholders::_1, hp[2], atk[2]));
		BS.push_back(bind(Wolf::generate, placeholders::_1, hp[4], atk[4]));

		vector<function<Weapon *(Samurai *)>> W;
		W.push_back(bind(Sword::generate, placeholders::_1, 0.2, 0.8));
		W.push_back(bind(Bomb::generate, placeholders::_1, 1));
		W.push_back(bind(Arrow::generate, placeholders::_1, R, 3));

		Headquarter red("red", M, AS, W), blue("blue", M, BS, W);

		BattleField B(N, red, blue);
		B.Run(D);
	}
	return 0;
}