def side(l):
    return max(l) - min(l) 

i = 0
l1 = []
l2 = []
l3 = []
while a := input():
    a = eval(a)
    l1.append(float(a[0]))
    l2.append(float(a[1]))
    l3.append(float(a[2]))

print(side(l1) * side(l2) * side(l3))
