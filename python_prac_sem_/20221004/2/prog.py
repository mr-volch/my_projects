def Pareto(*params):
    out_list = []
    for i in params:
        for j in params:
            if (j[0] >= i[0] and j[1] >= i[1]) and (j[0] > i[0] or j[1] > i[1]):
                break
        else:
            out_list.append(i)
    return(tuple(out_list))

cur = eval(input())
print(Pareto(*cur))
