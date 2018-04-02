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
	int posi;

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
	int get_pos(){return posi;}
	void set_pos(int x){posi=x;}
};

void Headquarter::Stop()
{
	if (stopped)
		return;
	stopped = true;
	return;
}

tuple<Samurai *, string> Headquarter::build_Sa()
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

class BattleField
{
	TIME T;
	int Size;
	Headquarter A, B;
	set<Samurai *> *city;

  public:
	BattleField(int n, Headquarter a, Headquarter b) : Size(n + 1), A(a), B(b)
	{
		A.set_pos(0);
		B.set_pos(n+1);
		city = new set<Samurai *>[n + 2];
	};
	void Run()
	{
		for (;;)
		{
			string log;
			Samurai *nS;
			tie(nS, log) = A.build_Sa();
			if (nS&&log.length())
				cout << T.sPrint() << ' ' << log << endl;
			if (nS) city[A.get_pos()].insert(nS);
			tie(nS, log) = B.build_Sa();
			if (nS&&log.length())
				cout << T.sPrint() << ' ' << log << endl;

			for (int)
		}
	}
};