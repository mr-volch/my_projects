def read_table():
    table = []
    while 1:
        to_add = (input()).split()
        if len(to_add) == 0:
            break
        tmp = ' '.join(to_add[2:len(to_add)-1])
        to_add = [to_add[0], to_add[1], tmp, to_add[len(to_add) - 1]]
        table.append(to_add)
    return table

def my_key(i):
    tmp = i.split(':')
    return int(tmp[0]) * 3600 + int(tmp[1]) * 60 + int(tmp[2])

def beauty_key(i):
    cur = ' '.join([i[1], i[0], i[2]])
    return cur

def find_best(table):
    tmp = sorted(table, key = lambda i: my_key(i[len(i) - 1]))
    cur = tmp[0][len(tmp[0]) - 1]
    counter = 0
    i = 0
    chemps = []
    borders = [0]

    while i < len(tmp):
        if tmp[i][len(tmp[i])-1] != cur:
            counter += 1
            borders.append(i)
            if counter == 3:
                break
            cur = tmp[i][len(tmp[i])-1]
        chemps.append(tmp[i])
        i += 1
    if i == len(tmp):
        borders.append(i)
    for j in range(len(borders) - 1):
         chemps[borders[j]:borders[j+1]] = sorted(chemps[borders[j]:borders[j+1]], key = lambda i : beauty_key(i))

    ff = len(max(chemps, key = lambda i : len(i[0]))[0])
    sf = len(max(chemps, key = lambda i : len(i[1]))[1])
    tf = len(max(chemps, key = lambda i : len(i[2]))[2])
    for mem in chemps:
        print(mem[0].ljust(ff), mem[1].ljust(sf), mem[2].ljust(tf), mem[3])

table = read_table()
find_best(table)        
