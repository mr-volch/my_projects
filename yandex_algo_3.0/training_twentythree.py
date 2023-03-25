def find_min_ops(N):
    if N == 1:
        return [0, 1]
    
    table = [[0, 0] for i in range(N)]
    for n in range(2, N+1):
        cur_vals = [table[n-2][0], 1000001, 1000001]
        if n % 2 == 0:
            cur_vals[1] = table[n//2 - 1][0]
        if n % 3 == 0:
            cur_vals[2] = table[n//3 - 1][0]

        if cur_vals[0] <= cur_vals[1] and cur_vals[0] <= cur_vals[2]:
            ind = 0
        elif cur_vals[1] <= cur_vals[0] and cur_vals[1] <= cur_vals[2]:
            ind = 1
        else:
            ind = 2

        table[n-1] = [cur_vals[ind] + 1, ind]

    res = [table[-1][0], N]
    cur_num = N
    while cur_num != 1:
        match table[cur_num-1][1]:
            case 0:
                cur_num -= 1
            case 1:
                cur_num //= 2
            case 2:
                cur_num //= 3
        res.append(cur_num)
    return res

N = int(input())

res = find_min_ops(N)
print(res[0])

way = res[1:]
way.reverse()
print(*way)
    
