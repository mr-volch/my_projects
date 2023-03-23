import itertools as it

def slide(seq, n):
    i = 0
    c = enumerate(seq)
    while True:
        new_s = it.tee(c, 2)
        c  = new_s[1]
        d = list(it.islice(new_s[0],i,i+n))
        e = [j[1] for j in d]
        if len(e) == 0:
            break
        yield from (e)
        i += 1


exec(input())
