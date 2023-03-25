def seesaw(gen):
    s = list(gen)
    l_c = [i for i in s if not i%2]
    l_n = [i for i in s if i%2]

    a = len(l_c)
    b = len(l_n)
    l_all = []
    for i in range(min(a, b)):
        l_all.append(l_c[i])
        l_all.append(l_n[i])

    if a > b:
        l_end = l_c[b:]
    else:
        l_end = l_n[a:]

    for i in l_all:
        yield i

    for i in l_end:
        yield i
