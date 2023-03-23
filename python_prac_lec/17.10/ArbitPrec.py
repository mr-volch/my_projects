from decimal import *

def to_func(func):
    def  f(x):
        return eval(func)
    return f

def find_root(f, eps):
    getcontext().prec = eps + 3
    eps = Decimal(10**(-Decimal(eps + 2)))
    left_x = Decimal('-1.5')
    right_x = Decimal('1.5')
    while (right_x - left_x) > eps:
        new_x = (left_x + right_x)/2
        if f(new_x)*f(right_x) <= 0:
            left_x = new_x
        else:
            right_x = new_x
    return new_x

func = input()
D = int(input())
func = to_func(func)
cur = find_root(func, D)
print(f'{cur:0.{D}f}')
