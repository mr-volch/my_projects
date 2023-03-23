a = str(input())
b = str(input())

if len(b) == 0:
    print('YES')

elif len(b) == 1:
    if b in a:
        print("YES")
    else:
        print("NO")

else:
    arr_first = []
    arr_second = []
    cur = 0

    i = 0
    for l in a:
        if l == b[0]:
            arr_first.append(i)
        if l == b[1]:
            arr_second.append(i)
        i += 1

##print(arr_first)
##print(arr_second)

    flag = 1
    for x in arr_first:
        for y in arr_second:
            cur_dist = 2*y-x
            if y-x <= 0:
                continue
            for n in range(2,len(b)):
                if cur_dist >= len(a):
                    break
                if a[cur_dist] != b[n]:
                    break
                cur_dist += y-x
            else:
##              print(cur_dist, y-x)
                print("YES")
                flag = 0
                break
        else:
            continue
        break

    if flag:
        print("NO")
