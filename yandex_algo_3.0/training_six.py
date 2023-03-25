def intersection(A,B,os):
    if os[0]<=A<=os[1] or os[0]<=B<=os[1]:
        return True
    return False



M = int(input())
N = int(input())

os_list = []

for i in range(N):
    cur = input().split()
    Ai = int(cur[0])
    Bi = int(cur[1])

    new_list = []

    for os in os_list:
        if not(intersection(Ai,Bi,os)):
            new_list.append(os)

    new_list.append([Ai,Bi])
    os_list = new_list

print(len(os_list))
            
            
