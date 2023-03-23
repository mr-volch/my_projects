def moar(a,b,n):
    count_a = 0
    count_b = 0

    for i in a:
        if i % n == 0:
            count_a += 1

    for i in b:
        if i % n == 0:
            count_b += 1

    return count_a > count_b



##print(moar((25,0,-115,976,100500,7),(32,5,78,98,10,9,42),5))
