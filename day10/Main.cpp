#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include "headquarter.h"
#include "samurai.h"
#include "weapon.h"
using namespace std;

int main()
{
	int T = 0;
	scanf("%d", &T);
	for (int cas = 1; cas <= T; ++cas)
	{
		printf("Case %d:\n", cas);
		int M, N, K, D;
		scanf("%d%d%d%d", &M, &N, &K, &D);
		int hp[5], atk[5];
		for (int i = 0; i < 5; ++i)
			scanf("%d", &hp[i]);
		for (int i = 0; i < 5; ++i)
			scanf("%d", &atk[i]);

		vector<function<Samurai *(Headquarter*)>> AS({bind(Iceman::generate,placeholders::_1,hp[2],atk[2]), bind(Lion::generate, placeholders::_1, hp[3], atk[3], K),
			bind(Wolf::generate,placeholders::_1, hp[4], atk[4]), bind(Ninja::generate, placeholders::_1, hp[1], atk[1]),
			bind(Dragon::generate, placeholders::_1, hp[0], atk[0])});
		vector<function<Samurai *(Headquarter*)>> BS({ bind(Lion::generate, placeholders::_1, hp[3], atk[3], K), bind(Dragon::generate, placeholders::_1, hp[0], atk[0]),
			bind(Ninja::generate, placeholders::_1, hp[1], atk[1]), bind(Iceman::generate,placeholders::_1,hp[2],atk[2]),
			bind(Wolf::generate,placeholders::_1, hp[4], atk[4]) });
		vector<function<Weapon *()>> W;
		W.push_back([]() -> Weapon * { return (Weapon *)new Sword("sword", 0.2); });
		W.push_back([]() -> Weapon * { return (Weapon *)new Bomb("bomb", 0.4, 1); });
		W.push_back([]() -> Weapon * { return (Weapon *)new Arrow("arrow", 0.3, 2); });

		Headquarter red("red", M, AS, W), blue("blue", M, BS, W);

		BattleField B(N, red, blue);
		B.Run(D);
	}
	return 0;
}