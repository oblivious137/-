#include <map>
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

int TIME;

class Samurai
{
    string kind;
    int HealthPoint;

  public:
    Samurai(const string &_kind = "", int _HP = 0) : kind(_kind), HealthPoint(_HP){};
    string getkind() const
    {
        return kind;
    }
    int getHP() const
    {
        return HealthPoint;
    }
};

class Headquarter
{
  private:
    string name;
    static const vector<Samurai> NULLOrder;
    vector<Samurai> Order;
    vector<int> ExistNumber;
    int HealthPoint, OnWitch = 0, Count = 0;
    bool stopped = false;

    bool CanConstruct(int x)
    {
        return HealthPoint >= Order[x].getHP();
    }

    void Construct(int x)
    {
        HealthPoint -= Order[x].getHP();
        ++Count;
        ++ExistNumber[x];
        printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",
               TIME, name.c_str(), Order[x].getkind().c_str(), Count, Order[x].getHP(),
               ExistNumber[x], Order[x].getkind().c_str(), name.c_str());
        return;
    }

  public:
    Headquarter(const string &_name = "", int _HP = 0,
                const vector<Samurai> &_Order = NULLOrder) : name(_name), HealthPoint(_HP),
                                                             Order(_Order), ExistNumber(_Order.size(), 0){};
    void Stop()
    {
        if (stopped)
            return;
        printf("%03d %s headquarter stops making warriors\n", TIME, name.c_str());
        stopped = true;
        return;
    }
    bool isstopped()
    {
        return stopped;
    }
    void next_turn()
    {
        if (stopped)
            return;
        for (int i = 0; i < Order.size(); ++i)
        {
            if (OnWitch >= Order.size())
                OnWitch = 0;
            if (CanConstruct(OnWitch))
            {
                Construct(OnWitch);
                ++OnWitch;
                return;
            }
            ++OnWitch;
        }
        Stop();
        return;
    }
};

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
        Samurai dragon("dragon", hp[0]);
        Samurai ninja("ninja", hp[1]);
        Samurai iceman("iceman", hp[2]);
        Samurai lion("lion", hp[3]);
        Samurai wolf("wolf", hp[4]);
        vector<Samurai> A({iceman, lion, wolf, ninja, dragon});
        vector<Samurai> B({lion, dragon, ninja, iceman, wolf});
        Headquarter red("red", tmp, A), blue("blue", tmp, B);
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