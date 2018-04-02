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
#include <set>
#include <tuple>
#include "weapon.h"
#include "samurai.h"
using namespace std;
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
				const vector<function<Weapon *()>> &_Weapons) : name(_name), HealthPoint(_HP),
																Order(_Order), ExistNumber(_Order.size(), 0),
																Weapons(_Weapons){};
	void Stop();
	bool isstopped() { return stopped; }
	tuple<Samurai *, string> build_Sa();
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
	string getname(){return name;}
};

void Headquarter::Stop()
{
	if (stopped)
		return;
	stopped = true;
	return;
}

tuple<Samurai *, string> Headquarter:: build_Sa()
{
	if (stopped)
		return {NULL, ""};
	for (int i = 0; i < Order.size(); ++i)
	{
		if (OnWitch >= Order.size())
			OnWitch -= Order.size();
		Samurai *tmp = Order[OnWitch]->generate(this);
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
			return {tmp, ret.str()};
		}
		++OnWitch;
	}
	Stop();
	return {NULL, ""};
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
			tie(nS, log) = HeadA.build_Sa();
			if (nS && log.length())
				output.push(outCount++, T.sPrint() + ' ' + log + '\n');
			if (nS)
				city[HeadA.get_pos()].insert(nS);
			tie(nS, log) = HeadB.build_Sa();
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
							output.push(outCount+aim*100+x->get_belong()->get_outputlevel(), tp.str());
						}
					}
				}
			}
			for (int i = Size; i >=0; --i)
			{
				for (auto x : city[i])
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
							output.push(outCount+aim*100+x->get_belong()->get_outputlevel(), tp.str());
						}
					}
				}
			}

		}
	}
};