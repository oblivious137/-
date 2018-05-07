import re
import json
import os, sys
from functools import reduce

class GameState:
    def __init__(self, cards, puliccard, now=0, first=0, last=0):
        self._publiccard = publiccard
        self._cards = cards
        self._now = now
        self._first = first
        self._last = last
        self._used = [[]] * 3

    def play(self, card):
        for x in card:
            self._cards[self._now].pop(self._cards[self._now].index(x))
            self._used[self._now].append(x)
        if len(card) != 0:
            self._last = self._now
        self._now = (self._now + 1) % 3
        if self._now == self._last:
            self._first = self._now

    def state(self):
        ret = [0] * (54 * 5)
        for i in self._cards[self._now]:  # 自己的
            ret[i] = 1
        for i in range(0, 3):  # 历史记录
            for j in self._cards[i]:
                ret[(i + 1) * 54 + j] = 1
        for i in self._publiccard:  # 明牌
            ret[4 * 54 + i] = 1
        ret.append(1 if (self._first == 0) else 0)  # 过牌信息
        ret.append(1 if (self._first == 1) else 0)
        ret.append(1 if (self._first == 2) else 0)
        ret.append(1 if (self._last == 0) else 0)
        ret.append(1 if (self._last == 1) else 0)
        ret.append(1 if (self._last == 2) else 0)

def getoperation(_card, history):
    def getvalue(x):
        if x < 52:
            return x//4
        else:
            return x-52+13
    
    ans = list()
    lst = dict()
    try:
        if len(history[-1]) > 0:
            for i in history[-1]:
                if ! getvalue(i) in lst:
                    lst.update(getvalue(i), 1)
                else:
                    lst[getvalue(i)] += 1
        elif len(history[-2]) > 0:
            for i in history[-2]:
                if ! getvalue(i) in lst:
                    lst.update(getvalue(i), 1)
                else:
                    lst[getvalue(i)] += 1
    except Exception:
        # ignore it
    
    # 预处理一下
    card = [[]]*15
    for i in _card:
        card[getvalue(i)].append(i)
    # 单
    if len(lst) == 0 or len(lst) == 1 and list(lst.values)[0] = 1:
        for i in range(list(lst.keys)[0]+1, 15):
            if len(card[i]) > 0:
                ans.append(card[i][0:1])
    # 对
    if len(lst) == 0 or len(lst) == 1 and list(lst.values)[0] = 2:
        for i in range(list(lst.keys)[0]+1, 15):
            if len(card[i]) > 1:
                ans.append(card[i][0:2])
    # 单顺
    if len(lst) == 0 or (len(lst) > 4 and reduce(lambda x,y: x and y == 1, lst.values())):
        l = 0
        sl = len(lst)
        for i in range(12,min(lst.keys()),-1):
            if len(card[i]) > 0:
                l += 1
                if l>=sl:
                    ans.append([x[0] for x in card[i:i+sl]])
            else:
                l = 0
    # 双顺
    if len(lst) == 0 or (len(lst) > 2 and reduce(lambda x,y: x and y == 2, lst.values())):
        l = 0
        sl = len(lst)
        for i in range(12,min(lst.keys()),-1):
            if len(card[i]) > 1:
                l += 1
                if l>=sl:
                    ans.append([x[0] for x in card[i:i+sl]]+[x[1] for x in card[i:i+sl]])
            else:
                l = 0
    # 三带
    if len(lst) == 0 or (len(lst) > 2 and reduce(lambda x,y: x and y == 2, lst.values())):

data = [open("0.data", "w"), open("1.data", "w"), open("2.data", "w")]

def makedata(init, history, res):
    sta = GameState(init["allocation"], init["publiccard"])
    scor = len(history)
    print("xxxxxx")
    for i in history:
        sta.play(i)
        data[sta._now].write(" ".join(sta.state), 1.0/scor if res[sta._now] == 2 else -1.0/scor)

if __name__ == "__main__":
    pat = re.compile(".+\.matches$")
    for file_name in os.listdir(sys.argv[1]):
        print(file_name)
        if pat.match(file_name) == None:
            continue
        with open(os.path.join(sys.argv[1], file_name)) as f:
            for line in f:
                tmp = json.loads(line)
                init = json.loads(tmp["initdata"])
                # init : {
                #     allocation (list, size = 3, initial cards of eachone)
                #     publiccard (list, size = 3, public cards of loard)
                # }
                score = tmp["scores"]  # list, size = 3, the score of each one
                log = tmp["log"]
                if "errorInfo" in log[-1]["output"]["display"]:
                    # unfinished match because of TLE or other reasons.
                    continue
                try:
                    print(log[-1]["output"]["command"])
                    if log[-1]["output"]["command"] == "finish":
                        last = log[-3]["output"]["content"]
                        history = last.get("0", last.get(
                            "1", last.get("2")))["history"]
                        history.append(log[-2].get("0", log[-2].get(
                            "1", log[-2].get("2")))["response"])
                        makedata(init, history, log[-1]["output"]["content"])
                except Exception:
                    continue
        break
