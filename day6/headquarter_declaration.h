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
#include "WarcraftClassDeclaration.h"
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
	int get_pos() { return posi; }
	void set_pos(int x) { posi = x; }
	int get_direct() { return dire; }
	void set_direct(int x) { dire = x; }
	int get_outputlevel() { return outputlevel; }
	void set_outputlevel(int x) { outputlevel = x; }
	string getname() { return name; }
};

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
	bool operator()(Samurai *const a, Samurai *const b);
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
	BattleField(int n, Headquarter a, Headquarter b);
	void Run();
};