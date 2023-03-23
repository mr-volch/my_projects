#k = int(input())
import math

def SwapFive(k):
    cur_num = k**2
    prev = 0
    res = k
    tail = cur_num % 10
    i = 1
    while not(tail == k and cur_num // 10 == 0):
        res += tail*(10**i)
        cur_num = (tail*k + cur_num//10)
        tail = cur_num % 10
        i += 1
    return res

##for i in range(1, 10):
##    s = SwapFive(i)
##    print('num', s)
##    print('equ', s*i == s//10 + i * 10**round(math.log(s, 10))) 
