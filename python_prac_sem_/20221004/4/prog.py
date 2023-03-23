from math import *

def calc(s,t,u):
    def f(x):
        y = eval(t)
        x = eval(s)
        return(eval(u))
    return f

a = eval(input())
F = calc(a[0], a[1], a[2])
a = eval(input())
print(F(a))
