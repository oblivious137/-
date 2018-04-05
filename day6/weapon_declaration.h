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
#include "WarcraftClassDeclaration.h"
using namespace std;

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
	virtual int getnumber()=0;
	virtual int priority(){return 0;}
};

class Sword:Weapon{
	public:
	virtual int getnumber(){return 0;}
};

class Bomb:Weapon{
	public:
	virtual int getnumber(){return 1;}
};

class Arrow:Weapon{
	int times;
	public:
	virtual int getnumber(){return 2;}
	int priority(){return times;}
};

class WeaponBag{
	vector<Weapon*> weapons;
	int VolumeLimit;
	vector<Weapon*>::iterator _now;

	public:
	void set_limit(int x){VolumeLimit=x;}
	void preliminary(bool (*x)(Weapon*,Weapon*) = [](Weapon*a,Weapon*b) -> bool{
		if (a->getnumber()!=b->getnumber()) return a->getnumber()<b->getnumber();
		return a->priority()<b->priority();
	}){sort(weapons.begin(),weapons.end(),x);_now=weapons.begin();}
	Weapon* next(){return *(_now++);}
	bool push(Weapon* x){return (weapons.size()<VolumeLimit) ? weapons.push_back(x),true : false;}
	int size(){return weapons.size();}
	void pop_top(int x=1){weapons.erase(weapons.begin(),weapons.begin()+x);}
	~WeaponBag(){
		for (auto&x:weapons) delete x;
	}
};