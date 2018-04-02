#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;

class Weapon;
class Samurai;
class Headquarter;

class Weapon
{
private:
	string name;
	int attack;

public:
	Weapon() = default;
	Weapon(const std::string &_name = "", int _attack = 0) : name(_name), attack(_attack) {};
	void setattack(int a) {};  /*************/
	void setname(string a) {}; /*************/
	string getname() { return name; }
	int getattack() { return attack; }
};

class Samurai
{
	Headquarter *belong;
	int Number, HealthPoint, Attack;

public:
	Samurai(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0) : belong(_B), Number(_number), HealthPoint(_HP), Attack(_Atk) {};
	virtual string getname() const = 0;
	int getHP() const { return HealthPoint; }
	int getnumber() const { return Number; }
	int getAtk() const { return Attack; }
	virtual string getinfo() const = 0;
	virtual Samurai *generate(Headquarter *) const = 0;
	virtual ~Samurai() = default;
	Headquarter* get_belong() { return belong; }
	int get_direct() { return belong->get_direct(); }
	string escape() { return ""; }
};

class Dragon : public Samurai
{
	Weapon *weapon;
	double morale;

public:
	Dragon(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0) : Samurai(_B, _number, _HP, _Atk){};
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
	Iceman(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, Weapon *_weapon = NULL) : Samurai(_B, _number, _HP, _Atk), weapon(_weapon) {};
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
	Lion(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0, int _loyalty = 0) : Samurai(_B, _number, _HP, _Atk), loyalty(_loyalty) {};
	virtual string getname() const { return "lion"; }
	virtual ~Lion() = default;
	virtual string getinfo() const;
	Samurai *generate(Headquarter *info) const;
};

class Wolf : public Samurai
{

public:
	Wolf(Headquarter *_B = NULL, int _number = 0, int _HP = 0, int _Atk = 0) : Samurai(_B, _number, _HP, _Atk) {};
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

class Headquarter
{
private:
	string name;
	vector<Samurai* > Order;
	vector<function<Weapon *()> > Weapons;
	vector<int> ExistNumber;
	int HealthPoint, OnWitch = 0, Count = 0;
	bool stopped = false;
	int posi, dire, outputlevel;

public:
	Headquarter(const string &_name, int _HP,
		const vector<Samurai *> &_Order,
		const vector<function<Weapon *()> > &_Weapons) : name(_name), HealthPoint(_HP),
		Order(_Order), ExistNumber(_Order.size(), 0),
		Weapons(_Weapons) {};
	void Stop();
	bool isstopped() { return stopped; }
	tuple<Samurai *, string> Build_SA();
	unsigned int getOrderSize() { return Order.size(); }
	unsigned int getWeaponsSize() { return Weapons.size(); }
	int getCount() const { return Count; }
	void changeCount(int x) { Count = x; }
	int getHP() const { return HealthPoint; }
	void changeHP(int x) { HealthPoint = x; }
	Weapon *getweapon(int x) const { return Weapons[x](); }
	int get_pos() { return posi; }
	void set_pos(int x) { posi = x; }
	int get_direct() { return dire; }
	void set_direct(int x) { dire = x; }
	int get_outputlevel() { return outputlevel; }
	void set_outputlevel(int x) { outputlevel = x; }
	string getname() { return name; }
};

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
		return { NULL, "" };
	for (int i = 0; i < Order.size(); ++i)
	{
		if (OnWitch >= Order.size())
			OnWitch -= Order.size();
		Samurai *tmp = Order[OnWitch]->generate((this));
		if (tmp)
		{
			HealthPoint -= tmp->getHP();
			++Count;
			++ExistNumber[OnWitch];
			stringstream ret;

			ret << name << ' ' << tmp->getname()
				<< ' ' << tmp->getnumber() << " born with strength " << tmp->getHP() << ","
				<< ExistNumber[OnWitch] << " " << tmp->getname() << " in " << name << " headquarter" << endl;
			string tp = tmp->getinfo();
			if (tp != "")
				ret << tp;
			++OnWitch;
			return { tmp, ret.str() };
		}
		++OnWitch;
	}
	Stop();
	return { NULL, "" };
}

class TIME
{
	int minutes;

public:
	TIME() = default;
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
};

class _OUTPUT_CMP
{
public:
	bool operator()(Samurai *const a, Samurai *const b)
	{
		if (a->get_belong()->get_outputlevel() == b->get_belong()->get_outputlevel())
			return a->get_belong()->get_outputlevel() < b->get_belong()->get_outputlevel();
		else
			return a < b;
	}
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
	typedef set<Samurai *, _OUTPUT_CMP> SamuraiSet;
	SamuraiSet *city;

public:
	BattleField(int n, Headquarter a, Headquarter b) : Size(n + 1), HeadA(a), HeadB(b)
	{
		HeadA.set_pos(0);
		HeadB.set_pos(n + 1);
		HeadA.set_direct(1);
		HeadB.set_direct(-1);
		HeadA.set_outputlevel(1);
		HeadB.set_outputlevel(2);
		city = new SamuraiSet[n + 2];
	};
	void Run()
	{
		for (;;)
		{
			OrderedOutput output;
			string log;
			Samurai *nS;
			int outCount = 0;
			tie(nS, log) = HeadA.Build_SA();
			if (nS && log.length())
				output.push(outCount++, T.sPrint() + ' ' + log + '\n');
			if (nS)
				city[HeadA.get_pos()].insert(nS);
			tie(nS, log) = HeadB.Build_SA();
			if (nS && log.length())
				output.push(outCount++, T.sPrint() + ' ' + log + '\n');
			if (nS)
				city[HeadB.get_pos()].insert(nS);
			T.inc(5);
			for (int i = 0; i <= Size; ++i)
			{
				string tmp;
				for (auto x : city[i])
				{
					if ((tmp = x->escape()) != "")
					{
						city[i].erase(x);
						delete x;
						output.push(outCount++, T.sPrint() + ' ' + tmp + "\n");
					}
				}
			}
			T.inc(5);
			++outCount;
			for (int i = 0; i <= Size; ++i)
			{
				for (auto x : city[i])
				{
					if (x->get_direct() < 0)
					{
						int aim = i + x->get_direct();
						city[i].erase(x);
						if (aim >= 0 && aim <= Size)
						{
							city[aim].insert(x);
							stringstream tp;
							tp << T.sPrint() << ' ' << x->get_belong()->getname() << ' ' << x->getname()
								<< ' ' << x->getnumber() << " marched to city " << aim << " with "
								<< x->getHP() << " elements and force " << x->getAtk() << '\n';
							output.push(outCount + aim * 100 + x->get_belong()->get_outputlevel(), tp.str());
						}
					}
				}
			}
			for (int i = Size; i >= 0; --i)
			{
				for (auto x : (city[i]))
				{
					if (x->get_direct() > 0)
					{
						int aim = i + x->get_direct();
						city[i].erase(x);
						if (aim >= 0 && aim <= Size)
						{
							city[aim].insert(x);
							stringstream tp;
							tp << T.sPrint() << ' ' << x->get_belong()->getname() << ' ' << x->getname()
								<< ' ' << x->getnumber() << " marched to city " << aim << " with "
								<< x->getHP() << " elements and force " << x->getAtk() << '\n';
							output.push(outCount + aim * 100 + x->get_belong()->get_outputlevel(), tp.str());
						}
					}
				}
			}

		}
	}
};

int main()
{
	// int T = 0;
	// scanf("%d", &T);
	// for (int cas = 1; cas <= T; ++cas)
	// {
	// 	printf("Case:%d\n", cas);
	// 	int tmp;
	// 	scanf("%d", &tmp);
	// 	int hp[5];
	// 	for (int i = 0; i < 5; ++i)
	// 		scanf("%d", &hp[i]);
	// 	vector<Samurai *> AS({ new Iceman(0, hp[2]), new Lion(0, hp[3]),
	// 		new Wolf(0, hp[4]), new Ninja(0, hp[1]), new Dragon(0, hp[0]) });
	// 	vector<Samurai *> BS({ new Lion(0, hp[3]), new Dragon(0, hp[0]),
	// 		new Ninja(0, hp[1]), new Iceman(0, hp[2]), new Wolf(0, hp[4]) });
	// 	vector<function<Weapon *()>> W({ []() { return new Weapon("sword"); },
	// 		[]() { return new Weapon("bomb"); },
	// 		[]() { return new Weapon("arrow"); } });
	// 	Headquarter red("red", tmp, AS, W), blue("blue", tmp, BS, W);
	// 	BattleField B(size, red, blue);
	// 	for (;;) {

	// 	}
	// }
	return 0;
}