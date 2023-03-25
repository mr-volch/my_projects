intro = [int(i) for i in input().split()]

size_y = intro[0]
size_x = intro[1]

matr = [0 for i in range(size_y)]

for i in range(size_y):
    matr[i] = [int(j) for j in input().split()]

sum_matr = [[0 for i in range(size_x)] for j in range(size_y)]

sum_matr[0][0] = matr[0][0]

for i in range(1, size_x):
   sum_matr[0][i] = sum_matr[0][i-1] + matr[0][i]

for i in range(1, size_y):
    sum_matr[i][0] = sum_matr[i-1][0] + matr[i][0]

for i1 in range(1, size_y):
    for i2 in range(1, size_x):
        sum_matr[i1][i2] = matr[i1][i2] + sum_matr[i1-1][i2] + sum_matr[i1][i2-1] - sum_matr[i1-1][i2-1]

for j in range(intro[2]):
    cur = [(int(k)-1) for k in input().split()]
    x1,y1,x2,y2 = cur[0],cur[1],cur[2],cur[3]

    cur_sum = sum_matr[x2][y2]
    
    if x1 >= 1:
        cur_sum -= sum_matr[x1-1][y2]

    if y1 >= 1:
        cur_sum -= sum_matr[x2][y1-1]

    if x1 >= 1 and y1 >= 1:
        cur_sum += sum_matr[x1-1][y1-1]

    print(cur_sum)
