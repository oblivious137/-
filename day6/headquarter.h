#pragma once
#include "headquarter_declaration.h"
#include "samurai_declaration.h"
#include "weapon_declaration.h"

/********* Headquarter ***********/

Headquarter::Headquarter(const string &_name, int _HP,
                         const vector<Samurai *> &_Order,
                         const vector<function<Weapon *()>> &_Weapons) : name(_name), HealthPoint(_HP), Order(_Order), Weapons(_Weapons), ExistNumber(0, _Order.size()){};

unsigned int Headquarter::getOrderSize() { return Order.size(); }
Weapon *Headquarter::getweapon(int x) const { return Weapons[x](); }

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
        return {NULL, ""};
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
            ++OnWitch;
            tmp->setpos(posi);
            tmp->setdirect(dire);
            return {tmp, tmp->getfullname() + " born"};
        }
        ++OnWitch;
    }
    Stop();
    return {NULL, ""};
}

/******** _OUTPUT_CMP **********/
bool _OUTPUT_CMP::operator()(const Samurai *const a, const Samurai *const b) const
{
    if (a->get_belong()->get_outputlevel() == b->get_belong()->get_outputlevel())
        return a->get_belong()->get_outputlevel() < b->get_belong()->get_outputlevel();
    else
        return a < b;
}

/******* BattleField *******/

BattleField::BattleField(int n, Headquarter a, Headquarter b) : Size(n + 1), HeadA(a), HeadB(b)
{
    HeadA.set_pos(0);
    HeadB.set_pos(n + 1);
    HeadA.set_direct(1);
    HeadB.set_direct(-1);
    HeadA.set_outputlevel(1);
    HeadB.set_outputlevel(2);
    city = new SamuraiSet[n + 2];
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
        city[HeadA.get_pos()].insert(nS);
    tie(nS, log) = HeadB.Build_SA();
    if (nS && log.length())
        output.push(HeadB.get_outputlevel(), T.sPrint() + ' ' + log + '\n');
    if (nS)
        city[HeadB.get_pos()].insert(nS);
}

int BattleField::MoveTurn()
{
    OrderedOutput output;
    for (int i = 0; i <= Size; ++i)
    {
        for (auto x : city[i])
        {
            if (x->get_direct() < 0)
            {
                int aim = i + x->get_direct();
                if (aim >= 0 && aim <= Size)
                {
                    city[i].erase(x);
                    city[aim].insert(x);
                    x->moveeffect();
                    x->setpos(aim);
                }
            }
        }
    }
    for (int i = Size; i >= 0; --i)
    {
        for (auto x : city[i])
        {
            if (x->get_direct() > 0)
            {
                int aim = i + x->get_direct();
                if (aim >= 0 && aim <= Size)
                {
                    city[i].erase(x);
                    city[aim].insert(x);
                    x->moveeffect();
                    x->setpos(aim);
                }
            }
        }
    }
    for (int i = 1; i <= Size; ++i)
    {
        for (auto x : city[i])
        {
            stringstream tp;
            tp << T.sPrint() << ' ' << x->getfullname() << " marched to city " << i << " with "
               << x->getHP() << " elements and force " << x->getAtk() << '\n';
            output.push(x->get_outputlevel(), tp.str());
        }
    }
    for (auto x : city[0])
    {
        stringstream tp;
        tp << T.sPrint() << ' ' << x->getfullname() << " reached " << HeadA.getname() << " headquarter with "
           << x->getHP() << " elements and force " << x->getAtk() << '\n';
        output.push(x->get_outputlevel(), tp.str());
    }
    for (auto x : city[Size])
    {
        stringstream tp;
        tp << T.sPrint() << ' ' << x->getfullname() << " reached " << HeadB.getname() << " headquarter with "
           << x->getHP() << " elements and force " << x->getAtk() << '\n';
        output.push(x->get_outputlevel(), tp.str());
    }
    if (city[0].size())
        output.push(50, T.sPrint() + ' ' + HeadA.getname() + " headquarter was taken\n");
    if (city[Size].size())
        output.push(100 * Size + 50, T.sPrint() + ' ' + HeadB.getname() + " headquarter was taken\n");
    if (city[0].size() || city[Size].size())
    {
        output.flush();
        return -1;
    }
}

void BattleField::BattleTurn()
{
    OrderedOutput output;
    for (int i = 0; i <= Size; ++i)
        if (city[i].size() == 2)
        {
            Samurai *a = *city[i].begin();
            Samurai *b = *city[i].rbegin();
            (a->getbag()).preliminary();
            (b->getbag()).preliminary();
            if (!BattleFirst(a, b))
                swap(a, b);

            //Process
            int flg = max(a->getbag().size(), b->getbag().size()) * 2 + 3;
            while ((a->getbag().size() || b->getbag().size()) && flg)
            {
                --flg;
                Weapon *x = a->getbag().renext();
                if (x == NULL)
                    continue;
                if (x->attack(a, b)!=0)
                    flg = max(a->getbag().size(), b->getbag().size()) * 2 + 3;
                if (a->isdead() || b->isdead())
                    break;
                swap(a, b);
            }
            //Confirm result
            if (a->isdead() ^ b->isdead())
            {
                if (a->isdead())
                    swap(a, b);

                output.push(i * 100, T.sPrint() + ' ' + a->getfullname() + " killed " + b->getfullname() + " in city " +
                                         to_string(i) + " remaining " + to_string(a->getHP()) + "elements\n");
                //Rob weapons
                WeaponBag& bag = b->getbag();
                bag.preliminary([](Weapon * a, Weapon * b){return !Weapon::CMP(a,b);});
                while(bag.size()){
                    Weapon* x = bag.next();
                    if (x==NULL) break;
                    if (a->getbag().push(x)) bag.pop_top();
                    else break;
                }
            }
            else
            {
                if (a->get_outputlevel() > b->get_outputlevel())
                    swap(a, b);
                if (a->isdead())
                {
                    output.push(i * 100, T.sPrint() + " both " + a->getfullname() + " and " + b->getfullname() +
                                             " died in city " + to_string(i) + "\n");
                }
                else
                {
                    output.push(i * 100, T.sPrint() + " both " + a->getfullname() + " and " + b->getfullname() +
                                             " were alive in city " + to_string(i) + "\n");
                }
            }

            //Delete dead samurais and yelled
            if (a->isdead())
            {
                city[a->getpos()].erase(a);
                delete a;
            }
            else
            {
                string log = a->yelled();
                if (log != "")
                    output.push(a->get_outputlevel(), T.sPrint() + ' ' + log);
            }
            if (b->isdead())
            {
                city[b->getpos()].erase(b);
                delete b;
            }
            else
            {
                string log = b->yelled();
                if (log != "")
                    output.push(b->get_outputlevel(), T.sPrint() + ' ' + log);
            }
        }
}

void BattleField::Run()
{
    for (;;)
    {
        OrderedOutput output;

        BuildTurn();

        T.inc(5);
        /********* Escape ********/
        {
            for (int i = 0; i <= Size; ++i)
            {
                string tmp;
                for (auto x : city[i])
                {
                    if ((tmp = x->escape()) != "")
                    {
                        city[i].erase(x);
                        output.push(x->get_outputlevel(), T.sPrint() + ' ' + tmp + "\n");
                        delete x;
                    }
                }
            }
        }

        T.inc(5);
        if (MoveTurn() == -1)
            break;

        T.inc(25);
        /********* Wolf robs weapons *********/
        {
            for (int i = 0; i <= Size; ++i)
            {
                if (city[i].size() == 2)
                {
                    Samurai *a = *city[i].begin();
                    Samurai *b = *city[i].rbegin();
                    string log = a->rob(b);
                    if (log != "")
                        output.push(i, T.sPrint() + ' ' + log + '\n');
                    log = b->rob(a);
                    if (log != "")
                        output.push(i, T.sPrint() + ' ' + log + '\n');
                }
            }
            output.flush();
        }

        T.inc(5);
        /********* Battle Begin *********/
        BattleTurn();
    }
}