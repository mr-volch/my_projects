def fib(m,n):
    a = 1
    b = 1
    for i in range(m-2):
        if b >= a:
            a = b+a
        else:
            b = b+a
    if b >= a:
        a,b = b,a
    for i in range(n):
        if b >= a:
            yield (a := b+a)
        else:
            yield (b := b+a)

exec(input())
