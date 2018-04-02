#include "headquarter_define.h"

/********* Headquarter ***********/

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

/******** _OUTPUT_CMP **********/
bool _OUTPUT_CMP::operator()(Samurai *const a, Samurai *const b)
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

void BattleField::Run()
{
    for (;;)
    {
        OrderedOutput output;
        string log;
        Samurai *nS;
        int outCount = 0;
        tie(nS, log) = HeadA.Build_SA();
        if (nS && log.length())
            output.push(outCount++, T.sPrint() + ' ' + log + '\n');
        if (nS)
            city[HeadA.get_pos()].insert(nS);
        tie(nS, log) = HeadB.Build_SA();
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
                        output.push(outCount + aim * 100 + x->get_belong()->get_outputlevel(), tp.str());
                    }
                }
            }
        }
        for (int i = Size; i >= 0; --i)
        {
            for (auto x : (city[i]))
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
                        output.push(outCount + aim * 100 + x->get_belong()->get_outputlevel(), tp.str());
                    }
                }
            }
        }
    }
}