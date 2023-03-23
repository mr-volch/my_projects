from math import *

def to_func(func):
    def  f(x):
        return eval(func)
    return f

in_tup = input().split()
W = int(in_tup[0])
H = int(in_tup[1])
A = int(in_tup[2])
B = int(in_tup[3])
f = to_func(in_tup[4])
screen = [[' ']*W for i in range(H)]

prev = -1
cur = -1


if True:
    values_list = []
    for i in range (W):
        s = f(A + (B-A) * (i/(W-1)))
        values_list.append(s)
    AY = min(values_list)
    BY = max(values_list)
    main_k = (H-1)/(BY-AY)
    prev = -1
    for i in range (W):
        cur = H - (round((values_list[i]-AY)*main_k)) - 1
        screen[cur][i] = '*'
        if prev != -1:
            for j in range(min(prev,cur) + 1, max(prev,cur)):
                screen[j][i-1] = '*'               
        prev = cur

for l in screen:
    print("".join(l))

