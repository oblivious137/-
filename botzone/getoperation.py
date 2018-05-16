from functools import reduce
import copy

def getoperation(_card, history):
    def getvalue(x):
        if x < 52:
            return x//4
        else:
            return x-52+13
    ans = list()
    lst = dict()
    try:
        if len(history) > 0 and len(history[-1]) > 0:
            for i in history[-1]:
                if not getvalue(i) in lst:
                    lst[getvalue(i)] = 1
                else:
                    lst[getvalue(i)] += 1
        elif len(history) > 1 and len(history[-2]) > 0:
            for i in history[-2]:
                if not getvalue(i) in lst:
                    lst[getvalue(i)] = 1
                else:
                    lst[getvalue(i)] += 1
    except Exception:
        return []
    # 预处理一下
    card = []
    for i in range(0,15):
        card.append([])
    for i in _card:
        card[getvalue(i)].append(i)
    # 单
    def get1(mn):
        for i in range(mn+1, 15):
            if len(card[i]) > 0:
                ans.append(card[i][0:1])
    if len(lst) == 1 and list(lst.values())[0] == 1:
        get1(list(lst.keys())[0])
    elif len(lst) == 0:
        get1(-1)
    
    # 对
    def get2(mn):
        for i in range(mn+1, 15):
            if len(card[i]) > 1:
                ans.append(card[i][0:2])
    if len(lst) == 1 and list(lst.values())[0] == 2:
        get2(list(lst.keys())[0])
    elif len(lst) == 0:
        get2(-1)
        
    # 单顺
    def get3(mn,sl):
        l = 0
        for i in range(11,mn,-1):
            if len(card[i]) > 0:
                l += 1
                if l>=sl:
                    ans.append([x[0] for x in card[i:i+sl]])
            else:
                l = 0
    if len(lst) > 4 and reduce(lambda x,y: x and y == 1, lst.values()):
        sl = len(lst)
        get3(min(lst.keys()),sl)
    elif len(lst) == 0:
        for sl in range(5,13):
            get3(-1,sl)
    
    # 双顺
    def get4(mn,sl):
        l = 0
        for i in range(11,mn,-1):
            if len(card[i]) > 1:
                l += 1
                if l>=sl:
                    ans.append([x[0] for x in card[i:i+sl]]+[x[1] for x in card[i:i+sl]])
            else:
                l = 0
    if len(lst) > 2 and reduce(lambda x,y: x and y == 2, lst.values()):
        sl = len(lst)
        get4(min(lst.keys()),sl)
    elif len(lst) == 0:
        for sl in range(3,13):
            get4(-1,sl)
    
    # 三带 && 飞机
    def get5(sl,sn,mn):
        l = 0
        for i in range(12,mn,-1):
            if len(card[i]) > 2:
                l += 1
                if l>=sl:
                    ret = [x[0] for x in card[i:i+sl]]+[x[1] for x in card[i:i+sl]]+[x[2] for x in card[i:i+sl]]
                    rst = card[0:i]+card[i+sl:]
                    def getcardwitht(pos, now):
                        if len(now)==sl*sn:
                            ans.append(ret+now)
                            return
                        # print(rst,pos,now)
                        if (len(rst) - pos)*sn + len(now) < sl*sn:
                            return
                        # if pos >= len(rst):
                        #     return
                        if len(rst[pos])>=sn:
                            now.extend(rst[pos][0:sn])
                            getcardwitht(pos+1,now)
                            for x in range(0,sn):
                                now.pop()
                        getcardwitht(pos+1,now)
                        return
                    if sn > 0:
                        getcardwitht(0,[])
                    else:
                        ans.append(ret)
            else:
                l = 0
    if 3 in lst.values():
        sl = 0
        sn = 0
        mn = 100
        for k,i in lst.items():
            if i == 3:
                sl += 1
                mn=min([mn,k])
            else:
                sn = i
        get5(sl,sn,mn)
    elif len(lst) == 0:
        for sl in range(1,13):
            get5(sl,0,-1)
            get5(sl,1,-1)
            get5(sl,2,-1)
    
    # 四帯二 or 航天飞机
    def get6(sl,sn,mn):
        l = 0
        for i in range(12,mn,-1):
            if len(card[i]) > 3:
                l += 1
                if l>=sl:
                    ret = [x[0] for x in card[i:i+sl]]+[x[1] for x in card[i:i+sl]]+[x[2] for x in card[i:i+sl]]+[x[3] for x in card[i:i+sl]]
                    rst = card[0:i]+card[i+sl:]
                    def getcardwitht(pos, now):
                        if len(now)==sl*sn:
                            ans.append(ret+now)
                            return
                        if (len(rst) - pos)*sn + len(now) < sl*sn:
                            return
                        if len(rst[pos])>=sn:
                            now.extend(rst[pos][0:sn])
                            getcardwitht(pos+1,now)
                            for x in range(0,sn):
                                now.pop()
                        getcardwitht(pos+1,now)
                        return
                    if sn > 0:
                        getcardwitht(0,[])
                    else:
                        ans.append(ret)
            else:
                l = 0
    if 4 in lst.values() and len(lst)>1:
        sl = 0
        sn = 0
        mn = 100
        for k,i in lst.items():
            if i == 4:
                sl+=1
                mn=min([mn,k])
            else:
                sn = i
        get6(sl,sn,mn)
    elif len(lst) == 0:
        for sl in range(1,13):
            get5(sl,0,-1)
            get5(sl,1,-1)
            get5(sl,2,-1)
    # 炸弹
    if not (4 in lst.values() and len(lst) == 1):
        if not (13 in lst.values() and 14 in lst.values() and len(lst) == 2):
            for i in range(0,13):
                if len(card[i]) == 4:
                    ans.append(card[i])
    else:
        for i in range(min(lst.keys())+1,13):
            if len(card[i]) == 4:
                ans.append(card[i])
    # 火箭
    if len(card[13])>0 and len(card[14]) > 0:
        ans.append(card[13]+card[14])
    return ans

if __name__ == "__main__":
    with open("test.txt","w") as f:
        x = getoperation(list(range(0,54)),[])
        f.write("{}".format(len(x))+'\n')
        print(len(x))
        for i in x:
            f.write(' '.join(map(str,sorted(i)))+'\n')
