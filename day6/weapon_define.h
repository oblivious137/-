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
};