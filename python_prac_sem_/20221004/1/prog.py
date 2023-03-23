def b_search(a,b):
    if not b:
        return False
    c = b[len(b)//2]
    if c == a:
        return True
    elif c > a:
        return b_search(a,b[:len(b)//2])
    else:
        return b_search(a,b[len(b)//2 + 1:])

d = eval(input())
print(b_search(d[0], d[1]))
