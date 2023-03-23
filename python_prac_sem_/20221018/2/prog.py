from math import *

def make_dict(k, v):
    d = {}
    for i in range(len(k) - 1):
        d[k[i]] = eval(v[i])
    return d

func_dict = {}
total_str = 0

while True:
    total_str += 1
    s = input()
    if s[0] == ':':
        s = s[1:].split()
        func_dict.setdefault(s[0], [])
        func_dict[s[0]] = s[1:len(s)]

    else:
        new_s = s.split()
        if new_s[0] == 'quit':
            btw = s[8:len(s)-3]
            print(len(func_dict) + 1, btw, total_str, sep = '')
            break
            
        cur = func_dict[new_s[0]]
        tmp_dict = make_dict(cur, new_s[1:])
        print(eval(cur[len(cur)-1], globals(), tmp_dict)) 
