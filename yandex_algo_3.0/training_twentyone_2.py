def count_one(N):
    if N == 1:
        return 2
    table_0_1 = [1 for i in range(N-1)]
    table_0 =[2 for i in range(N-1)]
    table_1_1 = [1 for i in range(N-1)]
    for i in range(1, N-1):
        table_0[i] = table_0[i-1] + table_0_1[i-1] + table_1_1[i-1]
        table_0_1[i] = table_0[i-1]
        table_1_1[i] = table_0_1[i-1]

    return table_0_1[-1] + table_0[-1] + table_1_1[-1]
        

N = int(input())
print(count_one(N))
