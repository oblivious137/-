#include <map>
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

class Headquarter;
class Weapon;
class Samurai;
class Dragon;
class Ninja;
class Iceman;
class Lion;
class Wolf;
extern int TIME;

class Weapon
{
  private:
    string name;
    int attack;

  public:
    Weapon() = default;
    Weapon(const string &_name = "", int _attack = 0) : name(_name), attack(_attack){};
    void setattack(int a);  /*************/
    void setname(string a); /*************/
    string getname() { return name; }
    int getattack() { return attack; }
};

class Samurai
{
    int Number, HealthPoint;

  public:
    Samurai(int _number = 0, int _HP = 0) : Number(_number), HealthPoint(_HP){};
    virtual string getname() const = 0;
    int getHP() const { return HealthPoint; }
    int getnumber() const { return Number; }
    virtual string getinfo() const = 0;
    virtual Samurai *generate(Headquarter *) const = 0;
    virtual ~Samurai() = default;
};

class Dragon : public Samurai
{
    Weapon *weapon;
    double morale;

  public:
    Dragon(int _number = 0, int _HP = 0, double _morale = 0, Weapon *_weapon = NULL) : Samurai(_number, _HP), morale(_morale), weapon(_weapon){};
    string getweaponname() const { return weapon->getname(); }
    virtual string getname() const { return "dragon"; }
    virtual ~Dragon()
    {
        if (weapon)
            delete weapon;
    }
    virtual string getinfo() const;
    virtual Samurai *generate(Headquarter *info) const;
};

class Ninja : public Samurai
{
    Weapon *weapon[2];

  public:
    Ninja(int _number = 0, int _HP = 0, Weapon *_weapon1 = NULL, Weapon *_weapon2 = NULL) : Samurai(_number, _HP)
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
    Iceman(int _number = 0, int _HP = 0, Weapon *_weapon = NULL) : Samurai(_number, _HP), weapon(_weapon){};
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
    Lion(int _number = 0, int _HP = 0, int _loyalty = 0) : Samurai(_number, _HP), loyalty(_loyalty){};
    virtual string getname() const { return "lion"; }
    virtual ~Lion() = default;
    virtual string getinfo() const;
    Samurai *generate(Headquarter *info) const;
};

class Wolf : public Samurai
{

  public:
    Wolf(int _number = 0, int _HP = 0) : Samurai(_number, _HP){};
    virtual string getname() const { return "wolf"; }
    virtual ~Wolf() = default;
    virtual string getinfo() const;
    Samurai *generate(Headquarter *info) const;
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

  public:
    Headquarter(const string &_name, int _HP,
                const vector<Samurai *>& _Order,
                const vector<function<Weapon *()>>& _Weapons) : name(_name), HealthPoint(_HP),
                                                               Order(_Order), ExistNumber(_Order.size(), 0),
                                                               Weapons(_Weapons){};
    void Stop();
    bool isstopped() { return stopped; }
    void next_turn();
    unsigned int getOrderSize() { return Order.size(); }
    unsigned int getWeaponsSize() { return Weapons.size(); }
    int getCount() const { return Count; }
    void changeCount(int x) { Count = x; }
    int getHP() const { return HealthPoint; }
    void changeHP(int x) { HealthPoint = x; }
    Weapon *getweapon(int x) const { return Weapons[x](); }
};

string Dragon::getinfo() const
{
    stringstream tmp;
    tmp<<"It has a "<<getweaponname()<<",and it's morale is "<<fixed<<setprecision(2)<<morale;
    return tmp.str();
}
Samurai *Dragon::generate(Headquarter *info) const
{
    //int _number, int _HP = 0, double _morale = 0, Weapon *_weapon = NULL
    if (info->getHP() < getHP())
        return NULL;
    int cnt = info->getCount() + 1;
    return new Dragon(cnt, getHP(), (double)info->getHP() / getHP() - 1, info->getweapon(cnt % 3));
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
    //int _number, int _HP = 0, Weapon *_weapon1 = NULL, Weapon *_weapon2 = NULL
    if (info->getHP() < getHP())
        return NULL;
    int cnt = info->getCount() + 1;
    return new Ninja(cnt, getHP(), info->getweapon(cnt % 3), info->getweapon((cnt + 1) % 3));
}

string Iceman::getinfo() const
{
    return "It has a " + ((weapon != NULL) ? weapon->getname() : "");
}
Samurai *Iceman::generate(Headquarter *info) const
{
    //int _number, int _HP = 0, Weapon *_weapon = NULL
    if (info->getHP() < getHP())
        return NULL;
    int cnt = info->getCount() + 1;
    return new Iceman(cnt, getHP(), info->getweapon(cnt % 3));
}

string Lion::getinfo() const
{
    return "It's loyalty is " + to_string(loyalty);
}
Samurai *Lion::generate(Headquarter *info) const
{
    //int _number, int _HP = 0, int _loyalty = 0
    if (info->getHP() < getHP())
        return NULL;
    int cnt = info->getCount() + 1;
    return new Lion(cnt, getHP(), info->getHP() - getHP());
}

string Wolf::getinfo() const
{
    return "";
}
Samurai *Wolf::generate(Headquarter *info) const
{
    //int _number, int _HP = 0, double _morale = 0, Weapon *_weapon = NULL
    if (info->getHP() < getHP())
        return NULL;
    int cnt = info->getCount() + 1;
    return new Wolf(cnt, getHP());
}

void Headquarter::Stop()
{
    if (stopped)
        return;
    printf("%03d %s headquarter stops making warriors\n", TIME, name.c_str());
    stopped = true;
    return;
}

void Headquarter::next_turn()
{
    if (stopped)
        return;
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
            cout << setw(3) << setfill('0') << TIME << ' ' << name << ' ' << tmp->getname()
                 << ' ' << tmp->getnumber() << " born with strength " << tmp->getHP() << ","
                 << ExistNumber[OnWitch] << " " << tmp->getname() << " in " << name << " headquarter" << endl;
            string tp = tmp->getinfo();
            if (tp != "")
                cout << tp << endl;
            ++OnWitch;
            delete tmp;
            return;
        }
        ++OnWitch;
    }
    Stop();
    return;
}

int TIME;

int main()
{
    int T = 0;
    scanf("%d", &T);
    for (int cas = 1; cas <= T; ++cas)
    {
        printf("Case:%d\n", cas);
        int tmp;
        scanf("%d", &tmp);
        int hp[5];
        for (int i = 0; i < 5; ++i)
            scanf("%d", &hp[i]);
        vector<Samurai *> AS({new Iceman(0, hp[2]), new Lion(0, hp[3]),
                              new Wolf(0, hp[4]), new Ninja(0, hp[1]), new Dragon(0, hp[0])});
        vector<Samurai *> BS({new Lion(0, hp[3]), new Dragon(0, hp[0]),
                              new Ninja(0, hp[1]), new Iceman(0, hp[2]), new Wolf(0, hp[4])});
        vector<function<Weapon *()>> W({[]() { return new Weapon("sword"); },
                                        []() { return new Weapon("bomb"); },
                                        []() { return new Weapon("arrow"); }});
        Headquarter red("red", tmp, AS, W), blue("blue", tmp, BS, W);
        TIME = 0;
        while (!red.isstopped() || !blue.isstopped())
        {
            red.next_turn();
            blue.next_turn();
            ++TIME;
        }
    }
    return 0;
}
/*
Case:1
000 red iceman 1 born with strength 5,1 iceman in red headquarter
It has a bomb
000 blue lion 1 born with strength 6,1 lion in blue headquarter
It's loyalty is 14
001 red lion 2 born with strength 6,1 lion in red headquarter
It's loyalty is 9
001 blue dragon 2 born with strength 3,1 dragon in blue headquarter
It has a arrow,and it's morale is 3.67
002 red wolf 3 born with strength 7,1 wolf in red headquarter
002 blue ninja 3 born with strength 4,1 ninja in blue headquarter
It has a sword and a bomb
003 red headquarter stops making warriors
003 blue iceman 4 born with strength 5,1 iceman in blue headquarter
It has a bomb
004 blue headquarter stops making warriors
*/