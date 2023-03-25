def cheapest_dinner(N, costs):
    if N == 1 or N == 2:
        return costs[0]
        
    matrix = [[0 for i in range(j+2)] for j in range(N)]
    print(matrix)
    if costs[0] > 100:
        matrix[0][0] = -1
        matrix[0][1] = costs[0]
    else:
        matrix[0][0] = costs[0]
        matrix[0][1] = -1
        
    for i in range(1, N):
        cur = matrix[i-1][i]
        cur_cost = costs[i]
        
        if cur == -1 or cur_cost <= 100:
            matrix[i][i+1] = -1
        else:
            matrix[i][i+1] = cur + cur_cost

        if cur_cost <= 100:
            cur = matrix[i-1][i]
        else:
            cur = matrix[i-1][i-1]
        if cur == -1:
            matrix[i][i] = -1
        else:
            matrix[i][i] = cur + cur_cost

        for j in range(0, i):
            if costs[i] > 100:
                cur_top = matrix[i-1][j-1]
            else:
                cur_top = matrix[i-1][j]
            if j == 0:
                
            cur_bot = matrix[i-1][j+1]

            flag_top = (cur_top == -1)
            flag_bot = (cur_bot == -1)

            if flag_bot and flag_top:
                matrix[i][j] = -1

            elif not(flag_bot) and not(flag_top):
                
                matrix[i][j] = min(cur_top + cur_cost, cur_bot)

            elif flag_bot:
                matrix[i][j] = cur_top + cur_cost

            else:
                matrix[i][j] = cur_bot
        
    return matrix


N = int(input())
costs = [int(input()) for i in range(N)]

res = cheapest_dinner(N, costs)
print(res)
