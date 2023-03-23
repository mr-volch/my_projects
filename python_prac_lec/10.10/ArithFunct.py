def ADD(f,g):
    def add_f(x):
        res = 0
        if callable(f):
            res = f(x)
        else:
            res = f
        if callable(g):
            res += g(x)
        else:
            res += g
        return res
    return add_f

def SUB(f,g):
    def sub_f(x):
        res = 0
        if callable(f):
            res = f(x)
        else:
            res = f
        if callable(g):
            res -= g(x)
        else:
            res -= g
        return res
    return sub_f

def MUL(f,g):
    def mul_f(x):
        res = 0
        if callable(f):
            res = f(x)
        else:
            res = f
        if callable(g):
            res *= g(x)
        else:
            res *= g
        return res
    return mul_f

def DIV(f,g):
    def div_f(x):
        res = 0
        if callable(f):
            res = f(x)
        else:
            res = f
        if callable(g):
            res /= g(x)
        else:
            res /= g
        return res
    return div_f

##from math import *
##
##f = SUB(sin, cos)
##print(f(12), sin(12)-cos(12))
##
##g = DIV(sin, cos)
##print(g(pi/6), tan(pi/6))
##
##h = MUL(exp, 0.1)
##print(h(2), e**2/10)
##
##t = ADD(len, sum)
##print(t(range(5)))
##print(callable((1,2)))
##f = ADD(lambda s: type(s)(sorted(s)), lambda s: type(s)(reversed(s)))
##print(f((3,2,7,6)))
##print(ADD(bin, hex)(100))
