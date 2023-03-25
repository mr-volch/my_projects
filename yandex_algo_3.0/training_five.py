import string

def find_min(values,left,right):
    cur_min = [left,values[left]]
    for i in range(left,right):
        if values[i] < cur_min[1]:
            cur_min[1] = values[i]
            cur_min[0] = i
    return cur_min



def add_goodness(letters, left, right):
    if left >= right or max(letters[left:right]) == 0:
        return 0
    cur = find_min(letters,left,right)
    for i in range(left, right):
        letters[i] -= cur[1]
    return (right-left-1)*cur[1] + add_goodness(letters, left, cur[0]) + add_goodness(letters, cur[0] + 1, right)
    

N = int(input())

letters = dict([(i, int(input())) for i in range(N)])

goodness = add_goodness(list(letters.values()), 0, N)
print(goodness)
