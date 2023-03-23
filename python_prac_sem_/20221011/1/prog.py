from fractions import Fraction

def is_root(A,B,a,b,s,w):
    left_side_up = 0
    left_side_down = 0
    for i in range(a+1):
        left_side_up += A[i] * (s ** (a-i))
    for i in range(b+1):
        left_side_down += B[i] * (s ** (b-i))
    if left_side_down == 0:
        return False
    return left_side_up/left_side_down == w  

in_tuple = eval(input())
s = Fraction(in_tuple[0])
w = Fraction(in_tuple[1])
a = int(in_tuple[2])
A = list(in_tuple[3:a+4])

b = int(in_tuple[a+4])
B = list(in_tuple[a+5:a+b+6])
for i in range(len(A)):
    A[i] = Fraction(A[i])
for i in range(len(B)):
    B[i] = Fraction(B[i])

print(is_root(A,B,a,b,s,w))
