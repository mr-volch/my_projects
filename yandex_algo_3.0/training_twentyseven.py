def cheap_way(N, M, matrix):
    max_cost = [[0 for i in range(M)] for j in range(N)]
    max_cost[0][0] = (matrix[0][0], 'I')
    if N == 1:
        cur_coords = [0, 1]
        first_pos = [0, 1]
    else:
        cur_coords = [1, 0]
        first_pos = [1, 0]

    for i in range(N*M-1):
        if cur_coords[1] == 0:
            cur = (max_cost[cur_coords[0]-1][0][0] + matrix[cur_coords[0]][cur_coords[1]], 'D')

        elif cur_coords[0] == 0:
            cur = (max_cost[0][cur_coords[1]-1][0] + matrix[cur_coords[0]][cur_coords[1]], 'R')

        else:
            cmp_1 = max_cost[cur_coords[0]][cur_coords[1]-1][0]
            cmp_2 = max_cost[cur_coords[0]-1][cur_coords[1]][0]
            if cmp_1 > cmp_2:
                cur = (cmp_1 + matrix[cur_coords[0]][cur_coords[1]], 'R')
            else:
                cur = (cmp_2 + matrix[cur_coords[0]][cur_coords[1]], 'D')

        max_cost[cur_coords[0]][cur_coords[1]] = cur

        if cur_coords[0] == 0 or cur_coords[1] == M-1:
            if first_pos[0] == N-1:
                first_pos[1] += 1
            else:
                first_pos[0] += 1
            cur_coords[0], cur_coords[1] = first_pos[0], first_pos[1]

        else:
            cur_coords[0] -= 1
            cur_coords[1] += 1

    cur_coords = [N-1, M-1]
    res = [max_cost[N-1][M-1][0]]
    cur_dir = max_cost[cur_coords[0]][cur_coords[1]][1]

    while cur_dir != 'I':
        res.append(cur_dir)
        match cur_dir:
            case 'D':
                cur_coords[0] -= 1
            case 'R':
                cur_coords[1] -= 1
        cur_dir = max_cost[cur_coords[0]][cur_coords[1]][1]
        
    return res


cur = [int(i) for i in input().split()]

N, M = cur[0], cur[1]

matrix = [[0 for i in range(M)] for j in range(N)]
for i in range(N):
    cur_string = [int(i) for i in input().split()]
    matrix[i] = cur_string

res = cheap_way(N, M, matrix)

print(res[0])
res.reverse()
print(*res[:-1])
