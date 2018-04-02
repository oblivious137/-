#include "samurai_define.h"

int Samurai::get_direct()
{
    return belong->get_direct();
}

Headquarter *Samurai::get_belong()
{
    return belong;
}

string Dragon::getinfo() const
{
	stringstream tmp;
	tmp << "It has a " << weapon->getname() << ",and it's morale is " << fixed << setprecision(2) << morale;
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