def my_minus(a, b):
    if (isinstance(a,list)) or (isinstance(a,tuple)):
        l = []
        for i in a:
            if i not in b:
                l.append(i)
        if isinstance(a,tuple):
            l = tuple(l)
        return l
    return a-b

in_p = eval(input())
print(my_minus(in_p[0], in_p[1]))
