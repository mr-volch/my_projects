def cheap_way(N, M):
    ways = [[0 for i in range(M)] for j in range(N)]
    ways[0][0] = 1
    cur_coords = [1, 0]
    first_pos = [1, 0]

    for i in range(N*M-1):
        statement_1 = not(cur_coords[0] - 2 < 0 or cur_coords[1] - 1 < 0)
        statement_2 = not(cur_coords[0] - 1 < 0 or cur_coords[1] - 2 < 0)
        if statement_1:
            ways[cur_coords[0]][cur_coords[1]] += ways[cur_coords[0]-2][cur_coords[1]-1]
        if statement_2:
            ways[cur_coords[0]][cur_coords[1]] += ways[cur_coords[0]-1][cur_coords[1]-2]
            

        if cur_coords[0] == 0 or cur_coords[1] == M-1:
            if first_pos[0] == N-1:
                first_pos[1] += 1
            else:
                first_pos[0] += 1
            cur_coords[0], cur_coords[1] = first_pos[0], first_pos[1]

        else:
            cur_coords[0] -= 1
            cur_coords[1] += 1

    return ways[N-1][M-1]


cur = input().split()

N, M = int(cur[0]), int(cur[1])


print(cheap_way(N, M))
