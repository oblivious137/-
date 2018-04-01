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

public:
	Headquarter(const string &_name, int _HP,
		const vector<Samurai *>& _Order,
		const vector<function<Weapon *()>>& _Weapons) : name(_name), HealthPoint(_HP),
		Order(_Order), ExistNumber(_Order.size(), 0),
		Weapons(_Weapons) {};
	void Stop();
	bool isstopped() { return stopped; }
	string build_Sa();
	unsigned int getOrderSize() { return Order.size(); }
	unsigned int getWeaponsSize() { return Weapons.size(); }
	int getCount() const { return Count; }
	void changeCount(int x) { Count = x; }
	int getHP() const { return HealthPoint; }
	void changeHP(int x) { HealthPoint = x; }
	Weapon *getweapon(int x) const { return Weapons[x](); }
};

void Headquarter::Stop()
{
	if (stopped)
		return;
	stopped = true;
	return;
}

string Headquarter::build_Sa()
{
	if (stopped)
		return "";
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
			delete tmp;
			return ret.str();
		}
		++OnWitch;
	}
	Stop();
	return "";
}

class TIME {
	int minutes;
public:
	TIME() = default;
	void inc(int a) { minutes += a; }
	void Print() {
		printf("%03d:%02d", minutes / 60, minutes % 60);
	}
	string sPrint() {
		char tmp[10];
		sprintf(tmp, "%03d:%02d", minutes / 60, minutes % 60);
		return tmp;
	}
};


class BattleField {
	TIME T;
	int Size;
	Headquarter A,B;
	set<Samurai*> *city;
public:
	BattleField(int n, Headquarter a, Headquarter b) :Size(n+1), A(a), B(b) {
		city = new set<Samurai*>[n + 2];
	};
	void Run() {
		for (;;) {
			string tmp = A.build_Sa();
			if (tmp.length()) cout<<T.sPrint()<<' '<<
			T.inc()
			for (int )
		}
	}
};