#define _CRT_SECURE_NO_WARNINGS
#include "headquarter.h"
#include "samurai.h"
#include "weapon.h"
using namespace std;

int main()
{
	freopen("in.txt", "r", stdin);
	freopen("tmp.out", "w", stdout);
	int T = 0;
	scanf("%d", &T);
	for (int cas = 1; cas <= T; ++cas)
	{
		printf("Case %d:\n", cas);
		int M, N, R, K, D;
		scanf("%d%d%d%d%d", &M, &N, &R, &K, &D);
		int hp[5], atk[5];
		for (int i = 0; i < 5; ++i)
			scanf("%d", &hp[i]);
		for (int i = 0; i < 5; ++i)
			scanf("%d", &atk[i]);
		vector<function<Samurai *(Headquarter *)>> AS;
		AS.push_back(bind(Iceman::generate, placeholders::_1, hp[2], atk[2]));
		AS.push_back(bind(Lion::generate, placeholders::_1, hp[3], atk[3], K));
		AS.push_back(bind(Wolf::generate, placeholders::_1, hp[4], atk[4]));
		AS.push_back(bind(Ninja::generate, placeholders::_1, hp[1], atk[1]));
		AS.push_back(bind(Dragon::generate, placeholders::_1, hp[0], atk[0]));

		vector<function<Samurai *(Headquarter *)>> BS;
		BS.push_back(bind(Lion::generate, placeholders::_1, hp[3], atk[3], K));
		BS.push_back(bind(Dragon::generate, placeholders::_1, hp[0], atk[0]));
		BS.push_back(bind(Ninja::generate, placeholders::_1, hp[1], atk[1]));
		BS.push_back(bind(Iceman::generate, placeholders::_1, hp[2], atk[2]));
		BS.push_back(bind(Wolf::generate, placeholders::_1, hp[4], atk[4]));

		vector<function<Weapon *(Samurai *)>> W;
		W.push_back(bind(Sword::generate, placeholders::_1, 0.2, 0.8));
		W.push_back(bind(Bomb::generate, placeholders::_1, 1));
		W.push_back(bind(Arrow::generate, placeholders::_1, R, 3));

		Headquarter red("red", M, AS, W), blue("blue", M, BS, W);

		BattleField B(N, red, blue);
		B.Run(D);
	}
	//system("pause");
	return 0;
}