def cheap_way(N, M, matrix):
    min_cost = [[0 for i in range(M)] for j in range(N)]
    min_cost[0][0] = matrix[0][0]
    cur_coords = [1, 0]
    first_pos = [1, 0]

    for i in range(N*M-1):
        if cur_coords[1] == 0:
            cur_coords = min_cost[cur_coords[0]-1][0] + matrix[cur_coords[0]][cur_coords[1]]

        elif cur_coords[0] == 0:
            cur_coords = min_cost[0][cur_coords[1]-1] + matrix[cur_coords[0]][cur_coords[1]]

        else:
            cur_coords = min(min_cost[cur_coords[0]][M-1], min_cost[cur_coords[0]-1][M])

        if cur_coords[0] == 0 or cur_coords[1] == M:
            if first_pos[0] == N:
                first_pos[1] += 1
            else:
                first_pos[0] += 1
            cur_coords[0], cur_coords[1] = first_pos[0], first_pos[1]

        else:
            cur_coords[0] -= 1
            cur_coords[1] += 1

    return min_cost[N-1][M-1]
    
            
        
            
  



cur = [int(i) for i in input().split()]

N, M = cur[0], cur[1]

matrix = [[0 for i in range(M)] for j in range(N)]
for i in range(N):
    cur_string = [int(i) for i in input().split()]
    matrix[i] = cur_string

cheap_way(N, M, matrix)


