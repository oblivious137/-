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
		// for (auto &x : weapons)
		// 	delete x;
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
};

class Dragon : public Samurai
{
	double morale;

  public:
	Dragon(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, double _morale = 0);
	string getname() const { return "dragon"; }
	~Dragon() = default;
	Samurai *generate(Headquarter *info) const;
	string yelled();
};

class Ninja : public Samurai
{

  public:
	Ninja(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	string getname() const { return "ninja"; }
	~Ninja() = default;
	Samurai *generate(Headquarter *info) const;
};

class Iceman : public Samurai
{

  public:
	Iceman(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	string getname() const { return "iceman"; }
	~Iceman() = default;
	Samurai *generate(Headquarter *info) const;
	void moveeffect() { hurted(getHP() * (0.1 + 1e-8)); }
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
};

class Wolf : public Samurai
{

  public:
	Wolf(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0);
	string getname() const { return "wolf"; }
	~Wolf() = default;
	Samurai *generate(Headquarter *info) const;
	string rob(Samurai *);
};

class Headquarter
{
  private:
	string name;
	vector<Samurai *> Order;
	vector<function<Weapon *()>> Weapons;
	vector<int> ExistNumber;
	int HealthPoint, OnWitch = 0, Count = 0;
	bool stopped = false;
	int posi, dire, outputlevel;

  public:
	Headquarter(const string &_name, int _HP,
				const vector<Samurai *> &_Order,
				const vector<function<Weapon *()>> &_Weapons);
	void Stop();
	bool isstopped() { return stopped; }
	tuple<Samurai *, string> Build_SA();
	unsigned int getOrderSize();
	unsigned int getWeaponsSize() { return Weapons.size(); }
	int getCount() const { return Count; }
	void changeCount(int x) { Count = x; }
	int getHP() const { return HealthPoint; }
	void changeHP(int x) { HealthPoint = x; }
	Weapon *getweapon(int x) const;
	int get_pos() const { return posi; }
	void set_pos(int x) { posi = x; }
	int get_direct() const { return dire; }
	void set_direct(int x) { dire = x; }
	int get_outputlevel() const { return outputlevel; }
	void set_outputlevel(int x) { outputlevel = x; }
	string getname() const { return name; }
	~Headquarter();
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

class BattleField
{
	TIME T;
	int Size;
	Headquarter HeadA, HeadB;
	typedef set<Samurai *> SamuraiSet;
	SamuraiSet *city;

  public:
	BattleField(int n, Headquarter a, Headquarter b);
	void Run(TIME);
	void BuildTurn();
	int MoveTurn();
	void BattleTurn();
	~BattleField();
};

/********* Headquarter ***********/

Headquarter::Headquarter(const string &_name, int _HP,
                         const vector<Samurai *> &_Order,
                         const vector<function<Weapon *()>> &_Weapons) : name(_name), HealthPoint(_HP), Order(_Order),
                                                                         Weapons(_Weapons), ExistNumber(_Order.size(), 0)
{
}

unsigned int Headquarter::getOrderSize() { return Order.size(); }
Weapon *Headquarter::getweapon(int x) const { 
	Weapon* ret =Weapons[x]();
	
//	printf("build weapon %s\n", ret->getname().c_str());

	return ret;
}

void Headquarter::Stop()
{
    if (stopped)
        return;
    stopped = true;
    return;
}

tuple<Samurai *, string> Headquarter::Build_SA()
{
    if (stopped)
        return make_tuple<Samurai *, string>(NULL, "");
    
    if (OnWitch >= Order.size())
        OnWitch -= Order.size();
    Samurai *tmp = Order[OnWitch]->generate((this));
    if (tmp)
    {
        HealthPoint -= tmp->getHP();
        ++Count;
        ++ExistNumber[OnWitch];
        ++OnWitch;
        string ret = tmp->borninfo();
        if (ret != "")
            ret = tmp->getfullname() + " born\n" + ret;
        else
            ret = tmp->getfullname() + " born";
        return make_tuple(tmp, ret);
    }
    ++OnWitch;
    Stop();
    return make_tuple<Samurai *, string>(NULL, "");
}

Headquarter::~Headquarter(){
    // fprintf(stderr,"%u\n", Order.size());
    // for (auto x:Order){
    //     fprintf(stderr, "name : %s\n", x->getname().c_str());
    //     delete x;
    // }
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
    city = new SamuraiSet[n + 2];
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
        city[HeadA.get_pos()].insert(nS), nS->setpos(HeadA.get_pos());

    tie(nS, log) = HeadB.Build_SA();
    if (nS && log.length())
        output.push(HeadB.get_outputlevel(), T.sPrint() + ' ' + log + '\n');
    if (nS)
        city[HeadB.get_pos()].insert(nS), nS->setpos(HeadB.get_pos());
}

int BattleField::MoveTurn()
{
    OrderedOutput output;
    for (int i = 0; i <= Size; ++i)
    {
		vector<Samurai*> tmp;
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
                }
            }
        }
		for (auto &x : tmp) city[i].erase(x);
    }

    for (int i = Size; i >= 0; --i)
    {
		vector<Samurai*> tmp;
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
                }
            }
        }
		for (auto &x : tmp) city[i].erase(x);
    }

    for (int i = 1; i < Size; ++i)
    {
        for (auto x : city[i])
        {
            stringstream tp;
            tp << T.sPrint() << ' ' << x->getfullname() << " marched to city " << i << " with "
               << x->getHP() << " elements and force " << x->getAtk() << '\n';
            output.push(x->get_outputlevel(), tp.str());
        }
    }
    for (auto x : city[0])
    {
        stringstream tp;
        tp << T.sPrint() << ' ' << x->getfullname() << " reached " << HeadA.getname() << " headquarter with "
           << x->getHP() << " elements and force " << x->getAtk() << '\n';
        output.push(x->get_outputlevel(), tp.str());
    }
    for (auto x : city[Size])
    {
        stringstream tp;
        tp << T.sPrint() << ' ' << x->getfullname() << " reached " << HeadB.getname() << " headquarter with "
           << x->getHP() << " elements and force " << x->getAtk() << '\n';
        output.push(x->get_outputlevel(), tp.str());
    }
    if (city[0].size())
        output.push(50, T.sPrint() + ' ' + HeadA.getname() + " headquarter was taken\n");
    if (city[Size].size())
        output.push(100 * Size + 50, T.sPrint() + ' ' + HeadB.getname() + " headquarter was taken\n");
    if (city[0].size() || city[Size].size())
    {
        output.flush();
        return -1;
    }
}

void BattleField::BattleTurn()
{

    OrderedOutput output;
    for (int i = 0; i <= Size; ++i)
        if (city[i].size() == 2)
        {

            Samurai *a = *city[i].begin();
            Samurai *b = *city[i].rbegin();
            (a->getbag()).preliminary();
            (b->getbag()).preliminary();
            if (!BattleFirst(a, b))
                swap(a, b);

            //Process
            int flg = max(a->getbag().size(), b->getbag().size()) * 2 + 3;
            while ((a->getbag().size() || b->getbag().size()) && flg)
            {
                --flg;
                Weapon *x = a->getbag().renext();
				if (x == NULL) {
					swap(a, b);
					continue;
				}
                if (x->attack(a, b) != 0)
                    flg = max(a->getbag().size(), b->getbag().size()) * 2 + 3;
                if (a->isdead() || b->isdead())
                    break;
                swap(a, b);
            }
            //Confirm result
            if (a->isdead() ^ b->isdead())
            {
                if (a->isdead())
                    swap(a, b);

                output.push(i * 100, T.sPrint() + ' ' + a->getfullname() + " killed " + b->getfullname() + " in city " +
                                         to_string(i) + " remaining " + to_string(a->getHP()) + " elements\n");
                //Rob weapons
                WeaponBag &bag = b->getbag();
                bag.preliminary(Weapon::RobCMP);
                while (bag.size())
                {
                    Weapon *x = bag.next();
                    if (x == NULL)
                        break;
                    if (a->getbag().push(x))
                        bag.pop_top();
                    else
                        break;
                }
            }
            else
            {
                if (a->get_outputlevel() > b->get_outputlevel())
                    swap(a, b);
                if (a->isdead())
                {
                    output.push(i * 100, T.sPrint() + " both " + a->getfullname() + " and " + b->getfullname() +
                                             " died in city " + to_string(i) + "\n");
                }
                else
                {
                    output.push(i * 100, T.sPrint() + " both " + a->getfullname() + " and " + b->getfullname() +
                                             " were alive in city " + to_string(i) + "\n");
                }
            }

            //Delete dead samurais and yelled
            if (a->isdead())
            {
                city[a->getpos()].erase(a);
                delete a;
            }
            else
            {
                string log = a->yelled();
                if (log != "")
                    output.push(a->get_outputlevel(), T.sPrint() + ' ' + log);
            }
            if (b->isdead())
            {
                city[b->getpos()].erase(b);
                delete b;
            }
            else
            {
                string log = b->yelled();
                if (log != "")
                    output.push(b->get_outputlevel(), T.sPrint() + ' ' + log);
            }
        }
}

void BattleField::Run(TIME limit)
{
    for (;;)
    {
        OrderedOutput output;

        BuildTurn();
        T.inc(5);
        if (T>limit) break;
        /********* Escape ********/
        {
            for (int i = 0; i <= Size; ++i)
            {
				vector<Samurai*> trash;
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
				for (auto& x : trash) city[i].erase(x);
            }
            output.flush();
        }

        T.inc(5);
        if (T>limit) break;
        if (MoveTurn() == -1)
            break;

        T.inc(25);
        if (T>limit) break;
        /********* Wolf robs weapons *********/
        {
            for (int i = 0; i <= Size; ++i)
            {
                if (city[i].size() == 2)
                {
                    Samurai *a = *city[i].begin();
                    Samurai *b = *city[i].rbegin();
                    string log = a->rob(b);
                    if (log != "")
                        output.push(i, T.sPrint() + ' ' + log + '\n');
                    log = b->rob(a);
                    if (log != "")
                        output.push(i, T.sPrint() + ' ' + log + '\n');
                }
            }
            output.flush();
        }

        T.inc(5);
        if (T>limit) break;
        /********* Battle Begin *********/
        BattleTurn();

        /********* Report **********/
        T.inc(10);
        if (T>limit) break;
        cout << T.sPrint() << ' ' << to_string(HeadA.getHP()) << " elements in " << HeadA.getname() << " headquarter\n";
        cout << T.sPrint() << ' ' << to_string(HeadB.getHP()) << " elements in " << HeadB.getname() << " headquarter\n";

        T.inc(5);
        if (T>limit) break;
        for (int i = 0; i <= Size; ++i)
            for (auto x : city[i])
                output.push(x->get_outputlevel(), T.sPrint() + ' ' + x->report() + '\n');
        output.flush();

        T.inc(5);
        if (T>limit) break;
    }
}

BattleField::~BattleField(){
    // for (int i=0;i<Size;++i){
    //     for (auto x: city[i]) delete x;
    // }
}

int Sword::attack(Samurai *a, Samurai *b)
{
    int tmp = getratio()*(a->getAtk());
    b->hurted(tmp);
    if (tmp>0) return 1;
    else return 0;
}

int Bomb::attack(Samurai *a, Samurai *b){
    if (times<=0) return 4;
    --times;
    int tmp = getratio()*(a->getAtk());
    int ret=4;
	if (a->getname() != "ninja") {
		a->hurted(tmp*0.5);
		if (tmp>1) ret |= 2;
	}
    b->hurted(tmp);
    if (tmp>0) ret|=1;
    return ret;
}

int Arrow::attack(Samurai *a, Samurai *b){
    if (times<=0) return 4;
    --times;
    int tmp = getratio()*(a->getAtk());
    b->hurted(tmp);
    if (tmp>0) return 5;
    else return 4;
}

Samurai::Samurai(Headquarter *_B, int _number, int _HP, int _Atk) : belong(_B), Number(_number), HealthPoint(_HP), Attack(_Atk){};
string Samurai::getfullname()const { return belong->getname() + ' ' + getname() + ' ' + to_string(Number); }
int Samurai::get_outputlevel() const{ return Position * 100 + belong->get_outputlevel(); }
bool BattleFirst(Samurai *a, Samurai *b) { return (a->getpos() & 1) && (a->get_belong()->get_pos() == 0) || (~a->getpos() & 1) && (a->get_belong()->get_pos() != 0); }
int Samurai::get_direct()const
{
	return belong->get_direct();
}
Headquarter *Samurai::get_belong()const
{
    return belong;
}
bool Samurai::addweapon(Weapon* x){

	return bag.push(x);
}


Dragon::Dragon(Headquarter *_B, int _number, int _HP, int _Atk, double _morale) : Samurai(_B, _number, _HP, _Atk), morale(_morale){};
Samurai *Dragon::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	Samurai* ret=new Dragon(info, cnt, getHP(), getAtk(), (double)info->getHP() / getHP() - 1);
	ret->addweapon(info->getweapon(cnt % 3));
	return ret;
}
string Dragon::yelled(){
	return getfullname()+" yelled in city " + to_string(getpos())+'\n';
}

Ninja::Ninja(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
Samurai *Ninja::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	Samurai* ret = new Ninja(info, cnt, getHP(), getAtk() );
	ret->addweapon(info->getweapon(cnt % 3));
	ret->addweapon(info->getweapon((cnt + 1) % 3));
	return ret;
}

Iceman::Iceman(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
Samurai *Iceman::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	Samurai* ret= new Iceman(info, cnt, getHP(), getAtk());
	ret->addweapon(info->getweapon(cnt % 3));
	return ret;
}

Lion::Lion(Headquarter *_B, int _number, int _HP, int _Atk, int _loyalty, int _LDec) : Samurai(_B, _number, _HP, _Atk), loyalty(_loyalty), LDec(_LDec){};
string Lion::borninfo() const
{
	return "Its loyalty is " + to_string(loyalty);
}
Samurai *Lion::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;

	Samurai* ret= new Lion(info, cnt, getHP(), getAtk(), info->getHP() - getHP(), LDec);
	ret->addweapon(info->getweapon(cnt % 3));

	return ret;
}
string Lion::escape(){

	if (loyalty>0)
		return "";
	else 
		return getfullname() + " ran away";
}

Wolf::Wolf(Headquarter *_B, int _number, int _HP, int _Atk) : Samurai(_B, _number, _HP, _Atk){};
Samurai *Wolf::generate(Headquarter *info) const
{
	if (info->getHP() < getHP())
		return NULL;
	int cnt = info->getCount() + 1;
	return new Wolf(info, cnt, getHP(), getAtk());
}
string Wolf::rob(Samurai* x){
	if (x->getname()=="wolf") return "";
	WeaponBag &tmp = x->getbag();
	tmp.preliminary(Weapon::RobCMP);
	int t=0;
	Weapon* first = NULL;
	for (;t<tmp.size();++t){
		Weapon* now = tmp.next();
		if (first==NULL) first=now;
		if (now->getnumber()!=first->getnumber()) break;
		if (!addweapon(now)) break;
	}
	tmp.pop_top(t);
	if (t==0) return "";
	return getfullname() + " took " + to_string(t) + ' ' +first->getname() +" from " + x->getfullname() + " in city " + to_string(getpos());
}

int main()
{
	freopen("datapub.in", "r", stdin);
	freopen("out.out", "w", stdout);
	int T = 0;
	scanf("%d", &T);
	for (int cas = 1; cas <= T; ++cas)
	{
		printf("Case %d:\n", cas);
		int M, N, K, D;
		scanf("%d%d%d%d", &M, &N, &K, &D);
		int hp[5], atk[5];
		for (int i = 0; i < 5; ++i)
			scanf("%d", &hp[i]);
		for (int i = 0; i < 5; ++i)
			scanf("%d", &atk[i]);

		vector<Samurai *> AS({new Iceman(NULL, 0, hp[2], atk[2]), new Lion(NULL, 0, hp[3], atk[3], 0, K),
							  new Wolf(NULL, 0, hp[4], atk[4]), new Ninja(NULL, 0, hp[1], atk[1]), new Dragon(NULL, 0, hp[0], atk[0], 0.0)});
		vector<Samurai *> BS({new Lion(NULL, 0, hp[3], atk[3], 0, K), new Dragon(NULL, 0, hp[0], atk[0], 0.0), new Ninja(NULL, 0, hp[1], atk[1]),
							  new Iceman(NULL, 0, hp[2], atk[2]), new Wolf(NULL, 0, hp[4], atk[4])});
		vector<function<Weapon *()>> W;
		W.push_back([]() -> Weapon * { return (Weapon *)new Sword("sword", 0.2); });
		W.push_back([]() -> Weapon * { return (Weapon *)new Bomb("bomb", 0.4, 1); });
		W.push_back([]() -> Weapon * { return (Weapon *)new Arrow("arrow", 0.3, 2); });

		Weapon* test = (Weapon*) new Arrow();

		Headquarter red("red", M, AS, W), blue("blue", M, BS, W);

		BattleField B(N, red, blue);
		B.Run(D);
	}
	return 0;
}