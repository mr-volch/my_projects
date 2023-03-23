l = input()
if l:
    l = eval(l)
    if isinstance(l, tuple):
        l = list(l)
        for i in range(len(l) - 1):
            for j in range(len(l) -  i - 1):
                if l[j + 1]**2 % 100 < l[j]**2 % 100:
                    l[j+1],l[j] = l[j],l[j+1]
    else:
        l = [l]
print(l)
