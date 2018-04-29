#pragma once
#include "headquarter_declaration.h"
#include "samurai_declaration.h"
#include "weapon_declaration.h"

/********* Headquarter ***********/

Headquarter::Headquarter(const string &_name, int _HP,
                         const vector<function<Samurai *(Headquarter *)>> &_Order,
                         const vector<function<Weapon *(Samurai *)>> &_Weapons) : name(_name), HealthPoint(_HP), Order(_Order),
                                                                                  Weapons(_Weapons), ExistNumber(_Order.size(), 0){};
Headquarter::Headquarter(const Headquarter &a) : name(a.name), HealthPoint(a.HealthPoint), Order(a.Order), Weapons(a.Weapons), ExistNumber(a.ExistNumber) {}

int Headquarter::generateNextSamurai(int _HP)
{
    HealthPoint -= _HP;
    return ++Count;
}

unsigned int Headquarter::getOrderSize() { return Order.size(); }
Weapon *Headquarter::getweapon(int x, Samurai *who) const
{
    return Weapons[x](who);
}

tuple<Samurai *, string> Headquarter::Build_SA()
{
    Samurai *tmp = Order[OnWitch](this);
    if (tmp)
    {
        ++ExistNumber[OnWitch];
        ++OnWitch;
        if (OnWitch >= Order.size())
            OnWitch -= Order.size();
        string ret = tmp->borninfo();
        if (ret != "")
            ret = tmp->getfullname() + " born\n" + ret;
        else
            ret = tmp->getfullname() + " born";
        return make_tuple(tmp, ret);
    }
    return make_tuple<Samurai *, string>(NULL, "");
}

void Headquarter::clearque()
{
    sort(que.begin(), que.end(), [this](const quetype &x, const quetype &y) {
        if (get<2>(x) != get<2>(y))
            return get<2>(x) > get<2>(y);
        return abs(get<0>(x) - get_pos()) > abs(get<0>(y) - get_pos());
    });
    for (auto x : que)
    {
        if (get<2>(x) > 0 && getHP() >= get<2>(x))
        {
            get<1>(x)->hurted(-get<2>(x));
            changeHP(getHP() - get<2>(x));
        }
        else if (get<2>(x) < 0)
        {
            changeHP(getHP() - get<2>(x));
        }
    }
    que.clear();
    return;
}

/******* City *********/

int City::lostHP()
{
    int ret = HP;
    HP = 0;
    return ret;
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
    city = new City[n + 2];
    for (int i = 0; i <= Size; ++i)
    {
        city[i].set_pos(i);
        if (i & 1)
            city[i].setdeffirst(&HeadA);
        else
            city[i].setdeffirst(&HeadB);
    }
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
        vector<Samurai *> tmp;
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
                    stringstream tp;
                    tp << T.sPrint() << ' ' << x->getfullname();
                    if (aim != 0 && aim != Size)
                        tp << " marched to city " << aim;
                    else
                        tp << " reached " << ((aim == 0) ? (HeadA.getname()) : (HeadB.getname())) << " headquarter";
                    tp << " with " << x->getHP() << " elements and force " << x->getAtk() << '\n';
                    output.push(x->get_routputlevel(), tp.str());
                }
            }
        }
        for (auto &x : tmp)
            city[i].erase(x);
    }

    for (int i = Size; i >= 0; --i)
    {
        vector<Samurai *> tmp;
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
                    stringstream tp;
                    tp << T.sPrint() << ' ' << x->getfullname();
                    if (aim != 0 && aim != Size)
                        tp << " marched to city " << aim;
                    else
                        tp << " reached " << ((aim == 0) ? (HeadA.getname()) : (HeadB.getname())) << " headquarter";
                    tp << " with " << x->getHP() << " elements and force " << x->getAtk() << '\n';
                    output.push(x->get_routputlevel(), tp.str());
                }
            }
        }
        for (auto &x : tmp)
            city[i].erase(x);
    }
    if (city[0].size() >= 2)
        output.push(50, T.sPrint() + ' ' + HeadA.getname() + " headquarter was taken\n");
    if (city[Size].size() >= 2)
        output.push(100 * Size + 50, T.sPrint() + ' ' + HeadB.getname() + " headquarter was taken\n");
    output.flush();
    if (city[0].size() >= 2 || city[Size].size() >= 2)
        return -1;
    else
        return 0;
}

bool BattleField::FightFirst(Samurai *a, Samurai *b)
{
    if (city[a->getpos()].conquer() == a->get_belong())
        return true;
    if (city[b->getpos()].conquer() == b->get_belong())
        return false;
    if (city[a->getpos()].deffirst() == a->get_belong())
        return true;
    return false;
}

vector<string> BattleField::Fight(Samurai *a, Samurai *b)
{
    vector<string> ret;
    if (a->isdead() || b->isdead())
        return ret;
    if (!FightFirst(a, b))
        swap(a, b);
    a->Fight(b);
    ret.push_back(a->getfullname() + " attacked " + b->getfullname() + " in city " + to_string(a->getpos()) +" with " + to_string(a->getHP()) + " elements and force " + to_string(a->getAtk()));
    if (!b->isdead())
    {
        b->reFight(a);
        ret.push_back(b->getfullname() + " fought back against " + a->getfullname() + " in city " +to_string(b->getpos()));
    }
    return ret;
}

void BattleField::BattleTurn()
{
    static const int base = 100, fight = 0, dead = 10, yelled = 15, earn = 20, flag = 25;
    OrderedOutput output;
    for (int i = 1; i < Size; ++i)
        if (city[i].size() == 2)
        {

            Samurai *a = *city[i].begin();
            Samurai *b = *city[i].rbegin();
            a->prefight();
            b->prefight();
            vector<string> state = Fight(a, b);
            //Output
            for (int k = 0; k < state.size(); ++k)
                output.push(i * base + fight + k, T.sPrint() + ' ' + state[k] + '\n');

            //win && draw && lose
            if (a->isdead() ^ b->isdead())
            {
                if (a->isdead())
                    swap(a, b);
                a->fightwin(b);
                b->fightlose(a);
            }
            else if (a->isdead())
            {
                a->fightdraw(b, 0);
                b->fightdraw(a, 0);
            }
            else
            {
                a->fightdraw(b, 1);
                b->fightdraw(a, 1);
            }

            //Dead message
            if (a->isdead())
            {
                if (state.size() > 0)
                    output.push(i * base + dead, T.sPrint() + a->getfullname() + " was killed in city " +
                                                     to_string(i) + '\n');
            }
            if (b->isdead())
            {
                if (state.size() > 0)
                    output.push(i * base + dead, T.sPrint() + b->getfullname() + " was killed in city " +
                                                     to_string(i) + '\n');
            }

            //yelled    because draw or win won't change Fight oreder!!!!!!!!!!!!!!!!!!!!!!
            if (!a->isdead() && FightFirst(a, b))
            {
                string log = a->yelled();
                if (log != "")
                    output.push(i * base + yelled, T.sPrint() + ' ' + log + '\n');
            }
            if (!b->isdead() && FightFirst(b, a))
            {
                string log = b->yelled();
                if (log != "")
                    output.push(i * base + yelled, T.sPrint() + ' ' + log + '\n');
            }

            //earn elements
            if (a->isdead() ^ b->isdead())
            {
                if (!b->isdead())
                    swap(a, b);
                int tmp = city[i].lostHP();
                output.push(i * base + earn, T.sPrint() + ' ' + a->getfullname() + " earned " +
                                                 to_string(tmp) + " elements for his headquarter\n");
                a->get_belong()->askforHP(make_tuple(i, a, -tmp));
            }

            //Confirm flag
            if (a->isdead() ^ b->isdead())
            {
                if (a->isdead())
                    swap(a, b);
                string log = city[i].fightresult(a->get_belong());
                if (log.length() > 0)
                    output.push(i * base + flag, T.sPrint() + ' ' + log + '\n');
            }
            else if (!a->isdead())
            {
                string log = city[i].fightresult(NULL);
                if (log.length() > 0)
                    output.push(i * base + flag, T.sPrint() + ' ' + log + '\n');
            }

            //delete dead samurai
            if (a->isdead())
            {
                city[i].erase(a);
                delete a;
            }
            if (b->isdead())
            {
                city[i].erase(b);
                delete b;
            }
        }
    // clear queue
    HeadA.clearque();
    HeadB.clearque();
    return;
}

void BattleField::Run(TIME limit)
{
    for (;;)
    {
        //fprintf(stderr,"xxxxxxxxxx\n");
        OrderedOutput output;
        /********* 0' : Build Samurais ********/
        BuildTurn();
        /********* 5' : Escape ********/
        T.inc(5);
        if (T > limit)
            break;
        {
            for (int i = 0; i <= Size; ++i)
            {
                vector<Samurai *> trash;
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
                for (auto &x : trash)
                    city[i].erase(x);
            }
            output.flush();
        }

        /******** 10: Move ********/
        T.inc(5);
        if (T > limit)
            break;
        if (MoveTurn() == -1)
            break;

        /********* 20' : generate HP in cities ********/
        T.inc(10);
        if (T > limit)
            break;
        for (int i = 1; i < Size; ++i)
            city[i].generateHP();

        /********* 30' : gain HP from cities ********/
        T.inc(10);
        if (T > limit)
            break;
        {
            for (int i = 1; i < Size; ++i)
                if (city[i].size() == 1)
                {
                    Samurai *a = *city[i].begin();
                    int tmp = city[i].lostHP();
                    a->get_belong()->receiveHP(tmp);
                    if (tmp)
                        output.push(i, T.sPrint() + ' ' + a->getfullname() + " earned " +
                                           to_string(tmp) + " elements for his headquarter\n");
                }
            output.flush();
        }

        /********* 35' : Shot *********/
        T.inc(5);
        if (T > limit)
            break;
        {
            for (int i = 0; i <= Size; ++i)
            {
                Weapon *p;
                for (auto x : city[i])
                    if (p = x->canshot())
                    {
                        int aim = i + x->get_direct();
                        if (aim >= 0 && aim <= Size)
                        {
                            for (auto y : city[aim])
                                if (y->get_belong() != x->get_belong())
                                {
                                    string log = T.sPrint() + ' ' + x->getfullname() + " shot";
                                    p->attack(x, y);
                                    if (y->isdead())
                                        log += " and killed " + y->getfullname();
                                    output.push(i * 100 + x->get_belong()->get_outputlevel(), log + '\n');
                                    break;
                                }
                        }
                    }
            }
            output.flush();
        }

        /********* 38' : Bomb **********/
        T.inc(3);
        if (T > limit)
            break;
        {
            for (int i = 1; i < Size; ++i)
                if (city[i].size() == 2)
                {
                    Samurai *a = *city[i].begin(), *b = *city[i].rbegin();
                    if (a->isdead() || b->isdead())
                        continue;
                    Samurai *aa = a->copy(), *bb = b->copy();
                    Fight(aa, bb);
                    if (aa->isdead() && aa->canbomb())
                    {
                        output.push(i, T.sPrint() + ' ' + aa->getfullname() + " used a bomb and killed " + bb->getfullname() + '\n');
                        city[i].erase(a);
                        delete a;
                        city[i].erase(b);
                        delete b;
                    }
                    else if (bb->isdead() && bb->canbomb())
                    {
                        output.push(i, T.sPrint() + ' ' + bb->getfullname() + " used a bomb and killed " + aa->getfullname() + '\n');
                        city[i].erase(a);
                        delete a;
                        city[i].erase(b);
                        delete b;
                    }
                    delete aa;
                    delete bb;
                }
            output.flush();
        }

        /********* 40' : Fight *********/
        T.inc(2);
        if (T > limit)
            break;
        BattleTurn();

        /********* 50' : Headquarter Report **********/
        T.inc(10);
        if (T > limit)
            break;
        output.push(HeadA.get_outputlevel(), T.sPrint() + ' ' + to_string(HeadA.getHP()) + " elements in " + HeadA.getname() + " headquarter\n");
        output.push(HeadB.get_outputlevel(), T.sPrint() + ' ' + to_string(HeadB.getHP()) + " elements in " + HeadB.getname() + " headquarter\n");
        output.flush();

        /********* 55' : Samurai Report **********/
        T.inc(5);
        if (T > limit)
            break;
        for (int i = 0; i <= Size; ++i)
            for (auto x : city[i])
                output.push(x->get_outputlevel(), T.sPrint() + ' ' + x->report() + '\n');
        output.flush();

        T.inc(5);
        if (T > limit)
            break;
    }
}

BattleField::~BattleField()
{
    for (int i = 0; i < Size; ++i)
    {
        for (auto x : city[i])
            delete x;
    }
    delete[] city;
}