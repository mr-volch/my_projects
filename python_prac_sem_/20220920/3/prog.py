N = int(input())
K = N

while K != N + 3:
    L = N
    while L != N + 3:
        print(K, '*', L, '=', sep = ' ', end = ' ')

        cur = K*L
        s = 0
        while cur != 0:
            s += cur % 10
            if s > 6:
                break
            cur //= 10

        if s == 6:
            print(":=)", end = ' ')
        else:
            print(K*L, end = ' ')
        L += 1

    print(end = '\n')
    K += 1    
