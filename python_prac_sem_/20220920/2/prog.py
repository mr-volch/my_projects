s = 0
while s <= 21:
    a = int(input())
    if a <= 0:
        print(a)
        break
    s += a
else:
    print(s)
