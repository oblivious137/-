import re
import json
import os, sys
from functools import reduce
import copy

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
        return ret

data = [open("0.data", "w"), open("1.data", "w"), open("2.data", "w")]

def makedata(init, history, res):
    sta = GameState(init["allocation"], init["publiccard"])
    scor = len(history)
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
