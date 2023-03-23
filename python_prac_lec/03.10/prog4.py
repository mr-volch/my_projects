mat = [eval(input())]

if isinstance(mat[0], tuple):
    for i in range(len(mat[0])-1):
        l = eval(input())
        mat.append(l)

    for num in range(len(mat)):
        i = num
        j = 0
        while not(i == 0 and j == num):
            print(mat[i][j], end = ',')
            if (j == num):
                i -= 1
            else:
                j+=1
        print(mat[i][j])

else:
    print(mat[0])
