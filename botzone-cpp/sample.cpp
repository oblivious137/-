#include <map>
#include <vector>
#include <cstdio>
#include <functional>
#include <iostream>
#include <set>
#include <ctime>
#include <string>
#include <cassert>
#include <cstring> // 注意memset是cstring里的
#include <algorithm>
#include "jsoncpp/json.h" // 在平台上，C++编译时默认包含此库
using namespace std;
using std::set;
using std::sort;
using std::string;
using std::unique;
using std::vector;

int getvalue(int x)
{
	if (x > 51)
		return x - 52 + 13;
	else
		return x / 4;
}

template <typename T>
vector<T> merge(const vector<T> &a, const vector<T> &b)
{
	vector<T> ret;
	for (auto &i : a)
		ret.push_back(i);
	for (auto &i : b)
		ret.push_back(i);
	return ret;
}

vector<vector<int>> getoperation(const set<int> &_card, const vector<int> &history)
{
	vector<vector<int>> ans;
	map<int, int> lst;
	if (history.size() > 0)
	{
		for (auto &i : history)
			lst[getvalue(i)] += 1;
	}
	if (lst.size() > 0)
		ans.push_back(vector<int>());
	// 预处理一下
	vector<vector<int>> card;
	for (int i = 0; i < 15; ++i)
		card.push_back(vector<int>());
	for (auto &i : _card)
		card[getvalue(i)].push_back(i);

	// 炸弹
	if (lst.size() == 1 && lst.begin()->second == 4)
	{
		for (int i = lst.begin()->first + 1; i < 13; ++i)
			if (card[i].size() >= 4)
			{
				ans.push_back(card[i]);
			}
	}
	else if (lst.find(13) == lst.end() || lst.find(14) == lst.end())
	{
		for (int i = 0; i < 13; ++i)
			if (card[i].size() >= 4)
			{
				ans.push_back(card[i]);
			}
	}

	// 火箭
	if (card[13].size() > 0 && card[14].size() > 0)
		ans.push_back(merge(card[13], card[14]));

	// 单
	{
		if (lst.size() == 1 && lst.begin()->second == 1)
		{
			for (int i = lst.begin()->first + 1; i < 15; ++i)
				if (card[i].size() >= 1)
				{
					vector<int> tmp;
					tmp.push_back(card[i][0]);
					ans.push_back(tmp);
				}
		}
		else if (lst.size() == 0)
		{
			for (int i = 0; i < 15; ++i)
				if (card[i].size() >= 1)
				{
					vector<int> tmp;
					tmp.push_back(card[i][0]);
					ans.push_back(tmp);
				}
		}
	}

	// 对
	{
		if (lst.size() == 1 && lst.begin()->second == 2)
		{
			for (int i = lst.begin()->first + 1; i < 15; ++i)
				if (card[i].size() >= 2)
				{
					vector<int> tmp;
					tmp.push_back(card[i][0]);
					tmp.push_back(card[i][1]);
					ans.push_back(tmp);
				}
		}
		else if (lst.size() == 0)
		{
			for (int i = 0; i < 15; ++i)
				if (card[i].size() >= 2)
				{
					vector<int> tmp;
					tmp.push_back(card[i][0]);
					tmp.push_back(card[i][1]);
					ans.push_back(tmp);
				}
		}
	}

	// 单顺
	{
		bool flag = true;
		int mn = 100, sl = 0;
		for (auto &i : lst)
		{
			if (i.second != 1)
			{
				flag = false;
				break;
			}
			mn = min(mn, i.first);
			++sl;
		}
		if (lst.size() > 4 && flag)
		{
			int l = 0;
			for (int i = 11; i > mn; --i)
			{
				if (card[i].size())
					++l;
				else
					l = 0;
				if (l >= sl)
				{
					vector<int> tmp;
					for (int j = 0; j < sl; ++j)
						tmp.push_back(card[i + j][0]);
					ans.push_back(tmp);
				}
			}
		}
		else if (lst.size() == 0)
		{
			int l = 0;
			for (int i = 11; i >= 0; --i)
			{
				if (card[i].size())
					++l;
				else
					l = 0;
				if (l >= 5)
				{
					vector<int> tmp;
					for (int j = 0; j < 4; ++j)
						tmp.push_back(card[i + j][0]);
					for (int j = 4; j < l; ++j)
					{
						tmp.push_back(card[i + j][0]);
						ans.push_back(tmp);
					}
				}
			}
		}
	}

	// 双顺
	{
		bool flag = true;
		int mn = 100, sl = 0;
		for (auto &i : lst)
		{
			if (i.second != 2)
			{
				flag = false;
				break;
			}
			mn = min(mn, i.first);
			++sl;
		}
		if (lst.size() > 2 && flag)
		{
			int l = 0;
			for (int i = 11; i > mn; --i)
			{
				if (card[i].size() > 1)
					++l;
				else
					l = 0;
				if (l >= sl)
				{
					vector<int> tmp;
					for (int j = 0; j < sl; ++j)
						tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]);
					ans.push_back(tmp);
				}
			}
		}
		else if (lst.size() == 0)
		{
			int l = 0;
			for (int i = 11; i >= 0; --i)
			{
				if (card[i].size() > 1)
					++l;
				else
					l = 0;
				if (l >= 3)
				{
					vector<int> tmp;
					for (int j = 0; j < 2; ++j)
						tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]);
					for (int j = 2; j < l; ++j)
					{
						tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]);
						ans.push_back(tmp);
					}
				}
			}
		}
	}

	// 三带 &&飞机
	{
		int sl = 0, sn = 0, mn = 100;
		bool flag = false;
		for (auto &i : lst)
		{
			if (i.second == 3)
			{
				flag = true;
				++sl;
				mn = min(mn, i.first);
			}
			else
				sn = i.second;
		}
		if (flag)
		{
			int l = 0;
			for (int i = 12; i > mn; --i)
			{
				if (card[i].size() >= 3)
					++l;
				else
					l = 0;
				if (l >= sl)
				{
					vector<int> tmp, canuse;
					for (int j = 0; j < sl; ++j)
						tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]), tmp.push_back(card[i + j][2]);
					for (int j = 0; j < i; ++j)
						if (card[j].size() >= sn)
							canuse.push_back(j);
					for (int j = i + sl; j < 15; ++j)
						if (card[j].size() >= sn)
							canuse.push_back(j);
					function<void(int)> func = [&](int pos) {
						if (tmp.size() == (sn + 3) * sl)
						{
							ans.push_back(tmp);
							return;
						}
						if ((canuse.size() - pos) * sn + tmp.size() < (sn + 3) * sl)
							return;
						func(pos + 1);
						for (int i = 0; i < sn; ++i)
							tmp.push_back(card[canuse[pos]][i]);
						func(pos + 1);
						for (int i = 0; i < sn; ++i)
							tmp.pop_back();
						return;
					};
					func(0);
				}
				if (i == 12)
					l = 0;
			}
		}
		else if (lst.size() == 0)
		{
			int l = 0;
			for (int i = 12; i >= 0; --i)
			{
				if (card[i].size() >= 3)
					++l;
				else
					l = 0;
				for (sl = 1; sl <= l; ++sl)
					for (sn = 0; sn < 3; ++sn)
					{
						vector<int> tmp, canuse;
						for (int j = 0; j < sl; ++j)
							tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]), tmp.push_back(card[i + j][2]);
						for (int j = 0; j < i; ++j)
							if (card[j].size() >= sn)
								canuse.push_back(j);
						for (int j = i + sl; j < 15; ++j)
							if (card[j].size() >= sn)
								canuse.push_back(j);
						function<void(int)> func = [&](int pos) {
							if (tmp.size() == (sn + 3) * sl)
							{
								ans.push_back(tmp);
								return;
							}
							if ((canuse.size() - pos) * sn + tmp.size() < (sn + 3) * sl)
								return;
							func(pos + 1);
							for (int i = 0; i < sn; ++i)
								tmp.push_back(card[canuse[pos]][i]);
							func(pos + 1);
							for (int i = 0; i < sn; ++i)
								tmp.pop_back();
							return;
						};
						func(0);
					}
				if (i == 12)
					l = 0;
			}
		}
	}

	// 四帯二 or 航天飞机
	{
		int sl = 0, sn = 0, mn = 100;
		bool flag = false;
		for (auto &i : lst)
		{
			if (i.second == 4)
			{
				flag = true;
				++sl;
				mn = min(mn, i.first);
			}
			else
				sn = i.second;
		}
		if (flag)
		{
			int l = 0;
			for (int i = 12; i > mn; --i)
			{
				if (card[i].size() >= 4)
					++l;
				else
					l = 0;
				if (l >= sl)
				{
					vector<int> tmp, canuse;
					for (int j = 0; j < sl; ++j)
						tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]), tmp.push_back(card[i + j][2]), tmp.push_back(card[i + j][3]);
					for (int j = 0; j < i; ++j)
						if (card[j].size() >= sn)
							canuse.push_back(j);
					for (int j = i + sl; j < 15; ++j)
						if (card[j].size() >= sn)
							canuse.push_back(j);
					function<void(int)> func = [&](int pos) {
						if (tmp.size() == 4 * sl + sn * sl * 2)
						{
							ans.push_back(tmp);
							return;
						}
						if ((canuse.size() - pos) * sn + tmp.size() < 4 * sl + sn * sl * 2)
							return;
						func(pos + 1);
						for (int i = 0; i < sn; ++i)
							tmp.push_back(card[canuse[pos]][i]);
						func(pos + 1);
						for (int i = 0; i < sn; ++i)
							tmp.pop_back();
						return;
					};
					func(0);
				}
				if (i == 12)
					l = 0;
			}
		}
		else if (lst.size() == 0)
		{
			int l = 0;
			for (int i = 12; i >= 0; --i)
			{
				if (card[i].size() >= 4)
					++l;
				else
					l = 0;
				for (sl = 1; sl <= l; ++sl)
					for (sn = ((sl == 1) ? 1 : 0); sn <= 2; ++sn)
					{
						vector<int> tmp, canuse;
						for (int j = 0; j < sl; ++j)
							tmp.push_back(card[i + j][0]), tmp.push_back(card[i + j][1]), tmp.push_back(card[i + j][2]), tmp.push_back(card[i + j][3]);
						for (int j = 0; j < i; ++j)
							if (card[j].size() >= sn)
								canuse.push_back(j);
						for (int j = i + sl; j < 15; ++j)
							if (card[j].size() >= sn)
								canuse.push_back(j);
						function<void(int)> func = [&](int pos) {
							if (tmp.size() == 4 * sl + sn * sl * 2)
							{
								ans.push_back(tmp);
								return;
							}
							if ((canuse.size() - pos) * sn + tmp.size() < 4 * sl + sn * sl * 2)
								return;
							func(pos + 1);
							for (int i = 0; i < sn; ++i)
								tmp.push_back(card[canuse[pos]][i]);
							func(pos + 1);
							for (int i = 0; i < sn; ++i)
								tmp.pop_back();
							return;
						};
						func(0);
					}
				if (i == 12)
					l = 0;
			}
		}
	}
	return ans;
}
constexpr int PLAYER_COUNT = 3;

enum class CardComboType
{
	PASS,		// 过
	SINGLE,		// 单张
	PAIR,		// 对子
	STRAIGHT,   // 顺子
	STRAIGHT2,  // 双顺
	TRIPLET,	// 三条
	TRIPLET1,   // 三带一
	TRIPLET2,   // 三带二
	BOMB,		// 炸弹
	QUADRUPLE2, // 四带二（只）
	QUADRUPLE4, // 四带二（对）
	PLANE,		// 飞机
	PLANE1,		// 飞机带小翼
	PLANE2,		// 飞机带大翼
	SSHUTTLE,   // 航天飞机
	SSHUTTLE2,  // 航天飞机带小翼
	SSHUTTLE4,  // 航天飞机带大翼
	ROCKET,		// 火箭
	INVALID		// 非法牌型
};

int cardComboScores[] = {
	0,  // 过
	1,  // 单张
	2,  // 对子
	6,  // 顺子
	6,  // 双顺
	4,  // 三条
	4,  // 三带一
	4,  // 三带二
	10, // 炸弹
	8,  // 四带二（只）
	8,  // 四带二（对）
	8,  // 飞机
	8,  // 飞机带小翼
	8,  // 飞机带大翼
	10, // 航天飞机（需要特判：二连为10分，多连为20分）
	10, // 航天飞机带小翼
	10, // 航天飞机带大翼
	16, // 火箭
	0   // 非法牌型
};

#ifndef _BOTZONE_ONLINE
string cardComboStrings[] = {
	"PASS",
	"SINGLE",
	"PAIR",
	"STRAIGHT",
	"STRAIGHT2",
	"TRIPLET",
	"TRIPLET1",
	"TRIPLET2",
	"BOMB",
	"QUADRUPLE2",
	"QUADRUPLE4",
	"PLANE",
	"PLANE1",
	"PLANE2",
	"SSHUTTLE",
	"SSHUTTLE2",
	"SSHUTTLE4",
	"ROCKET",
	"INVALID"};
#endif

// 用0~53这54个整数表示唯一的一张牌
using Card = int;
constexpr Card card_joker = 52;
constexpr Card card_JOKER = 53;

// 除了用0~53这54个整数表示唯一的牌，
// 这里还用另一种序号表示牌的大小（不管花色），以便比较，称作等级（Level）
// 对应关系如下：
// 3 4 5 6 7 8 9 10	J Q K	A	2	小王	大王
// 0 1 2 3 4 5 6 7	8 9 10	11	12	13	14
using Level = int;
constexpr Level MAX_LEVEL = 15;
constexpr Level MAX_STRAIGHT_LEVEL = 11;
constexpr Level level_joker = 13;
constexpr Level level_JOKER = 14;

/**
 * 将Card变成Level
 */
constexpr Level card2level(Card card)
{
	return card / 4 + card / 53;
}

// 牌的组合，用于计算牌型
struct CardCombo
{
	// 表示同等级的牌有多少张
	// 会按个数从大到小、等级从大到小排序
	struct CardPack
	{
		Level level;
		short count;

		bool operator<(const CardPack &b) const
		{
			if (count == b.count)
				return level > b.level;
			return count > b.count;
		}
	};
	vector<Card> cards;		 // 原始的牌，未排序
	vector<CardPack> packs;  // 按数目和大小排序的牌种
	CardComboType comboType; // 算出的牌型
	Level comboLevel = 0;	// 算出的大小序

	/**
	 * 检查个数最多的CardPack递减了几个
	 */
	int findMaxSeq() const
	{
		for (unsigned c = 1; c < packs.size(); c++)
			if (packs[c].count != packs[0].count ||
				packs[c].level != packs[c - 1].level - 1)
				return c;
		return packs.size();
	}

	/**
	* 这个牌型最后算总分的时候的权重
	*/
	int getWeight() const
	{
		if (comboType == CardComboType::SSHUTTLE ||
			comboType == CardComboType::SSHUTTLE2 ||
			comboType == CardComboType::SSHUTTLE4)
			return cardComboScores[(int)comboType] + (findMaxSeq() > 2) * 10;
		return cardComboScores[(int)comboType];
	}

	// 创建一个空牌组
	CardCombo() : comboType(CardComboType::PASS) {}

	/**
	 * 通过Card（即short）类型的迭代器创建一个牌型
	 * 并计算出牌型和大小序等
	 * 假设输入没有重复数字（即重复的Card）
	 */
	template <typename CARD_ITERATOR>
	CardCombo(CARD_ITERATOR begin, CARD_ITERATOR end)
	{
		// 特判：空
		if (begin == end)
		{
			comboType = CardComboType::PASS;
			return;
		}

		// 每种牌有多少个
		short counts[MAX_LEVEL + 1] = {};

		// 同种牌的张数（有多少个单张、对子、三条、四条）
		short countOfCount[5] = {};

		cards = vector<Card>(begin, end);
		for (Card c : cards)
			counts[card2level(c)]++;
		for (Level l = 0; l <= MAX_LEVEL; l++)
			if (counts[l])
			{
				packs.push_back(CardPack{l, counts[l]});
				countOfCount[counts[l]]++;
			}
		sort(packs.begin(), packs.end());

		// 用最多的那种牌总是可以比较大小的
		comboLevel = packs[0].level;

		// 计算牌型
		// 按照 同种牌的张数 有几种 进行分类
		vector<int> kindOfCountOfCount;
		for (int i = 0; i <= 4; i++)
			if (countOfCount[i])
				kindOfCountOfCount.push_back(i);
		sort(kindOfCountOfCount.begin(), kindOfCountOfCount.end());

		int curr, lesser;

		switch (kindOfCountOfCount.size())
		{
		case 1: // 只有一类牌
			curr = countOfCount[kindOfCountOfCount[0]];
			switch (kindOfCountOfCount[0])
			{
			case 1:
				// 只有若干单张
				if (curr == 1)
				{
					comboType = CardComboType::SINGLE;
					return;
				}
				if (curr == 2 && packs[1].level == level_joker)
				{
					comboType = CardComboType::ROCKET;
					return;
				}
				if (curr >= 5 && findMaxSeq() == curr &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::STRAIGHT;
					return;
				}
				break;
			case 2:
				// 只有若干对子
				if (curr == 1)
				{
					comboType = CardComboType::PAIR;
					return;
				}
				if (curr >= 3 && findMaxSeq() == curr &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::STRAIGHT2;
					return;
				}
				break;
			case 3:
				// 只有若干三条
				if (curr == 1)
				{
					comboType = CardComboType::TRIPLET;
					return;
				}
				if (findMaxSeq() == curr &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::PLANE;
					return;
				}
				break;
			case 4:
				// 只有若干四条
				if (curr == 1)
				{
					comboType = CardComboType::BOMB;
					return;
				}
				if (findMaxSeq() == curr &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::SSHUTTLE;
					return;
				}
			}
			break;
		case 2: // 有两类牌
			curr = countOfCount[kindOfCountOfCount[1]];
			lesser = countOfCount[kindOfCountOfCount[0]];
			if (kindOfCountOfCount[1] == 3)
			{
				// 三条带？
				if (kindOfCountOfCount[0] == 1)
				{
					// 三带一
					if (curr == 1 && lesser == 1)
					{
						comboType = CardComboType::TRIPLET1;
						return;
					}
					if (findMaxSeq() == curr && lesser == curr &&
						packs.begin()->level <= MAX_STRAIGHT_LEVEL)
					{
						comboType = CardComboType::PLANE1;
						return;
					}
				}
				if (kindOfCountOfCount[0] == 2)
				{
					// 三带二
					if (curr == 1 && lesser == 1)
					{
						comboType = CardComboType::TRIPLET2;
						return;
					}
					if (findMaxSeq() == curr && lesser == curr &&
						packs.begin()->level <= MAX_STRAIGHT_LEVEL)
					{
						comboType = CardComboType::PLANE2;
						return;
					}
				}
			}
			if (kindOfCountOfCount[1] == 4)
			{
				// 四条带？
				if (kindOfCountOfCount[0] == 1)
				{
					// 四条带两只 * n
					if (curr == 1 && lesser == 2)
					{
						comboType = CardComboType::QUADRUPLE2;
						return;
					}
					if (findMaxSeq() == curr && lesser == curr * 2 &&
						packs.begin()->level <= MAX_STRAIGHT_LEVEL)
					{
						comboType = CardComboType::SSHUTTLE2;
						return;
					}
				}
				if (kindOfCountOfCount[0] == 2)
				{
					// 四条带两对 * n
					if (curr == 1 && lesser == 2)
					{
						comboType = CardComboType::QUADRUPLE4;
						return;
					}
					if (findMaxSeq() == curr && lesser == curr * 2 &&
						packs.begin()->level <= MAX_STRAIGHT_LEVEL)
					{
						comboType = CardComboType::SSHUTTLE4;
						return;
					}
				}
			}
		}

		comboType = CardComboType::INVALID;
	}

	/**
	 * 判断指定牌组能否大过当前牌组（这个函数不考虑过牌的情况！）
	 */
	bool canBeBeatenBy(const CardCombo &b) const
	{
		if (comboType == CardComboType::INVALID || b.comboType == CardComboType::INVALID)
			return false;
		if (b.comboType == CardComboType::ROCKET)
			return true;
		if (b.comboType == CardComboType::BOMB)
			switch (comboType)
			{
			case CardComboType::ROCKET:
				return false;
			case CardComboType::BOMB:
				return b.comboLevel > comboLevel;
			default:
				return true;
			}
		return b.comboType == comboType && b.cards.size() == cards.size() && b.comboLevel > comboLevel;
	}

	/**
	 * 从指定手牌中寻找第一个能大过当前牌组的牌组
	 * 如果随便出的话只出第一张
	 * 如果不存在则返回一个PASS的牌组
	 */
	template <typename CARD_ITERATOR>
	CardCombo findFirstValid(CARD_ITERATOR begin, CARD_ITERATOR end) const
	{
		if (comboType == CardComboType::PASS) // 如果不需要大过谁，只需要随便出
		{
			CARD_ITERATOR second = begin;
			second++;
			return CardCombo(begin, second); // 那么就出第一张牌……
		}

		// 然后先看一下是不是火箭，是的话就过
		if (comboType == CardComboType::ROCKET)
			return CardCombo();

		// 现在打算从手牌中凑出同牌型的牌
		auto deck = vector<Card>(begin, end); // 手牌
		short counts[MAX_LEVEL + 1] = {};

		unsigned short kindCount = 0;

		// 先数一下手牌里每种牌有多少个
		for (Card c : deck)
			counts[card2level(c)]++;

		// 手牌如果不够用，直接不用凑了，看看能不能炸吧
		if (deck.size() < cards.size())
			goto failure;

		// 再数一下手牌里有多少种牌
		for (short c : counts)
			if (c)
				kindCount++;

		// 否则不断增大当前牌组的主牌，看看能不能找到匹配的牌组
		{
			// 开始增大主牌
			int mainPackCount = findMaxSeq();
			bool isSequential =
				comboType == CardComboType::STRAIGHT ||
				comboType == CardComboType::STRAIGHT2 ||
				comboType == CardComboType::PLANE ||
				comboType == CardComboType::PLANE1 ||
				comboType == CardComboType::PLANE2 ||
				comboType == CardComboType::SSHUTTLE ||
				comboType == CardComboType::SSHUTTLE2 ||
				comboType == CardComboType::SSHUTTLE4;
			for (Level i = 1;; i++) // 增大多少
			{
				for (int j = 0; j < mainPackCount; j++)
				{
					int level = packs[j].level + i;

					// 各种连续牌型的主牌不能到2，非连续牌型的主牌不能到小王，单张的主牌不能超过大王
					if ((comboType == CardComboType::SINGLE && level > MAX_LEVEL) ||
						(isSequential && level > MAX_STRAIGHT_LEVEL) ||
						(comboType != CardComboType::SINGLE && !isSequential && level >= level_joker))
						goto failure;

					// 如果手牌中这种牌不够，就不用继续增了
					if (counts[level] < packs[j].count)
						goto next;
				}

				{
					// 找到了合适的主牌，那么从牌呢？
					// 如果手牌的种类数不够，那从牌的种类数就不够，也不行
					if (kindCount < packs.size())
						continue;

					// 好终于可以了
					// 计算每种牌的要求数目吧
					short requiredCounts[MAX_LEVEL + 1] = {};
					for (int j = 0; j < mainPackCount; j++)
						requiredCounts[packs[j].level + i] = packs[j].count;
					for (unsigned j = mainPackCount; j < packs.size(); j++)
					{
						Level k;
						for (k = 0; k <= MAX_LEVEL; k++)
						{
							if (requiredCounts[k] || counts[k] < packs[j].count)
								continue;
							requiredCounts[k] = packs[j].count;
							break;
						}
						if (k == MAX_LEVEL + 1) // 如果是都不符合要求……就不行了
							goto next;
					}

					// 开始产生解
					vector<Card> solve;
					for (Card c : deck)
					{
						Level level = card2level(c);
						if (requiredCounts[level])
						{
							solve.push_back(c);
							requiredCounts[level]--;
						}
					}
					return CardCombo(solve.begin(), solve.end());
				}

			next:; // 再增大
			}
		}

	failure:
		// 实在找不到啊
		// 最后看一下能不能炸吧

		for (Level i = 0; i < level_joker; i++)
			if (counts[i] == 4 && (comboType != CardComboType::BOMB || i > packs[0].level)) // 如果对方是炸弹，能炸的过才行
			{
				// 还真可以啊……
				Card bomb[] = {Card(i * 4), Card(i * 4 + 1), Card(i * 4 + 2), Card(i * 4 + 3)};
				return CardCombo(bomb, bomb + 4);
			}

		// 有没有火箭？
		if (counts[level_joker] + counts[level_JOKER] == 2)
		{
			Card rocket[] = {card_joker, card_JOKER};
			return CardCombo(rocket, rocket + 2);
		}

		// ……
		return CardCombo();
	}

	void debugPrint()
	{
#ifndef _BOTZONE_ONLINE
		std::cout << "【" << cardComboStrings[(int)comboType] << "共" << cards.size() << "张，大小序" << comboLevel << "】";
#endif
	}
};

// 我的牌有哪些
set<Card> myCards;
vector<int> mcard[15];
set<Card>::iterator it;
// 地主被明示的牌有哪些
set<Card> landlordPublicCards;

// 大家从最开始到现在都出过什么
vector<vector<Card>> whatTheyPlayed[PLAYER_COUNT];

// 当前要出的牌需要大过谁
CardCombo lastValidCombo;

// 大家还剩多少牌
short cardRemaining[PLAYER_COUNT] = {20, 17, 17};
set<int> LeftCard;

// 我是几号玩家（0-地主，1-农民甲，2-农民乙）
int myPosition;

namespace BotzoneIO
{
using namespace std;
void input()
{
	// 读入输入（平台上的输入是单行）
	string line;
	getline(cin, line);
	Json::Value input;
	Json::Reader reader;
	reader.parse(line, input);

	// 首先处理第一回合，得知自己是谁、有哪些牌
	{
		auto firstRequest = input["requests"][0u]; // 下标需要是 unsigned，可以通过在数字后面加u来做到
		auto own = firstRequest["own"];
		auto llpublic = firstRequest["public"];
		auto history = firstRequest["history"];
		for (unsigned i = 0; i < own.size(); i++)
			myCards.insert(own[i].asInt());
		for (unsigned i = 0; i < llpublic.size(); i++)
			landlordPublicCards.insert(llpublic[i].asInt());
		if (history[0u].size() == 0)
			if (history[1].size() == 0)
				myPosition = 0; // 上上家和上家都没出牌，说明是地主
			else
				myPosition = 1; // 上上家没出牌，但是上家出牌了，说明是农民甲
		else
			myPosition = 2; // 上上家出牌了，说明是农民乙
	}

	// history里第一项（上上家）和第二项（上家）分别是谁的决策
	int whoInHistory[] = {(myPosition - 2 + PLAYER_COUNT) % PLAYER_COUNT, (myPosition - 1 + PLAYER_COUNT) % PLAYER_COUNT};

	int turn = input["requests"].size();
	for (int i = 0; i < turn; i++)
	{
		// 逐次恢复局面到当前
		auto history = input["requests"][i]["history"]; // 每个历史中有上家和上上家出的牌
		int howManyPass = 0;
		for (int p = 0; p < 2; p++)
		{
			int player = whoInHistory[p];   // 是谁出的牌
			auto playerAction = history[p]; // 出的哪些牌
			vector<Card> playedCards;
			for (unsigned _ = 0; _ < playerAction.size(); _++) // 循环枚举这个人出的所有牌
			{
				int card = playerAction[_].asInt(); // 这里是出的一张牌
				playedCards.push_back(card);
			}
			whatTheyPlayed[player].push_back(playedCards); // 记录这段历史
			cardRemaining[player] -= playerAction.size();

			if (playerAction.size() == 0)
				howManyPass++;
			else
				lastValidCombo = CardCombo(playedCards.begin(), playedCards.end());
		}

		if (howManyPass == 2)
			lastValidCombo = CardCombo();

		if (i < turn - 1)
		{
			// 还要恢复自己曾经出过的牌
			auto playerAction = input["responses"][i]; // 出的哪些牌
			vector<Card> playedCards;
			for (unsigned _ = 0; _ < playerAction.size(); _++) // 循环枚举自己出的所有牌
			{
				int card = playerAction[_].asInt(); // 这里是自己出的一张牌
				myCards.erase(card);				// 从自己手牌中删掉
				playedCards.push_back(card);
			}
			whatTheyPlayed[myPosition].push_back(playedCards); // 记录这段历史
			cardRemaining[myPosition] -= playerAction.size();
		}
	}
}

/**
		 * 输出决策，begin是迭代器起点，end是迭代器终点
		 * CARD_ITERATOR是Card（即short）类型的迭代器
		 */
template <typename CARD_ITERATOR>
void output(CARD_ITERATOR begin, CARD_ITERATOR end)
{
	Json::Value result, response(Json::arrayValue);
	for (; begin != end; begin++)
		response.append(*begin);
	result["response"] = response;

	Json::FastWriter writer;
	cout << writer.write(result) << endl;
}
} // namespace BotzoneIO

double getp(int cas, vector<int> cd)
{
	return (double)(getoperation(LeftCard, cd).size())/10;
	return (double)rand();// / RAND_MAX;
}

double getp(vector<int> cd)
{
	return (double)(getoperation(LeftCard, cd).size())/10;
	return (double)rand();// / RAND_MAX;
}

tuple<double, vector<vector<int>>> _split(int cas = 0, int st = 0)
{
	int sum = 0;
	for (int i=0;i<15;++i) sum+=mcard[i].size();
	// printf("search %d card, %d, %d\n",sum,cas,st);
	double mn = 1e100;
	vector<vector<int>> ans;
	switch (cas)
	{
	case 0:
		// 双顺
		{
			int l = 0;
			for (int i = st; i < 12; ++i)
			{
				if (l + 12 - i < 3)
					break;
				if (mcard[i].size() > 1)
					++l;
				else
					l = 0;
				if (l >= 3)
				{
					vector<int> rec;
					for (int j = 0; j < 2; ++j)
					{
						rec.push_back(mcard[i - j][mcard[i - j].size() - 1]), mcard[i - j].pop_back();
						rec.push_back(mcard[i - j][mcard[i - j].size() - 1]), mcard[i - j].pop_back();
					}
					for (int j = 2; j < l; ++j)
					{
						rec.push_back(mcard[i - j][mcard[i - j].size() - 1]), mcard[i - j].pop_back();
						rec.push_back(mcard[i - j][mcard[i - j].size() - 1]), mcard[i - j].pop_back();
						double h;
						vector<vector<int>> tmp;
						tie(h, tmp) = _split(cas, i - j);
						h += 1 + getp(0, rec);
						if (h < mn)
						{
							ans = tmp;
							ans.push_back(rec);
						}
					}
					for (auto &x : rec)
						mcard[getvalue(x)].push_back(x);
				}
			}
			st = 0;
		}
	case 1:
		// 顺
		{
			int l = 0;
			for (int i = st; i < 12; ++i)
			{
				if (l + 12 - i < 5)
					break;
				if (mcard[i].size() > 0)
					++l;
				else
					l = 0;
				if (l >= 5)
				{
					vector<int> rec;
					for (int j = 0; j < 4; ++j)
						rec.push_back(mcard[i - j][mcard[i - j].size() - 1]), mcard[i - j].pop_back();
					for (int j = 4; j < l; ++j)
					{
						rec.push_back(mcard[i - j][mcard[i - j].size() - 1]), mcard[i - j].pop_back();
						double h;
						vector<vector<int>> tmp;
						tie(h, tmp) = _split(cas, i - j);
						h += 1 + getp(1, rec);
						if (h < mn)
						{
							ans = tmp;
							ans.push_back(rec);
						}
					}
					for (auto &x : rec)
						mcard[getvalue(x)].push_back(x);
				}
			}
			st = 0;
		}
	case 2:
		// 三带或飞机
		{
			int l = 0;
			for (int i = st; i < 13; ++i)
			{
				if (i == 12)
					l = 0;
				if (mcard[i].size() >= 3)
					++l;
				else
					l = 0;
				for (int sl = 1; sl <= l; ++sl)
					for (int sn = 0; sn < 3; ++sn)
					{
						vector<int> tmp, canuse;
						for (int j = 0; j < sl; ++j)
						{
							tmp.push_back(mcard[i - j][mcard[i - j].size() - 1]);
							tmp.push_back(mcard[i - j][mcard[i - j].size() - 2]);
							tmp.push_back(mcard[i - j][mcard[i - j].size() - 3]);
						}
						for (int j = 0; j <= i - sl; ++j)
							if (mcard[j].size() >= sn)
								canuse.push_back(j);
						for (int j = i + 1; j < 15; ++j)
							if (mcard[j].size() >= sn)
								canuse.push_back(j);
						function<void(int)> func = [&](int pos) {
							if (tmp.size() == (sn + 3) * sl)
							{
								for (auto &x : tmp)
									mcard[getvalue(x)].pop_back();
								double h;
								vector<vector<int>> uu;
								tie(h, uu) = _split(2, i - sl + 1);
								h += 1 + getp(2, tmp);
								if (h < mn)
								{
									mn = h;
									ans = uu;
									ans.push_back(tmp);
								}
								for (auto &x : tmp)
									mcard[getvalue(x)].push_back(x);
								return;
							}
							if ((canuse.size() - pos) * sn + tmp.size() < (sn + 3) * sl)
								return;
							func(pos + 1);
							for (int i = 0; i < sn; ++i)
								tmp.push_back(mcard[canuse[pos]][mcard[canuse[pos]].size() - i - 1]);
							func(pos + 1);
							for (int i = 0; i < sn; ++i)
								tmp.pop_back();
							return;
						};
						func(0);
					}
			}
			st = 0;
		}
	case 3:
		// 四带或航天飞机
		{
			int l = 0;
			for (int i = st; i < 13; ++i)
			{
				if (i == 12)
					l = 0;
				if (mcard[i].size() >= 4)
					++l;
				else
					l = 0;
				for (int sl = 1; sl <= l; ++sl)
					for (int sn = ((sl != 1) ? 0 : 1); sn < 3; ++sn)
					{
						vector<int> tmp, canuse;
						for (int j = 0; j < sl; ++j)
						{
							tmp.push_back(mcard[i - j][mcard[i - j].size() - 1]);
							tmp.push_back(mcard[i - j][mcard[i - j].size() - 2]);
							tmp.push_back(mcard[i - j][mcard[i - j].size() - 3]);
							tmp.push_back(mcard[i - j][mcard[i - j].size() - 4]);
						}
						for (int j = 0; j <= i - sl; ++j)
							if (mcard[j].size() >= sn)
								canuse.push_back(j);
						for (int j = i + 1; j < 15; ++j)
							if (mcard[j].size() >= sn)
								canuse.push_back(j);
						function<void(int)> func = [&](int pos) {
							if (tmp.size() == sn * sl * 2 + sl * 4)
							{
								for (auto &x : tmp)
									mcard[getvalue(x)].pop_back();
								double h;
								vector<vector<int>> uu;
								tie(h, uu) = _split(3, i - sl + 1);
								h += 1 + getp(3, tmp);
								if (h < mn)
								{
									mn = h;
									ans = uu;
									ans.push_back(tmp);
								}
								for (auto &x : tmp)
									mcard[getvalue(x)].push_back(x);
								return;
							}
							if ((canuse.size() - pos) * sn + tmp.size() < sn * sl * 2 + sl * 4)
								return;
							func(pos + 1);
							for (int i = 0; i < sn; ++i)
								tmp.push_back(mcard[canuse[pos]][mcard[canuse[pos]].size() - i - 1]);
							func(pos + 1);
							for (int i = 0; i < sn; ++i)
								tmp.pop_back();
							return;
						};
						func(0);
					}
			}
			st = 0;
		}
		// printf("sum = %d\n",sum);
	case 4:
		// 炸弹
		{
			for (int i = st; i < 13; ++i)
				if (mcard[i].size() >= 4)
				{
					vector<int> uu = mcard[i];
					mcard[i].clear();
					double h;
					vector<vector<int>> tmp;
					tie(h, tmp) = _split(4, i);
					h += 1 + getp(4, uu);
					if (h < mn)
					{
						mn = h;
						ans = tmp;
						ans.push_back(uu);
					}
					for (auto &x : uu)
						mcard[i].push_back(x);
				}
			st = 0;
		}
	case 5:
		// 火箭
		{
			if (mcard[13].size() > 0 && mcard[14].size() > 0)
			{
				vector<int> uu;
				uu.push_back(mcard[13][0]);
				uu.push_back(mcard[14][0]);
				mcard[13].pop_back();
				mcard[14].pop_back();
				double h;
				vector<vector<int>> tmp;
				tie(h, tmp) = _split(5, 0);
				h += 1 + getp(5, uu);
				if (h < mn)
				{
					mn = h;
					ans = tmp;
					ans.push_back(uu);
				}
				for (auto &x : uu)
					mcard[getvalue(x)].push_back(x);
			}
		}
	case 6:
	case 7:
		// 单 and 对
		{
			double h = 0;
			vector<vector<int>> tmp;
			for (int i = 0; i < 15; ++i)
			{
				for (int cas = 0; cas < (mcard[i].size() + 1) / 2; ++cas)
				{
					vector<int> uu;
					for (int j = cas * 2; j < (cas + 1) * 2 && j < mcard[i].size(); ++j)
						uu.push_back(mcard[i][j]);
					h += 1 + getp((uu.size() == 1) ? 6 : 7, uu);
					tmp.push_back(uu);
				}
			}
			if (h < mn)
			{
				mn = h;
				ans = tmp;
			}
		}
	}
	// printf("return %d card, %d, %d\n",sum,cas,st);
	return make_tuple(mn, ans);
}

tuple<double,vector<vector<int>>> split()
{
	for (int i=0;i<15;++i) mcard[i].clear();
	for (auto&x:myCards)
		mcard[getvalue(x)].push_back(x);
	// puts("xxxxxxx");
	auto ans = _split();
	return ans;
}

inline vector<int> getans(vector<int> lastcard)
{
	auto action_list = getoperation(myCards, lastcard);
	for (auto &i : action_list)
	{
		if (i.size() == myCards.size())
			return i;
	}
	if (lastcard.empty())
	{
		auto sp = get<1>(split());
		function<double(vector<int>)> f;
		if (myPosition == 0)
		{
			if (cardRemaining[1] < 4 || cardRemaining[2] < 4)
				f = [](vector<int> x) { return getp(x); };
			else
				f = [](vector<int> x) { return -getp(x); };
		}
		else
		{
			if (cardRemaining[0] < 4)
				f = [](vector<int> x) { return getp(x); };
			else
				f = [](vector<int> x) { return -getp(x); };
		}
		double mn = 1e100;
		vector<int> ans;
		for (auto &x : sp)
		{
			double h = f(x);
			if (h < mn)
			{
				mn = h;
				ans = x;
			}
		}
		return ans;
	}
	double mn = 1e100;
	vector<int> ans;
	if (myPosition == 0)
		{
			if (cardRemaining[1] < 4 || cardRemaining[2] < 4){
				for (auto& i:action_list){
					double h = getp(i);
					if (h<mn) {
						mn=h;
						ans=i;
					}
				}
			}
			else{
				for (auto &i : action_list)
				{
					for (auto &x : i)
						myCards.erase(x);
					double tmp = get<0>(split());
					if (tmp < mn)
					{
						mn = tmp;
						ans = i;
					}
					for (auto &x : i)
						myCards.insert(x);
				}
			}
		}
		else
		{
			if (cardRemaining[0] < 4){
				for (auto& i:action_list){
					double h = getp(i);
					if (h<mn) {
						mn=h;
						ans=i;
					}
				}
			}
			else{
				for (auto &i : action_list)
				{
					// cout<<"op:";
					for (auto &x : i){
						myCards.erase(x);
						// cout<<x<<' ';
					}
					double tmp = get<0>(split());
					// cout<<tmp<<endl;
					if (tmp < mn)
					{
						mn = tmp;
						ans = i;
					}
					for (auto &x : i)
						myCards.insert(x);
				}
			}
		}
	
	return ans;
}
int main()
{
	srand(time(0));
	for (int i = 0; i < 54; ++i)
		LeftCard.insert(i);
	BotzoneIO::input();
	for (auto &x : myCards)
		LeftCard.erase(x);
	for (auto &x : landlordPublicCards)
		LeftCard.erase(x);
	for (int i = 0; i < 3; ++i)
		for (auto &x : whatTheyPlayed[i])
			for (auto &y : x)
				LeftCard.erase(y);
	auto action = getans(lastValidCombo.cards);
	BotzoneIO::output(action.begin(), action.end());
	return 0;
}
