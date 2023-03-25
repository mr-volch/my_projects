a,b,c = eval(input())

if a == 0:
    if b == 0:
        if c == 0:
            print(-1)
        else:
            print(0)
    else:
        print(-c/b)

else:
    d = b**2 - 4*a*c
    if d > 0:
        l = [(-b - d**0.5)/(2*a), (-b + d**0.5)/(2*a)]
        print(min(l), max(l), sep = ' ')

    elif d == 0:
        print(-b/(2*a))

    else:
        print(0)
